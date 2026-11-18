// =============================================================
//  MiniRetroTV  -  Sesli, MicroSD'li, Şarjlı & Wi-Fi'lı Mini Retro TV
//  Donanım: ESP32-C3 SuperMini + 1.69" 240x280 ST7789 SPI IPS Ekran
//           + MAX98357A I2S Amfi + Mini Hoparlör + MicroSD (SPI)
//
//  Özellikler:
//    - Müşteri dostu Captive Portal + tarayıcıdan Ev Wi-Fi tarama/kaydetme
//      (config.h düzenlemeden telefondan ağ seçip bağlanabilir!)
//    - Senkronize MJPEG video (280x240) + 16 kHz mono PCM ses
//    - MicroSD + dahili LittleFS otomatik geçiş
//    - CRT açılış/kapanış efekti, sesli karlanma paraziti, yeşil CH OSD
// =============================================================
#include <Arduino.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include <vector>
#include <algorithm>
#include <Arduino_GFX_Library.h>
#include <JPEGDEC.h>
#include "config.h"
#include "web_page.h"

#if USE_AUDIO
#include <ESP_I2S.h>
I2SClass i2s;
bool audioReady = false;
#endif

// ---------------- Donanım nesneleri ----------------
// Paylaşımlı SPI: Ekran + MicroSD aynı SCK/MOSI hattını güvenli şekilde paylaşır
Arduino_DataBus *bus = new Arduino_ESP32SPI(
    PIN_LCD_DC, PIN_LCD_CS, PIN_LCD_SCK, PIN_LCD_MOSI, PIN_SD_MISO, FSPI, true);
// 240x280 panel, satır ofseti 20 (ST7789V2 1.69")
Arduino_GFX *gfx = new Arduino_ST7789(bus, PIN_LCD_RST, LCD_ROTATION, true, 240, 280, 0, 20, 0, 20);

JPEGDEC jpeg;
WebServer server(80);
DNSServer dnsServer;
Preferences prefs;
fs::FS *store = &LittleFS;       // MicroSD takılıysa &SD, yoksa &LittleFS
bool usingSD = false;

// ---------------- Durum ----------------
std::vector<String> channels;
int      curCh = 0;
uint8_t  chFps = DEFAULT_FPS;
File     vf;                     // aktif .mjpeg video dosyası
File     af;                     // eşleşen .pcm ses dosyası (varsa)
uint8_t *rb = nullptr;           // JPEG kare tamponu
size_t   rbLen = 0, scanPos = 2;
uint16_t *noiseBuf = nullptr;    // karlanma efekti (şerit tamponu)
int16_t  audioBuf[1024];         // ses çıkış tamponu

bool     wifiOn = false, apMode = false, tvOff = false;
bool     uploading = false, uploadFailed = false;
bool     pendingWifiReconnect = false;
uint32_t reconnectAt = 0;
File     upFile;
String   upName, ipText, savedSsid, savedPass;
uint32_t lastWeb = 0, nextFrameAt = 0, osdUntil = 0, staticUntil = 0, infoUntil = 0;
uint8_t  brightness = DEFAULT_BRIGHTNESS;
uint8_t  volume = DEFAULT_VOLUME;
uint8_t  defFps = DEFAULT_FPS;

// ---------------- Yardımcılar ----------------
static String vpath(const String &n) { return String(VIDEO_DIR) + "/" + n; }

static String pcmPathFor(const String &mjpegName) {
  if (mjpegName.endsWith(".mjpeg"))
    return vpath(mjpegName.substring(0, mjpegName.length() - 6) + ".pcm");
  return "";
}

static String jsonEsc(const String &s) {
  String o;
  o.reserve(s.length() + 4);
  for (unsigned i = 0; i < s.length(); i++) {
    char c = s[i];
    if (c == '"' || c == '\\') { o += '\\'; o += c; }
    else if ((uint8_t)c >= 0x20) o += c;
  }
  return o;
}

static uint64_t storageTotal() { return usingSD ? SD.totalBytes() : LittleFS.totalBytes(); }
static uint64_t storageUsed()  { return usingSD ? SD.usedBytes()  : LittleFS.usedBytes(); }

