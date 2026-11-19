// =========================================================================
//  Collectible Display - 1.3" IPS ST7789 + ESP32-S3 Mini Firmware
//  Çift SPI (FSPI Ekran + HSPI 16GB MicroSD) + 25 FPS Akıcı Video Motoru
//  MAX98357A I2S Ses + Ses Kısma/Açma (MUTE) + Çok Fonksiyonlu Buton + OSD
//  Kesintisiz Parçalı Yükleme + Wi-Fi Ev Ağı (STA/AP) & Şifre Yönetimi
// =========================================================================

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <JPEGDEC.h>
#include <LittleFS.h>
#include <SD.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include <vector>
#include "esp_private/brownout.h"

#include "config.h"
#include "web_page.h"

#if USE_AUDIO
#include <ESP_I2S.h>
I2SClass i2s;
bool audioReady = false;
File audioFile;
#endif

// Donanım Nesneleri (Çift Donanımsal SPI)
Adafruit_ST7789 tft(&SPI, TFT_CS, TFT_DC, TFT_RES);
SPIClass sdSPI(HSPI);
JPEGDEC jpeg;
WebServer server(80);
DNSServer dnsServer;
const byte DNS_PORT = 53;
Preferences prefs;

// Depolama Aygıtı (SD Kart veya Dahili Flash Yedek)
fs::FS *storage = &SD;
bool isSDActive = false;

// Video & Ses Yönetimi
struct MediaItem {
  String name;
  size_t size;
  bool hasAudio;
};
std::vector<MediaItem> playlist;
size_t cachedTotalBytes = 0;
size_t cachedUsedBytes = 0;
int currentVideoIdx = -1;
File videoFile;
bool isUploading = false;

// Wi-Fi Ayarları (NVS Kalıcı Hafıza)
String currentApSsid = AP_SSID;
String currentApPass = AP_PASS;
String currentStaSsid = "";
String currentStaPass = "";

// Parçalı Yükleme (Chunked Resumable Upload)
File uploadChunkFile;
String uploadChunkFilePath = "";
unsigned long lastUploadChunkTime = 0;

// Ses Seviyesi ve MUTE Durumu (NVS Kalıcı Hafıza)
uint8_t currentVolume = DEFAULT_VOLUME;
bool isMuted = false;
uint8_t lastUnmutedVolume = DEFAULT_VOLUME;
unsigned long osdUntil = 0;

// Kare Tamponu
uint8_t *frameBuffer = nullptr;
size_t frameBufferSize = MAX_FRAME_SIZE;

// Hızlı Okuma Tamponu (RAM Chunk Buffer - 100 kat hızlı dosya okuma)
static uint8_t fileReadBuf[4096];
static size_t fileReadLen = 0;
static size_t fileReadPos = 0;

inline void resetFastReader() {
  fileReadLen = 0;
  fileReadPos = 0;
}

inline int readFastByte(File &f) {
  if (fileReadPos >= fileReadLen) {
    fileReadLen = f.read(fileReadBuf, sizeof(fileReadBuf));
    fileReadPos = 0;
    if (fileReadLen == 0) return -1;
  }
  return fileReadBuf[fileReadPos++];
}

inline int peekFastByte(File &f) {
  if (fileReadPos >= fileReadLen) {
    fileReadLen = f.read(fileReadBuf, sizeof(fileReadBuf));
    fileReadPos = 0;
    if (fileReadLen == 0) return -1;
  }
  return fileReadBuf[fileReadPos];
}

// Zamanlama
unsigned long lastFrameTime = 0;
unsigned long frameDelayMs = 1000 / DEFAULT_FPS;

// Buton Yönetimi (Tek Tık: Kanal, Çift Tık: Mute, Uzun Bas: Ses Kademesi)
static int buttonClicks = 0;
static unsigned long buttonDownTime = 0;
static unsigned long lastReleaseTime = 0;
static bool buttonPressed = false;
static bool longPressHandled = false;

// =========================================================================
//  JPEGDEC Çizim Geri Çağrısı
// =========================================================================
int JPEGDraw(JPEGDRAW *pDraw) {
  tft.drawRGBBitmap(pDraw->x, pDraw->y, pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);
  return 1;
}

// =========================================================================
//  CRT Parazit Efekti
// =========================================================================
void drawCRTStatic(int ms) {
  unsigned long start = millis();
  while (millis() - start < (unsigned long)ms) {
    for (int y = 0; y < SCREEN_H; y += 8) {
      uint16_t row[SCREEN_W];
      for (int x = 0; x < SCREEN_W; x++) {
        uint8_t r = (uint8_t)esp_random();
        row[x] = (r < 128) ? 0x0000 : 0xFFFF;
      }
      tft.drawRGBBitmap(0, y, row, SCREEN_W, 8);
    }
  }
}