// ---------------- Ses yardımcıları (MAX98357A) ----------------
static void initAudio() {
#if USE_AUDIO
  i2s.setPins(PIN_I2S_BCLK, PIN_I2S_LRC, PIN_I2S_DIN);
  audioReady = i2s.begin(I2S_MODE_STD, AUDIO_RATE, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO);
#endif
}

static void writeAudioScaled(int16_t *samples, size_t count) {
#if USE_AUDIO
  if (!audioReady || count == 0) return;
  const int32_t gain = (int32_t)volume * 256 / 100;
  for (size_t i = 0; i < count; i++) {
    samples[i] = (int16_t)(((int32_t)samples[i] * gain) >> 8);
  }
  i2s.write((uint8_t *)samples, count * sizeof(int16_t));
#endif
}

static void playStaticNoiseChunk(size_t samples) {
#if USE_AUDIO
  if (!audioReady || volume == 0) return;
  size_t left = samples;
  while (left > 0) {
    size_t n = std::min(left, (size_t)512);
    for (size_t i = 0; i < n; i++) {
      audioBuf[i] = (int16_t)(((int32_t)(esp_random() & 0xFFFF) - 32768) / 5);
    }
    writeAudioScaled(audioBuf, n);
    left -= n;
  }
#endif
}

static void feedAudioForFrame() {
#if USE_AUDIO
  if (!audioReady || !af) return;
  size_t samplesNeeded = AUDIO_RATE / chFps;
  while (samplesNeeded > 0) {
    size_t chunk = std::min(samplesNeeded, (size_t)1024);
    int got = af.read((uint8_t *)audioBuf, chunk * 2);
    if (got <= 0) break;
    writeAudioScaled(audioBuf, got / 2);
    samplesNeeded -= (got / 2);
  }
#endif
}

// ---------------- Ekran yardımcıları ----------------
static void setBacklight(uint8_t v) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcWrite(PIN_LCD_BL, v);
#else
  ledcWrite(0, v);
#endif
}

static int jpegDraw(JPEGDRAW *d) {
  gfx->draw16bitBeRGBBitmap(d->x, d->y, d->pPixels, d->iWidth, d->iHeight);
  return 1;
}

static void colorBars(const char *l1, const char *l2, const char *l3) {
  static const uint16_t c[7] = {0xFFFF, 0xFFE0, 0x07FF, 0x07E0, 0xF81F, 0xF800, 0x001F};
  const int bw = SCREEN_W / 7 + 1, bandY = SCREEN_H - 78;
  for (int i = 0; i < 7; i++) gfx->fillRect(i * (SCREEN_W / 7), 0, bw, bandY, c[i]);
  gfx->fillRect(0, bandY, SCREEN_W, SCREEN_H - bandY, 0x0000);
  gfx->setTextColor(0xFFFF);
  gfx->setTextSize(2);
  gfx->setCursor(8, bandY + 8);  gfx->print(l1);
  gfx->setTextColor(0x07E0);
  gfx->setCursor(8, bandY + 32); gfx->print(l2);
  gfx->setTextColor(0xBDF7);
  gfx->setTextSize(1);
  gfx->setCursor(8, bandY + 58); gfx->print(l3);
}

static void showInfo(uint32_t ms) {
  String head = channels.empty() ? String("VIDEO YOK") : (usingSD ? String("MINI TV [SD]") : String("MINI TV"));
  String l2 = wifiOn ? ("http://" + ipText) : String("Wi-Fi kapali");
  String l3 = wifiOn ? (apMode ? String("Ag: " WIFI_AP_SSID " / " WIFI_AP_PASS) : String("http://" MDNS_NAME ".local"))
                     : String("Acmak icin BOOT'a uzun bas");
  colorBars(head.c_str(), l2.c_str(), l3.c_str());
  infoUntil = millis() + ms;
}

static void crtOn() {
  gfx->fillScreen(0x0000);
  const int cy = SCREEN_H / 2;
  for (int w = 10; w <= SCREEN_W; w += 30) { gfx->fillRect((SCREEN_W - w) / 2, cy - 1, w, 2, 0xFFFF); delay(8); }
  for (int h = 2; h <= SCREEN_H; h += 26) { gfx->fillRect(0, cy - h / 2, SCREEN_W, h, 0xFFFF); delay(10); }
  gfx->fillScreen(0x0000);
}

static void crtOff() {
  const int cy = SCREEN_H / 2;
  for (int h = SCREEN_H; h >= 4; h -= 28) {
    gfx->fillScreen(0x0000);
    gfx->fillRect(0, cy - h / 2, SCREEN_W, h, 0xFFFF);
    delay(10);
  }
  for (int w = SCREEN_W; w >= 6; w -= 32) {
    gfx->fillScreen(0x0000);
    gfx->fillRect((SCREEN_W - w) / 2, cy - 1, w, 2, 0xFFFF);
    delay(8);
  }
  gfx->fillScreen(0x0000);
  delay(80);
}

static const int NOISE_STRIP_H = 16;
static void drawStatic() {
  playStaticNoiseChunk(480);
  if (!noiseBuf) { gfx->fillScreen(0x0000); return; }
  for (int y = 0; y < SCREEN_H; y += NOISE_STRIP_H) {
    int h = std::min(NOISE_STRIP_H, SCREEN_H - y);
    int n = SCREEN_W * h;
    for (int i = 0; i < n; i += 4) {
      uint32_t r = esp_random();
      for (int k = 0; k < 4 && (i + k) < n; k++) {
        uint8_t v = (r >> (k * 8)) & 0xFF;
        noiseBuf[i + k] = ((v >> 3) << 11) | ((v >> 2) << 5) | (v >> 3);
      }
    }
    gfx->draw16bitRGBBitmap(0, y, noiseBuf, SCREEN_W, h);
  }
}

static void drawOSD() {
  gfx->setTextSize(3);
  gfx->setTextColor(0x07E0);
  gfx->setCursor(SCREEN_W - 84, 10);
  gfx->printf("CH%d", curCh + 1);
}

// ---------------- Kanal listesi ----------------
static void scanChannels() {
  channels.clear();
  File d = store->open(VIDEO_DIR);
  if (!d || !d.isDirectory()) { store->mkdir(VIDEO_DIR); return; }
  for (File f = d.openNextFile(); f; f = d.openNextFile()) {
    String n = f.name();
    int s = n.lastIndexOf('/');
    if (s >= 0) n = n.substring(s + 1);
    if (!f.isDirectory() && n.endsWith(".mjpeg")) channels.push_back(n);
  }
  std::sort(channels.begin(), channels.end());
}

static uint8_t fpsFromName(const String &n) {
  int k = n.lastIndexOf("_f");
  if (k > 0) {
    int v = n.substring(k + 2, n.length() - 6).toInt();
    if (v >= 1 && v <= 30) return v;
  }
  return defFps;
}

static void closeActiveMedia() {
  if (vf) vf.close();
  if (af) af.close();
}

static bool openChannel(int i) {
  closeActiveMedia();
  rbLen = 0; scanPos = 2;
  if (channels.empty()) return false;
  int n = channels.size();
  curCh = ((i % n) + n) % n;
  chFps = fpsFromName(channels[curCh]);
  vf = store->open(vpath(channels[curCh]), "r");
  String ap = pcmPathFor(channels[curCh]);
  if (ap.length() && store->exists(ap)) af = store->open(ap, "r");
  return (bool)vf;
}

static void switchChannel(int i) {
  if (channels.empty()) return;
  openChannel(i);
  staticUntil = millis() + STATIC_MS;
  osdUntil = staticUntil + 1500;
  infoUntil = 0;
  prefs.putUChar("ch", curCh);
}