// =========================================================================
//  OSD (On-Screen Display) Ses & Mute Göstergesi
// =========================================================================
void drawVolumeOSD() {
  int boxW = 160;
  int boxH = 26;
  int boxX = (SCREEN_W - boxW) / 2;
  int boxY = 10;

  if (isMuted || currentVolume == 0) {
    tft.fillRoundRect(boxX, boxY, boxW, boxH, 6, ST77XX_RED);
    tft.drawRoundRect(boxX, boxY, boxW, boxH, 6, ST77XX_WHITE);
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(boxX + 22, boxY + 9);
    tft.print("SES KAPALI (MUTE)");
  } else {
    tft.fillRoundRect(boxX, boxY, boxW, boxH, 6, 0x10A2); // Koyu lacivert
    tft.drawRoundRect(boxX, boxY, boxW, boxH, 6, ST77XX_GREEN);
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_GREEN);
    tft.setCursor(boxX + 10, boxY + 9);
    tft.printf("SES: %%%d", currentVolume);

    // Ses İlerleme Çubuğu
    int barX = boxX + 75;
    int barY = boxY + 8;
    int barMaxW = 75;
    int barH = 10;
    tft.drawRect(barX, barY, barMaxW, barH, ST77XX_WHITE);
    int fillW = map(currentVolume, 0, 100, 0, barMaxW - 2);
    if (fillW > 0) {
      tft.fillRect(barX + 1, barY + 1, fillW, barH - 2, ST77XX_GREEN);
    }
  }
}

void triggerOSD() {
  osdUntil = millis() + 1800; // 1.8 saniye göster
}

void setVolume(uint8_t vol) {
  if (vol > 100) vol = 100;
  currentVolume = vol;
  if (currentVolume == 0) {
    isMuted = true;
  } else {
    isMuted = false;
    lastUnmutedVolume = currentVolume;
  }
  prefs.putUChar("vol", currentVolume);
  prefs.putBool("muted", isMuted);
  triggerOSD();
  Serial.printf("Ses Seviyesi: %d%% %s\n", currentVolume, isMuted ? "(MUTE)" : "");
}

void toggleMute() {
  if (isMuted || currentVolume == 0) {
    isMuted = false;
    currentVolume = (lastUnmutedVolume > 0) ? lastUnmutedVolume : DEFAULT_VOLUME;
  } else {
    lastUnmutedVolume = (currentVolume > 0) ? currentVolume : DEFAULT_VOLUME;
    isMuted = true;
    currentVolume = 0;
  }
  prefs.putUChar("vol", currentVolume);
  prefs.putBool("muted", isMuted);
  triggerOSD();
  Serial.printf("Mute Durumu: %s (Ses: %d%%)\n", isMuted ? "SES KAPALI (MUTE)" : "SES ACIK", currentVolume);
}

// =========================================================================
//  Bekleme Ekranı
// =========================================================================
void drawStandbyScreen() {
  Serial.println(">>> [STANDBY] drawStandbyScreen BASLADI <<<");
  tft.fillScreen(ST77XX_BLACK);
  tft.fillRect(0, 0, SCREEN_W, 4, ST77XX_RED);

  // EBU Renk Barları
  uint16_t colors[7] = {
    ST77XX_WHITE, ST77XX_YELLOW, ST77XX_CYAN,
    ST77XX_GREEN, ST77XX_MAGENTA, ST77XX_RED, ST77XX_BLUE
  };
  int barW = SCREEN_W / 7;
  for (int i = 0; i < 7; i++) {
    tft.fillRect(i * barW, 6, barW + 1, 55, colors[i]);
  }
  Serial.println(">>> [STANDBY] Renk barlari cizildi <<<");

  tft.fillRect(0, 64, SCREEN_W, 36, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(20, 68);
  tft.print("COLLECTIBLE");
  tft.setTextColor(ST77XX_RED);
  tft.setCursor(160, 68);
  tft.println("TV");

  tft.setTextSize(1);
  tft.setTextColor(ST77XX_GREEN);
  tft.setCursor(22, 90);
  if (isSDActive) {
    uint64_t sizeMB = SD.cardSize() / (1024 * 1024);
    tft.printf("[ 16GB SD KART: %llu MB ]\n", sizeMB);
  } else {
    tft.println("[ DAHILI FLASH AKTIF ]");
  }

  tft.drawFastHLine(10, 104, SCREEN_W - 20, ST77XX_RED);

  // Wi-Fi Bilgileri
  tft.setTextSize(1);
  tft.setTextColor(0xFD20); // Turuncu
  tft.setCursor(12, 112);
  tft.print("Hotspot:");
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(68, 112);
  tft.println(currentApSsid);

  tft.setTextColor(0xFD20);
  tft.setCursor(12, 126);
  tft.print("Sifre  :");
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(68, 126);
  tft.println(currentApPass);

  // Ev Wi-Fi Durumu
  if (WiFi.status() == WL_CONNECTED) {
    tft.setTextColor(0x07E0); // Yeşil
    tft.setCursor(12, 140);
    tft.print("Ev Agi :");
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(68, 140);
    tft.println(WiFi.SSID());

    tft.setTextColor(0x07E0);
    tft.setCursor(12, 154);
    tft.print("Ev IP  :");
    tft.setTextColor(ST77XX_CYAN);
    tft.setCursor(68, 154);
    tft.println(WiFi.localIP().toString());

    tft.setTextColor(0xFD20);
    tft.setCursor(12, 168);
    tft.print("Web URL:");
    tft.setTextColor(ST77XX_GREEN);
    tft.setCursor(68, 168);
    tft.println("collectibletv.local");
  } else {
    tft.setTextColor(ST77XX_YELLOW);
    tft.setCursor(12, 140);
    tft.print("Web IP :");
    tft.setTextColor(ST77XX_GREEN);
    tft.setCursor(68, 140);
    tft.println("192.168.4.1");

    tft.setTextColor(0x7BEF); // Gri
    tft.setCursor(12, 156);
    tft.println("Ev Wi-Fi: Bagli Degil");
    tft.setCursor(12, 168);
    tft.println("(Web panelinden baglayin)");
  }

  tft.drawFastHLine(10, 182, SCREEN_W - 20, ST77XX_RED);

  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(45, 195);
  tft.println("VIDEO BEKLENIYOR...");

  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(20, 215);
  tft.println("Web panelinden video yukleyin");

  tft.fillRect(0, SCREEN_H - 4, SCREEN_W, 4, ST77XX_RED);
  Serial.println(">>> [STANDBY] drawStandbyScreen TAMAMLANDI <<<");
}

// =========================================================================
//  Oynatma Listesini Güncelle
// =========================================================================
void refreshPlaylist() {
  playlist.clear();
  File root = storage->open(VIDEO_DIR);
  if (!root || !root.isDirectory()) {
    storage->mkdir(VIDEO_DIR);
    root = storage->open(VIDEO_DIR);
  }

  if (root) {
    File f = root.openNextFile();
    while (f) {
      if (!f.isDirectory()) {
        String name = String(f.name());
        if (name.endsWith(".mjpeg") || name.endsWith(".MJPEG")) {
          size_t sz = f.size();
          if (sz > 1024) {
            String pcmPath = String(VIDEO_DIR) + "/" + name.substring(0, name.lastIndexOf('.')) + ".pcm";
            bool hasAudio = storage->exists(pcmPath);
            MediaItem item;
            item.name = name;
            item.size = sz;
            item.hasAudio = hasAudio;
            playlist.push_back(item);
          } else {
            // 0 bayt veya bozuk dosyaları temizle
            String delPath = String(VIDEO_DIR) + "/" + name;
            storage->remove(delPath);
            Serial.printf("Bozuk/bos dosya silindi: %s\n", name.c_str());
          }
        }
      }
      f = root.openNextFile();
    }
  }

  // Kapasiteyi bir kez hafızaya al
  if (isSDActive) {
    cachedTotalBytes = SD.totalBytes();
    cachedUsedBytes = SD.usedBytes();
  } else {
    cachedTotalBytes = LittleFS.totalBytes();
    cachedUsedBytes = LittleFS.usedBytes();
  }

  Serial.printf("Oynatma listesi güncellendi: %d video bulundu. (Depolama: %s)\n", 
                playlist.size(), isSDActive ? "16GB MicroSD" : "LittleFS");
}

// =========================================================================
//  Seçilen Videoyu Başlat
// =========================================================================
void playVideoByIndex(int idx) {
  if (videoFile) videoFile.close();
#if USE_AUDIO
  if (audioFile) audioFile.close();
#endif
  resetFastReader();

  if (idx < 0 || idx >= (int)playlist.size()) {
    currentVideoIdx = -1;
    drawStandbyScreen();
    return;
  }

  currentVideoIdx = idx;
  String path = playlist[currentVideoIdx].name;
  String fullPath = path.startsWith("/") ? path : (String(VIDEO_DIR) + "/" + path);
  if (!storage->exists(fullPath)) {
    fullPath = "/" + path;
  }

  videoFile = storage->open(fullPath, "r");

  if (!videoFile || videoFile.size() == 0) {
    Serial.printf("HATA: %s açılamadı veya 0 bayt!\n", fullPath.c_str());
    if (videoFile) {
      videoFile.close();
      storage->remove(fullPath);
    }
    currentVideoIdx = -1;
    drawStandbyScreen();
  } else {
    Serial.printf("Oynatılıyor: %s (%u bayt) [25 FPS Akıcı Mod]\n", fullPath.c_str(), videoFile.size());
#if USE_AUDIO
    String pcmPath = fullPath.substring(0, fullPath.lastIndexOf('.')) + ".pcm";
    if (storage->exists(pcmPath)) {
      audioFile = storage->open(pcmPath, "r");
      Serial.printf("Eşleşen PCM Ses Dosyası Açıldı: %s (%u bayt)\n", pcmPath.c_str(), audioFile.size());
    } else {
      Serial.println("Bu videoya ait ses dosyası yok (Sessiz mod)");
    }
#endif
    drawCRTStatic(15);
  }
}

void nextChannel() {
  if (playlist.empty()) return;
  int next = (currentVideoIdx + 1) % playlist.size();
  playVideoByIndex(next);
}

// =========================================================================
//  Web Sunucu
// =========================================================================
void setupWebServer() {
  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", INDEX_HTML);
  });

  server.on("/ping", HTTP_GET, []() {
    server.send(200, "text/plain", "pong");
  });

  server.on("/volume", HTTP_GET, []() {
    if (server.hasArg("val")) {
      int val = server.arg("val").toInt();
      setVolume(val);
    }
    String json = "{\"volume\":" + String(currentVolume) + ",\"muted\":" + (isMuted ? "true" : "false") + "}";
    server.send(200, "application/json", json);
  });

  server.on("/mute", HTTP_GET, []() {
    toggleMute();
    String json = "{\"volume\":" + String(currentVolume) + ",\"muted\":" + (isMuted ? "true" : "false") + "}";
    server.send(200, "application/json", json);
  });

  server.on("/list", HTTP_GET, []() {
    bool staConn = (WiFi.status() == WL_CONNECTED);
    String staIp = staConn ? WiFi.localIP().toString() : "";

    String json = "{\"total\":" + String(cachedTotalBytes) + 
                  ",\"used\":" + String(cachedUsedBytes) + 
                  ",\"volume\":" + String(currentVolume) + 
                  ",\"muted\":" + (isMuted ? "true" : "false") + 
                  ",\"active\":\"" + (currentVideoIdx >= 0 && currentVideoIdx < (int)playlist.size() ? playlist[currentVideoIdx].name : "") + "\"" +
                  ",\"wifi\":{" +
                    "\"ap_ssid\":\"" + currentApSsid + "\"," +
                    "\"ap_pass\":\"" + currentApPass + "\"," +
                    "\"sta_ssid\":\"" + currentStaSsid + "\"," +
                    "\"sta_connected\":" + (staConn ? "true" : "false") + "," +
                    "\"sta_ip\":\"" + staIp + "\"," +
                    "\"ap_ip\":\"" + WiFi.softAPIP().toString() + "\"" +
                  "}," +
                  "\"files\":[";

    for (size_t i = 0; i < playlist.size(); i++) {
      if (i > 0) json += ",";
      json += "{\"name\":\"" + playlist[i].name + "\",\"size\":" + String(playlist[i].size) + ",\"hasAudio\":" + (playlist[i].hasAudio ? "true" : "false") + "}";
    }
    json += "]}";
    server.send(200, "application/json", json);
  });

  server.on("/play", HTTP_GET, []() {
    if (server.hasArg("file")) {
      String name = server.arg("file");
      for (size_t i = 0; i < playlist.size(); i++) {
        if (playlist[i].name == name) {
          server.send(200, "text/plain", "OK");
          playVideoByIndex(i);
          return;
        }
      }
    }
    server.send(400, "text/plain", "Video bulunamadı");
  });

  server.on("/delete", HTTP_GET, []() {
    if (server.hasArg("file")) {
      String name = server.arg("file");
      String path = String(VIDEO_DIR) + "/" + name;
      if (storage->exists(path)) {
        if (videoFile && currentVideoIdx >= 0 && playlist[currentVideoIdx].name == name) {
          videoFile.close();
        }
        storage->remove(path);

        // Varsa eşleşen .pcm ses dosyasını da sil
        String pcmPath = path.substring(0, path.lastIndexOf('.')) + ".pcm";
        if (storage->exists(pcmPath)) {
#if USE_AUDIO
          if (audioFile) audioFile.close();
#endif
          storage->remove(pcmPath);
        }

        refreshPlaylist();
        server.send(200, "text/plain", "Silindi");
        if (playlist.empty()) drawStandbyScreen();
        else playVideoByIndex(0);
        return;
      }
    }
    server.send(400, "text/plain", "Dosya bulunamadı");
  });

  // ---- WI-FI VE SİSTEM YÖNETİM ENDPOINT'LERİ ----
  server.on("/scan_wifi", HTTP_GET, []() {
    int n = WiFi.scanNetworks();
    String json = "[";
    for (int i = 0; i < n; i++) {
      if (i > 0) json += ",";
      json += "{\"ssid\":\"" + WiFi.SSID(i) + "\",\"rssi\":" + String(WiFi.RSSI(i)) + ",\"secure\":" + (WiFi.encryptionType(i) != WIFI_AUTH_OPEN ? "true" : "false") + "}";
    }
    json += "]";
    WiFi.scanDelete();
    server.send(200, "application/json", json);
  });

  server.on("/save_wifi", HTTP_POST, []() {
    bool changed = false;
    if (server.hasArg("ap_pass")) {
      String newAp = server.arg("ap_pass");
      if (newAp.length() >= 8) {
        prefs.putString("ap_pass", newAp);
        currentApPass = newAp;
        changed = true;
      }
    }
    if (server.hasArg("sta_ssid")) {
      String newSsid = server.arg("sta_ssid");
      String newPass = server.hasArg("sta_pass") ? server.arg("sta_pass") : "";
      prefs.putString("sta_ssid", newSsid);
      prefs.putString("sta_pass", newPass);
      currentStaSsid = newSsid;
      currentStaPass = newPass;
      changed = true;
    }
    server.send(200, "application/json", "{\"status\":\"ok\",\"message\":\"Ayarlar kaydedildi. Cihaz yeniden başlatılıyor...\"}");
    delay(1000);
    ESP.restart();
  });

  server.on("/forget_wifi", HTTP_POST, []() {
    prefs.putString("sta_ssid", "");
    prefs.putString("sta_pass", "");
    currentStaSsid = "";
    currentStaPass = "";
    WiFi.disconnect(true);
    server.send(200, "application/json", "{\"status\":\"ok\",\"message\":\"Ev Wi-Fi agi unutuldu. Cihaz yeniden başlatılıyor...\"}");
    delay(1000);
    ESP.restart();
  });

  server.on("/reset_factory", HTTP_POST, []() {
    prefs.clear();
    server.send(200, "application/json", "{\"status\":\"ok\",\"message\":\"Tüm ayarlar fabrika ayarlarına sıfırlandı. Cihaz yeniden başlatılıyor...\"}");
    delay(1000);
    ESP.restart();
  });

  // ---- KESİNTİSİZ PARÇALI (CHUNKED) YÜKLEME ENDPOINT'LERİ ----
  server.on("/upload_start", HTTP_POST, []() {
    if (!server.hasArg("name")) {
      server.send(400, "application/json", "{\"error\":\"name parametresi eksik\"}");
      return;
    }
    String name = server.arg("name");
    if (name.lastIndexOf('/') >= 0) name = name.substring(name.lastIndexOf('/') + 1);
    if (name.lastIndexOf('\\') >= 0) name = name.substring(name.lastIndexOf('\\') + 1);

    uploadChunkFilePath = String(VIDEO_DIR) + "/" + name;
    Serial.printf("Parçalı Yükleme Başlıyor: %s\n", uploadChunkFilePath.c_str());

    isUploading = true;
    if (videoFile) videoFile.close();
#if USE_AUDIO
    if (audioFile) audioFile.close();
#endif
    if (uploadChunkFile) uploadChunkFile.close();

    if (storage->exists(uploadChunkFilePath)) {
      storage->remove(uploadChunkFilePath);
    }
    uploadChunkFile = storage->open(uploadChunkFilePath, "w");
    if (uploadChunkFile) {
      uploadChunkFile.setBufferSize(8192); // 8 KB donanımsal yazma tamponu
      lastUploadChunkTime = millis();

      // Mini TV Ekranında Yükleme Görseli
      tft.fillScreen(ST77XX_BLACK);
      tft.fillRect(0, 0, SCREEN_W, 4, 0x001F); // Mavi
      tft.setTextSize(2);
      tft.setTextColor(0x07FF); // Cyan
      tft.setCursor(35, 60);
      tft.println("YUKLENIYOR");
      tft.setTextSize(1);
      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(20, 95);
      tft.println(name);
      tft.setTextColor(0xFD20); // Turuncu
      tft.setCursor(20, 118);
      tft.println("Wi-Fi Aktarimi...");
      tft.drawRect(20, 140, SCREEN_W - 40, 14, ST77XX_WHITE);

      server.send(200, "application/json", "{\"status\":\"ok\"}");
    } else {
      isUploading = false;
      server.send(500, "application/json", "{\"error\":\"Dosya açılamadı\"}");
    }
  });

  server.on("/upload_chunk", HTTP_POST, []() {
    lastUploadChunkTime = millis();
    if (server.hasArg("idx") && server.hasArg("total")) {
      int idx = server.arg("idx").toInt();
      int total = server.arg("total").toInt();
      if (total > 0 && idx % 4 == 0) {
        int w = ((SCREEN_W - 44) * (idx + 1)) / total;
        tft.fillRect(22, 142, w, 10, 0x07E0); // Yeşil dolgu
      }
    }
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  }, []() {
    HTTPUpload& upload = server.upload();
    lastUploadChunkTime = millis();
    if (upload.status == UPLOAD_FILE_WRITE) {
      if (uploadChunkFile && upload.currentSize > 0) {
        uploadChunkFile.write(upload.buf, upload.currentSize);
      }
    }
  });

  server.on("/upload_finish", HTTP_POST, []() {
    String name = server.hasArg("name") ? server.arg("name") : "";
    if (uploadChunkFile) {
      uploadChunkFile.flush();
      uploadChunkFile.close();
    }
    isUploading = false;
    Serial.printf("Parçalı Yükleme Tamamlandı: %s\n", uploadChunkFilePath.c_str());

    server.send(200, "application/json", "{\"status\":\"ok\"}");

    refreshPlaylist();
    if (!playlist.empty()) {
      if (name.length() > 0) {
        for (size_t i = 0; i < playlist.size(); i++) {
          if (playlist[i].name == name) {
            playVideoByIndex(i);
            return;
          }
        }
      }
      playVideoByIndex(playlist.size() - 1);
    }
  });

  server.on("/upload_abort", HTTP_POST, []() {
    if (uploadChunkFile) {
      uploadChunkFile.close();
    }
    if (uploadChunkFilePath.length() > 0 && storage->exists(uploadChunkFilePath)) {
      storage->remove(uploadChunkFilePath);
      Serial.printf("Yarıda kalan yükleme silindi: %s\n", uploadChunkFilePath.c_str());
    }
    isUploading = false;
    server.send(200, "application/json", "{\"status\":\"aborted\"}");
  });

  // Geleneksel Tek Parça Yükleme (Geriye Dönük Uyumluluk)
  static File uploadFile;
  server.on("/upload", HTTP_POST, []() {
    server.send(200, "text/plain", "Yükleme Başarılı");
    isUploading = false;
    refreshPlaylist();
    if (!playlist.empty()) {
      playVideoByIndex(playlist.size() - 1);
    }
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      isUploading = true;
      if (videoFile) videoFile.close();
#if USE_AUDIO
      if (audioFile) audioFile.close();
#endif

      String filename = upload.filename;
      if (filename.lastIndexOf('/') >= 0) filename = filename.substring(filename.lastIndexOf('/') + 1);
      if (filename.lastIndexOf('\\') >= 0) filename = filename.substring(filename.lastIndexOf('\\') + 1);

      String path = String(VIDEO_DIR) + "/" + filename;
      Serial.printf("Geleneksel Yükleme Başlıyor: %s\n", path.c_str());
      if (storage->exists(path)) storage->remove(path);
      uploadFile = storage->open(path, "w");
      if (uploadFile) uploadFile.setBufferSize(8192);
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (uploadFile) uploadFile.write(upload.buf, upload.currentSize);
    } else if (upload.status == UPLOAD_FILE_END) {
      if (uploadFile) {
        uploadFile.flush();
        uploadFile.close();
        Serial.printf("Yükleme Tamamlandı: %u bayt\n", upload.totalSize);
      }
    } else if (upload.status == UPLOAD_FILE_ABORTED) {
      if (uploadFile) uploadFile.close();
      isUploading = false;
      Serial.println("Yükleme iptal edildi veya koptu.");
    }
  });

  server.enableCORS(true);

  // Captive Portal ve Is-Online Yanıtları (Telefonların Wi-Fi'den atmasını kesin olarak engeller)
  server.on("/generate_204", HTTP_GET, []() {
    server.send(204, "text/plain", "");
  });
  server.on("/gen_204", HTTP_GET, []() {
    server.send(204, "text/plain", "");
  });
  server.on("/hotspot-detect.html", HTTP_GET, []() {
    server.send(200, "text/html", "<HTML><HEAD><TITLE>Success</TITLE></HEAD><BODY>Success</BODY></HTML>");
  });
  server.on("/ncsi.txt", HTTP_GET, []() {
    server.send(200, "text/plain", "Microsoft NCSI");
  });
  server.on("/connecttest.txt", HTTP_GET, []() {
    server.send(200, "text/plain", "Microsoft Connect Test");
  });
  server.onNotFound([]() {
    if (server.method() == HTTP_OPTIONS) {
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
      server.sendHeader("Access-Control-Allow-Headers", "*");
      server.send(204);
      return;
    }
    server.sendHeader("Location", "http://192.168.4.1/", true);
    server.send(302, "text/plain", "");
  });

  server.begin();
}