// ---------------- MJPEG okuyucu ----------------
static size_t readFrame() {
  for (;;) {
    if (rbLen >= 2 && !(rb[0] == 0xFF && rb[1] == 0xD8)) {
      size_t k = 1;
      while (k + 1 < rbLen && !(rb[k] == 0xFF && rb[k + 1] == 0xD8)) k++;
      memmove(rb, rb + k, rbLen - k);
      rbLen -= k; scanPos = 2;
    }
    if (scanPos < 2) scanPos = 2;
    for (; scanPos + 1 < rbLen; scanPos++)
      if (rb[scanPos] == 0xFF && rb[scanPos + 1] == 0xD9) return scanPos + 2;
    if (rbLen >= FRAME_BUF_SIZE) { rbLen = 0; scanPos = 2; }
    size_t want = std::min((size_t)8192, (size_t)FRAME_BUF_SIZE - rbLen);
    int n = vf.read(rb + rbLen, want);
    if (n <= 0) return 0;
    rbLen += n;
  }
}

static void consumeFrame(size_t len) {
  memmove(rb, rb + len, rbLen - len);
  rbLen -= len; scanPos = 2;
}

static void playStep() {
  const uint32_t now = millis();
  if (uploading || channels.empty()) return;
  if ((int32_t)(infoUntil - now) > 0) return;
  if ((int32_t)(staticUntil - now) > 0) { drawStatic(); return; }
  if ((int32_t)(nextFrameAt - now) > 0) return;
  nextFrameAt = now + 1000 / chFps;

  if (!vf && !openChannel(curCh)) return;
  size_t len = readFrame();
  if (!len) {
    vf.seek(0); rbLen = 0; scanPos = 2;
    if (af) af.seek(0);
    len = readFrame();
    if (!len) return;
  }
  if (jpeg.openRAM(rb, len, jpegDraw)) {
    jpeg.setPixelType(RGB565_BIG_ENDIAN);
    int x = std::max(0, (SCREEN_W - jpeg.getWidth()) / 2);
    int y = std::max(0, (SCREEN_H - jpeg.getHeight()) / 2);
    jpeg.decode(x, y, 0);
    jpeg.close();
  }
  consumeFrame(len);
  feedAudioForFrame();
  if ((int32_t)(osdUntil - now) > 0) drawOSD();
}

// ---------------- Wi-Fi & Captive Portal ----------------
static void loadSavedWifi() {
  savedSsid = prefs.getString("ssid", "");
  savedPass = prefs.getString("pass", "");
  if (savedSsid.isEmpty() && strlen(WIFI_STA_SSID) > 0) {
    savedSsid = WIFI_STA_SSID;
    savedPass = WIFI_STA_PASS;
  }
}

static void wifiStart() {
  loadSavedWifi();
  bool sta = false;
  if (!savedSsid.isEmpty()) {
    colorBars("WI-FI BAGLANIYOR", savedSsid.c_str(), "Lutfen bekleyin...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(savedSsid.c_str(), savedPass.c_str());
    uint32_t t = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - t < 10000) delay(200);
    sta = WiFi.status() == WL_CONNECTED;
  } else {
    colorBars("MINI TV", "Wi-Fi aciliyor...", "");
  }
  if (sta) {
    apMode = false;
    ipText = WiFi.localIP().toString();
    WiFi.setSleep(true);
  } else {
    // Ev ağı yoksa veya bağlanamazsa otomatik kendi kurulum ağına (AP + Captive Portal) geçer
    apMode = true;
    WiFi.mode(WIFI_AP_STA); // STA aktif kalır ki tarayıcıdan yakındaki ağları tarayabilelim!
    WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASS);
    ipText = WiFi.softAPIP().toString();
    dnsServer.start(53, "*", WiFi.softAPIP());
  }
  if (MDNS.begin(MDNS_NAME)) MDNS.addService("http", "tcp", 80);
  server.begin();
  wifiOn = true;
  lastWeb = millis();
}

static void wifiStop() {
  server.stop();
  dnsServer.stop();
  MDNS.end();
  if (apMode) WiFi.softAPdisconnect(true);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  wifiOn = false;
  apMode = false;
}

// ---------------- Web API ----------------
static void touch() { lastWeb = millis(); }

static String sanitize(String n) {
  int s = n.lastIndexOf('/');
  if (s >= 0) n = n.substring(s + 1);
  bool isPcm = n.endsWith(".pcm");
  int dot = n.lastIndexOf('.');
  if (dot > 0) n = n.substring(0, dot);
  String o;
  for (unsigned i = 0; i < n.length() && o.length() < 28; i++) {
    char c = n[i];
    if (isalnum((unsigned char)c) || c == '-' || c == '_') o += c;
    else if (c == ' ') o += '_';
  }
  if (o.isEmpty()) o = "video";
  return o + (isPcm ? ".pcm" : ".mjpeg");
}

static int indexOfChannel(const String &n) {
  for (size_t i = 0; i < channels.size(); i++) if (channels[i] == n) return i;
  return -1;
}

static void hRoot() { touch(); server.send(200, "text/html; charset=utf-8", INDEX_HTML); }

static void hList() {
  touch();
  String j = "{\"files\":[";
  for (size_t i = 0; i < channels.size(); i++) {
    File f = store->open(vpath(channels[i]), "r");
    size_t sz = f ? f.size() : 0;
    if (f) f.close();
    String ap = pcmPathFor(channels[i]);
    bool hasAud = ap.length() && store->exists(ap);
    if (hasAud) {
      File fa = store->open(ap, "r");
      if (fa) { sz += fa.size(); fa.close(); }
    }
    if (i) j += ',';
    j += "{\"name\":\"" + jsonEsc(channels[i]) + "\",\"size\":" + String(sz) + ",\"audio\":" + (hasAud ? "true" : "false") + "}";
  }
  j += "],\"current\":" + String(curCh);
  j += ",\"total\":" + String((unsigned long)(storageTotal() / 1024ULL));
  j += ",\"used\":" + String((unsigned long)(storageUsed() / 1024ULL));
  j += ",\"sd\":" + String(usingSD ? "true" : "false");
  j += ",\"ap\":" + String(apMode ? "true" : "false");
  j += ",\"ssid\":\"" + jsonEsc(apMode ? String(WIFI_AP_SSID) : WiFi.SSID()) + "\"";
  j += ",\"savedSsid\":\"" + jsonEsc(savedSsid) + "\"";
  j += ",\"ip\":\"" + jsonEsc(ipText) + "\"";
  j += ",\"brightness\":" + String(brightness);
  j += ",\"volume\":" + String(volume);
  j += ",\"fps\":" + String(defFps) + "}";
  server.send(200, "application/json", j);
}

// Yakındaki Wi-Fi ağlarını tara (Müşteri listeden seçsin!)
static void hWifiScan() {
  touch();
  int n = WiFi.scanNetworks(false, false);
  String j = "{\"nets\":[";
  int added = 0;
  for (int i = 0; i < n && added < 20; i++) {
    String s = WiFi.SSID(i);
    if (s.isEmpty()) continue;
    if (added++) j += ',';
    j += "{\"ssid\":\"" + jsonEsc(s) + "\",\"rssi\":" + String(WiFi.RSSI(i)) + "}";
  }
  WiFi.scanDelete();
  j += "]}";
  server.send(200, "application/json", j);
}

// Ev Wi-Fi bilgisini NVS hafızaya kalıcı kaydet ve bağlan
static void hWifiSave() {
  touch();
  String s = server.arg("ssid");
  String p = server.arg("pass");
  s.trim();
  if (s.isEmpty()) {
    server.send(400, "application/json", "{\"ok\":false,\"error\":\"Wi-Fi adi bos olamaz\"}");
    return;
  }
  prefs.putString("ssid", s);
  prefs.putString("pass", p);
  savedSsid = s;
  savedPass = p;
  server.send(200, "application/json", "{\"ok\":true}");
  pendingWifiReconnect = true;
  reconnectAt = millis() + 800;
}

// Kayıtlı ev Wi-Fi'ını sil ve MiniTV kurulum moduna dön
static void hWifiReset() {
  touch();
  prefs.remove("ssid");
  prefs.remove("pass");
  savedSsid = "";
  savedPass = "";
  server.send(200, "application/json", "{\"ok\":true}");
  pendingWifiReconnect = true;
  reconnectAt = millis() + 800;
}