// =========================================================================
//  YÜKSEK HIZLI TAMPONLU KARE OKUYUCU
// =========================================================================
bool readNextMJPEGFrameFast(File &file, uint8_t *buf, size_t &frameSize, size_t maxLimit) {
  frameSize = 0;
  if (!file) return false;

  int c = 0;
  while (true) {
    c = readFastByte(file);
    if (c < 0) return false;
    if (c == 0xFF) {
      if (peekFastByte(file) == 0xD8) {
        readFastByte(file);
        buf[0] = 0xFF;
        buf[1] = 0xD8;
        frameSize = 2;
        break;
      }
    }
  }

  while (true) {
    if (frameSize >= maxLimit - 2) return false;
    c = readFastByte(file);
    if (c < 0) return false;
    buf[frameSize++] = (uint8_t)c;
    if (c == 0xFF) {
      if (peekFastByte(file) == 0xD9) {
        buf[frameSize++] = (uint8_t)readFastByte(file);
        return true;
      }
    }
  }
}

// =========================================================================
//  SETUP
// =========================================================================
void setup() {
  esp_brownout_disable(); // Pilde voltaj çöküşü resetlerini ve ekran titremesini engeller
  Serial.begin(115200);
  Serial.setTxTimeoutMs(0); // USB bağlı değilken Serial.print kilitlenmesini engeller
  delay(100);
  Serial.println("\n=============================================");
  Serial.println("   🎬 COLLECTIBLE DISPLAY - 16GB SD EDITION");
  Serial.println("=============================================");

  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  // Donanımsal Reset
  pinMode(TFT_RES, OUTPUT);
  digitalWrite(TFT_RES, HIGH);
  delay(50);
  digitalWrite(TFT_RES, LOW);
  delay(100);
  digitalWrite(TFT_RES, HIGH);
  delay(200);

  // 1. Ekranı Başlat (FSPI: SCK=4, MOSI=6)
  Serial.println(">>> [EKRAN] SPI ve ST7789 baslatiliyor... <<<");
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, -1);
  tft.init(SCREEN_W, SCREEN_H, SPI_MODE3);
  tft.setSPISpeed(SPI_SPEED);
  tft.setRotation(LCD_ROTATION);
  tft.fillScreen(ST77XX_RED);
  delay(150);
  tft.fillScreen(ST77XX_BLUE);
  delay(150);
  tft.fillScreen(ST77XX_BLACK);
  Serial.println(">>> [EKRAN] ST7789 baslatildi ve renk testi yapildi! <<<");

  jpeg.setPixelType(RGB565_LITTLE_ENDIAN);

  frameBuffer = (uint8_t *)malloc(frameBufferSize);

  // Hafızadan son ses, mute ve Wi-Fi ayarlarını yükle
  prefs.begin("retro_tv", false);
  currentVolume = prefs.getUChar("vol", DEFAULT_VOLUME);
  isMuted = prefs.getBool("muted", false);
  lastUnmutedVolume = (currentVolume > 0) ? currentVolume : DEFAULT_VOLUME;

  currentApSsid = prefs.getString("ap_ssid", AP_SSID);
  currentApPass = prefs.getString("ap_pass", AP_PASS);
  if (currentApPass.length() < 8) currentApPass = AP_PASS;
  currentStaSsid = prefs.getString("sta_ssid", "");
  currentStaPass = prefs.getString("sta_pass", "");

#if USE_AUDIO
  // 2. I2S Ses Başlat (MAX98357A: BCLK=7, LRC=8, DIN=9)
  i2s.setPins(PIN_I2S_BCLK, PIN_I2S_LRC, PIN_I2S_DIN);
  if (i2s.begin(I2S_MODE_STD, AUDIO_RATE, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO)) {
    audioReady = true;
    Serial.printf(">>> I2S SES AMFISI AKTIF (MAX98357A) - Ses: %%%d %s <<<\n", 
                  currentVolume, isMuted ? "(MUTE)" : "");
  }
#endif

  // 3. SD Kartı Başlat (HSPI: SCK=12, MISO=13, MOSI=11, CS=10)
  Serial.println("SD Kart baslatiliyor (HSPI: 10, 11, 12, 13)...");
  sdSPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  delay(100);

  bool sdOk = false;
  for (int r = 0; r < 3; r++) {
    if (SD.begin(SD_CS, sdSPI, 20000000) || SD.begin(SD_CS, sdSPI, 4000000) || SD.begin(SD_CS, sdSPI, 1000000)) {
      sdOk = true;
      break;
    }
    delay(150);
  }

  if (sdOk) {
    uint64_t sizeMB = SD.cardSize() / (1024 * 1024);
    Serial.printf(">>> 16GB SD KART BASARIYLA BAGLANDI! (%llu MB) <<<\n", sizeMB);
    storage = &SD;
    isSDActive = true;
    if (!storage->exists(VIDEO_DIR)) {
      storage->mkdir(VIDEO_DIR);
    }
  } else {
    Serial.println("SD Kart bulunamadi, Dahili Flash kullaniliyor.");
    LittleFS.begin(true);
    storage = &LittleFS;
    isSDActive = false;
  }

  // 4. Wi-Fi Başlat (Maksimum Donanımsal Güç ve Güvenilir Erişim Noktası)
  if (currentStaSsid.length() > 0) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(currentApSsid.c_str(), currentApPass.c_str(), 6, 0, 8);
    Serial.printf("Ev Wi-Fi agina baglaniliyor: %s ...\n", currentStaSsid.c_str());
    WiFi.begin(currentStaSsid.c_str(), currentStaPass.c_str());
    unsigned long startConn = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startConn < 3000) {
      delay(100);
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.printf(">>> EV WI-FI AGINA BAGLANDI! IP: %s <<<\n", WiFi.localIP().toString().c_str());
    } else {
      Serial.println("Ev Wi-Fi baglantisi arka planda denenmeye devam edecek.");
    }
  } else {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(currentApSsid.c_str(), currentApPass.c_str(), 6, 0, 8);
  }

  WiFi.setSleep(false);                 // Modem uykusunu kapat (sinyal gucunu ve hizi tavan yapar)
  WiFi.setTxPower(WIFI_POWER_17dBm);    // Kararli donanimsal verici gucu (voltaj cokusunu onler)
  Serial.print("Wi-Fi Hotspot: ");
  Serial.print(currentApSsid);
  Serial.print(" - IP: ");
  Serial.println(WiFi.softAPIP());

  // DNS Server (Captive Portal - Telefonda otomatik giriş sayfasını tetikler)
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  Serial.println("DNS Sunucu (Captive Portal) aktif edildi.");

  // mDNS Başlat (http://collectibletv.local)
  if (MDNS.begin("collectibletv")) {
    MDNS.addService("http", "tcp", 80);
    Serial.println("mDNS Aktif: http://collectibletv.local");
  }

  setupWebServer();
  refreshPlaylist();

  if (!playlist.empty()) {
    playVideoByIndex(0);
  } else {
    drawStandbyScreen();
  }
}