static void hUploadData() {
  HTTPUpload &u = server.upload();
  touch();
  if (u.status == UPLOAD_FILE_START) {
    uploading = true; uploadFailed = false;
    closeActiveMedia();
    upName = sanitize(u.filename);
    upFile = store->open(vpath(upName), "w");
    if (!upFile) uploadFailed = true;
    colorBars("YUKLENIYOR...", upName.c_str(), usingSD ? "MicroSD karta yaziliyor" : "Hafizaya yaziliyor");
  } else if (u.status == UPLOAD_FILE_WRITE) {
    if (upFile && !uploadFailed && upFile.write(u.buf, u.currentSize) != u.currentSize) uploadFailed = true;
  } else if (u.status == UPLOAD_FILE_END) {
    if (upFile) upFile.close();
    if (uploadFailed) store->remove(vpath(upName));
  } else if (u.status == UPLOAD_FILE_ABORTED) {
    if (upFile) upFile.close();
    store->remove(vpath(upName));
    uploadFailed = true;
    uploading = false;
    scanChannels();
  }
}

static void hUploadDone() {
  uploading = false;
  scanChannels();
  if (uploadFailed) {
    server.send(507, "application/json", "{\"ok\":false,\"error\":\"Yer yetersiz veya yazma hatasi\"}");
    if (channels.empty()) showInfo(0); else switchChannel(curCh);
    return;
  }
  String target = upName;
  if (target.endsWith(".pcm")) target = target.substring(0, target.length() - 4) + ".mjpeg";
  int idx = indexOfChannel(target);
  if (idx >= 0) switchChannel(idx);
  server.send(200, "application/json", "{\"ok\":true}");
}

static void hDelete() {
  touch();
  String n = sanitize(server.arg("name"));
  bool wasCurrent = indexOfChannel(n) == curCh;
  if (wasCurrent) closeActiveMedia();
  bool ok = store->remove(vpath(n));
  String ap = pcmPathFor(n);
  if (ap.length() && store->exists(ap)) store->remove(ap);
  scanChannels();
  if (channels.empty()) showInfo(0);
  else if (wasCurrent) switchChannel(std::min(curCh, (int)channels.size() - 1));
  server.send(ok ? 200 : 404, "application/json", ok ? "{\"ok\":true}" : "{\"ok\":false}");
}

static void hPlay() {
  touch();
  switchChannel(server.arg("i").toInt());
  server.send(200, "application/json", "{\"ok\":true}");
}

static void hSettings() {
  touch();
  if (server.hasArg("b")) {
    brightness = constrain(server.arg("b").toInt(), 5, 255);
    setBacklight(brightness);
    prefs.putUChar("bri", brightness);
  }
  if (server.hasArg("vol")) {
    volume = constrain(server.arg("vol").toInt(), 0, 100);
    prefs.putUChar("vol", volume);
  }
  if (server.hasArg("fps")) {
    defFps = constrain(server.arg("fps").toInt(), 1, 30);
    prefs.putUChar("fps", defFps);
    if (!channels.empty()) chFps = fpsFromName(channels[curCh]);
  }
  server.send(200, "application/json", "{\"ok\":true}");
}

static void setupRoutes() {
  server.on("/", HTTP_GET, hRoot);
  server.on("/api/list", HTTP_GET, hList);
  server.on("/api/wifi/scan", HTTP_GET, hWifiScan);
  server.on("/api/wifi/save", HTTP_POST, hWifiSave);
  server.on("/api/wifi/reset", HTTP_POST, hWifiReset);
  server.on("/api/upload", HTTP_POST, hUploadDone, hUploadData);
  server.on("/api/delete", HTTP_POST, hDelete);
  server.on("/api/play", HTTP_POST, hPlay);
  server.on("/api/settings", HTTP_POST, hSettings);
  // Captive Portal: iOS/Android bağlanınca ayar sayfası otomatik açılır
  server.onNotFound([]() {
    if (apMode && !server.uri().startsWith("/api/")) {
      server.sendHeader("Location", String("http://") + ipText + "/", true);
      server.send(302, "text/plain", "");
      return;
    }
    server.send(404, "text/plain", "Bulunamadi");
  });
}

// ---------------- Buton & Uyku Modu ----------------
static void enterStandby() {
  tvOff = true;
  if (wifiOn) wifiStop();
  crtOff();
  setBacklight(0);
  gfx->displayOff();
}