// =========================================================================
//  BUTON KONTROLÜ (Tek Tık: Kanal, Çift Tık: Mute, Uzun Bas: Ses Kademesi)
// =========================================================================
void handleButton() {
  int btnState = digitalRead(PIN_BUTTON);
  unsigned long now = millis();

  if (btnState == LOW) { // Butona basılı
    if (!buttonPressed) {
      buttonPressed = true;
      buttonDownTime = now;
      longPressHandled = false;
    } else {
      // Uzun basış (> 1200 ms): Ses kademesi döngüsü
      if (!longPressHandled && (now - buttonDownTime >= 1200)) {
        longPressHandled = true;
        buttonClicks = 0;
        if (isMuted || currentVolume == 0) setVolume(25);
        else if (currentVolume < 50) setVolume(50);
        else if (currentVolume < 75) setVolume(75);
        else if (currentVolume < 100) setVolume(100);
        else setVolume(0); // Sessiz / Mute
      }
    }
  } else { // Buton bırakıldı
    if (buttonPressed) {
      buttonPressed = false;
      unsigned long duration = now - buttonDownTime;
      if (!longPressHandled && duration > 40) { // Geçerli basış
        buttonClicks++;
        lastReleaseTime = now;
      }
    }
  }

  // Tıklama zaman aşımı (350 ms içinde ikinci tık gelmezse karar ver)
  if (buttonClicks > 0 && (now - lastReleaseTime > 350)) {
    if (buttonClicks == 1) {
      // TEK TIK: Sonraki Kanal
      nextChannel();
    } else if (buttonClicks >= 2) {
      // ÇİFT TIK: Sesi Kapat / Aç (MUTE TOGGLE)
      toggleMute();
    }
    buttonClicks = 0;
  }
}

// =========================================================================
//  SERIAL KOMUT YÖNETİMİ (USB üzerinden Wi-Fi ve Hızlı Ayarlar)
// =========================================================================
void handleSerialCommands() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd.startsWith("WIFI:")) {
      int firstColon = cmd.indexOf(':');
      int secondColon = cmd.indexOf(':', firstColon + 1);
      if (secondColon > firstColon) {
        String newSsid = cmd.substring(firstColon + 1, secondColon);
        String newPass = cmd.substring(secondColon + 1);
        Serial.printf("Serial uzerinden yeni Wi-Fi alindi: %s\n", newSsid.c_str());
        prefs.putString("sta_ssid", newSsid);
        prefs.putString("sta_pass", newPass);
        Serial.println("Kaydedildi. Cihaz yeniden baslatiliyor...");
        delay(800);
        ESP.restart();
      }
    } else if (cmd == "RESET") {
      prefs.clear();
      Serial.println("Fabrika ayarlarina sifirlandi. Yeniden baslatiliyor...");
      delay(800);
      ESP.restart();
    }
  }
}

// =========================================================================
//  LOOP
// =========================================================================
void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  handleButton();
  handleSerialCommands();

  // Yükleme Zaman Aşımı Emniyeti (30 sn sessizlik olursa yarıda kalan yüklemeyi temizle)
  if (isUploading && uploadChunkFile && (millis() - lastUploadChunkTime > 30000)) {
    Serial.println("Yükleme zaman aşımına uğradı! Yarıda kalan dosya temizleniyor...");
    uploadChunkFile.close();
    if (uploadChunkFilePath.length() > 0) {
      storage->remove(uploadChunkFilePath);
    }
    isUploading = false;
  }

  if (isUploading) {
    delay(2);
    return;
  }

  if (!videoFile || currentVideoIdx < 0) {
    delay(2);
    return;
  }

  unsigned long now = millis();
  if (now - lastFrameTime < frameDelayMs) {
    delay(2);
    return;
  }
  lastFrameTime = now;

    // 1. Ses Çalma (16 kHz Mono PCM Senkronizasyonu)