static void exitStandby() {
  tvOff = false;
  gfx->displayOn();
  setBacklight(brightness);
  crtOn();
  if (channels.empty()) { wifiStart(); showInfo(0); }
  else { openChannel(curCh); osdUntil = millis() + 1500; }
}

static void handleButton() {
  static bool last = HIGH, longDone = false, sleepDone = false;
  static uint32_t downAt = 0;
  const bool s = digitalRead(PIN_BUTTON);
  const uint32_t now = millis();
  if (s != last) {
    if (s == LOW) { downAt = now; longDone = false; sleepDone = false; }
    else if (!longDone && !sleepDone && now - downAt > 40) {
      if (tvOff) exitStandby();
      else switchChannel(curCh + 1);
    }
    last = s;
  }
  if (!tvOff && s == LOW && !longDone && now - downAt > 1500) {
    longDone = true;
    if (wifiOn) wifiStop(); else wifiStart();
    showInfo(3000);
  }
  if (!tvOff && s == LOW && !sleepDone && now - downAt > 4000) {
    sleepDone = true;
    enterStandby();
  }
}

// ---------------- Kurulum ----------------
void setup() {
#if PIN_SYS_EN >= 0
  pinMode(PIN_SYS_EN, OUTPUT);
  digitalWrite(PIN_SYS_EN, HIGH);
#endif
  Serial.begin(115200);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  pinMode(PIN_LCD_CS, OUTPUT); digitalWrite(PIN_LCD_CS, HIGH);
#if USE_SD_CARD
  pinMode(PIN_SD_CS, OUTPUT);  digitalWrite(PIN_SD_CS, HIGH);
#endif
  SPI.begin(PIN_LCD_SCK, PIN_SD_MISO, PIN_LCD_MOSI);

#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcAttach(PIN_LCD_BL, 5000, 8);
#else
  ledcSetup(0, 5000, 8);
  ledcAttachPin(PIN_LCD_BL, 0);
#endif
  setBacklight(0);

  gfx->begin(SPI_HZ);
  gfx->fillScreen(0x0000);

  prefs.begin("minitv", false);
  brightness = prefs.getUChar("bri", DEFAULT_BRIGHTNESS);
  volume     = prefs.getUChar("vol", DEFAULT_VOLUME);
  defFps     = prefs.getUChar("fps", DEFAULT_FPS);
  curCh      = prefs.getUChar("ch", 0);
  setBacklight(brightness);

  initAudio();

  rb = (uint8_t *)ps_malloc(FRAME_BUF_SIZE);
  if (!rb) rb = (uint8_t *)malloc(FRAME_BUF_SIZE);
  noiseBuf = (uint16_t *)malloc(SCREEN_W * NOISE_STRIP_H * 2);

#if USE_SD_CARD
  if (SD.begin(PIN_SD_CS, SPI, SD_SPI_HZ)) {
    usingSD = true;
    store = &SD;
  }
#endif
  if (!usingSD) {
    if (!LittleFS.begin(true)) colorBars("DOSYA SISTEMI HATASI", "SD veya Partition kontrol et", "");
    store = &LittleFS;
  }
  if (!store->exists(VIDEO_DIR)) store->mkdir(VIDEO_DIR);
  scanChannels();
  setupRoutes();

  crtOn();
  if (WIFI_ON_BOOT) wifiStart();
  showInfo(channels.empty() ? 0 : 4000);
  if (!channels.empty()) openChannel(curCh);
}

void loop() {
  handleButton();
  if (tvOff) { delay(50); return; }
  if (pendingWifiReconnect && (int32_t)(millis() - reconnectAt) >= 0) {
    pendingWifiReconnect = false;
    wifiStop();
    wifiStart();
    showInfo(channels.empty() ? 0 : 5000);
  }
  if (wifiOn) {
    if (apMode) dnsServer.processNextRequest();
    server.handleClient();
  }
#if WIFI_AUTO_OFF_MIN > 0
  if (wifiOn && !uploading && millis() - lastWeb > (uint32_t)WIFI_AUTO_OFF_MIN * 60000UL) {
    wifiStop();
    if (channels.empty()) showInfo(0);
  }
#endif
  playStep();
  delay(1);
}