#if USE_AUDIO
    if (audioReady && audioFile && audioFile.available()) {
      int16_t audioBuf[640]; // 40 ms @ 16 kHz = 640 örnek
      int nBytes = audioFile.read((uint8_t *)audioBuf, sizeof(audioBuf));
      if (nBytes > 0) {
        int samples = nBytes / 2;
        int32_t gain = (isMuted || currentVolume == 0) ? 0 : ((int32_t)currentVolume * 256 / 100);
        for (int i = 0; i < samples; i++) {
          audioBuf[i] = (int16_t)(((int32_t)audioBuf[i] * gain) >> 8);
        }
        i2s.write((uint8_t *)audioBuf, nBytes);
      }
    } else if (audioFile) {
      audioFile.seek(0); // Ses bittiğinde video ile birlikte başa sar
    }
#endif

    // 2. Video Kare Çizimi
    size_t frameSize = 0;
    if (readNextMJPEGFrameFast(videoFile, frameBuffer, frameSize, frameBufferSize)) {
      if (jpeg.openRAM(frameBuffer, frameSize, JPEGDraw)) {
        jpeg.decode(0, 0, 0);
        jpeg.close();

        // OSD Gösterimi (Ses değiştirildiğinde veya Mute yapıldığında)
        if (millis() < osdUntil) {
          drawVolumeOSD();
        }
      }
    } else {
      videoFile.seek(0);
      resetFastReader();
      if (videoFile.size() < 100000) { // Küçük dosya veya fotoğraf ise döngüyü yavaşlatıp Wi-Fi ve CPU'yu rahatlat
        delay(60);
      }
#if USE_AUDIO
      if (audioFile) audioFile.seek(0);
#endif
    }

    // Her kare sonrasında Wi-Fi ve Web Sunucusuna nefes aldır
    delay(10);
}
