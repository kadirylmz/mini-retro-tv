// =========================================================================
//  Collectible Display - 1.3" IPS ST7789 + ESP32-S3 Mini Firmware
//  Çift SPI (FSPI Ekran + HSPI 16GB MicroSD) + 25 FPS Akıcı Video Motoru
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
#include <vector>

#include "config.h"
#include "web_page.h"

// Donanım Nesneleri (Çift Donanımsal SPI)
Adafruit_ST7789 tft(&SPI, TFT_CS, TFT_DC, TFT_RES);
SPIClass sdSPI(HSPI);
JPEGDEC jpeg;
WebServer server(80);

// Depolama Aygıtı (SD Kart veya Dahili Flash Yedek)
fs::FS *storage = &SD;
bool isSDActive = false;

// Video Yönetimi
std::vector<String> playlist;
int currentVideoIdx = -1;
File videoFile;
bool isUploading = false;

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

// Zamanlama ve Buton
unsigned long lastFrameTime = 0;
unsigned long frameDelayMs = 1000 / DEFAULT_FPS;
unsigned long lastButtonPress = 0;

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
//  Bekleme Ekranı (Yatay Tasarım)
// =========================================================================
void drawStandbyScreen() {
  tft.fillScreen(ST77XX_BLACK);
  tft.fillRect(0, 0, SCREEN_W, 4, ST77XX_RED);

  // EBU Renk Barları
  uint16_t colors[7] = {
    ST77XX_WHITE, ST77XX_YELLOW, ST77XX_CYAN,
    ST77XX_GREEN, ST77XX_MAGENTA, ST77XX_RED, ST77XX_BLUE
  };
  int barW = SCREEN_W / 7;
  for (int i = 0; i < 7; i++) {
    tft.fillRect(i * barW, 6, barW + 1, 60, colors[i]);
  }

  tft.fillRect(0, 70, SCREEN_W, 42, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(20, 75);
  tft.print("COLLECTIBLE");
  tft.setTextColor(ST77XX_RED);
  tft.setCursor(160, 75);
  tft.println("TV");

  tft.setTextSize(1);
  tft.setTextColor(ST77XX_GREEN);
  tft.setCursor(22, 98);
  if (isSDActive) {
    uint64_t sizeMB = SD.cardSize() / (1024 * 1024);
    tft.printf("[ 16GB SD KART AKTIF: %llu MB ]\n", sizeMB);
  } else {
    tft.println("[ DAHILI FLASH AKTIF ]");
  }

  tft.drawFastHLine(10, 118, SCREEN_W - 20, ST77XX_RED);

  tft.setTextSize(1);
  tft.setTextColor(0xFD20);
  tft.setCursor(15, 128);
  tft.print("Wi-Fi :");
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(65, 128);
  tft.println(AP_SSID);

  tft.setTextColor(0xFD20);
  tft.setCursor(15, 145);
  tft.print("Sifre :");
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(65, 145);
  tft.println(AP_PASS);

  tft.setTextColor(0xFD20);
  tft.setCursor(15, 162);
  tft.print("Web   :");
  tft.setTextColor(ST77XX_GREEN);
  tft.setCursor(65, 162);
  tft.println("192.168.4.1");

  tft.drawFastHLine(10, 182, SCREEN_W - 20, ST77XX_RED);

  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(45, 195);
  tft.println("VIDEO BEKLENIYOR...");

  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(25, 215);
  tft.println("Web panelinden video yukleyin");

  tft.fillRect(0, SCREEN_H - 4, SCREEN_W, 4, ST77XX_RED);
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
          playlist.push_back(name);
        }
      }
      f = root.openNextFile();
    }
  }

  // Kök dizinde de video varsa listeye al
  if (playlist.empty()) {
    File rootRoot = storage->open("/");
    if (rootRoot) {
      File f = rootRoot.openNextFile();
      while (f) {
        if (!f.isDirectory()) {
          String name = String(f.name());
          if (name.endsWith(".mjpeg") || name.endsWith(".MJPEG")) {
            playlist.push_back(name);
          }
        }
        f = rootRoot.openNextFile();
      }
    }
  }

  Serial.printf("Oynatma listesi güncellendi: %d video bulundu. (Depolama: %s)\n", 
                playlist.size(), isSDActive ? "16GB MicroSD" : "LittleFS");
}

// =========================================================================
//  Seçilen Videoyu Başlat
// =========================================================================
void playVideoByIndex(int idx) {
  if (videoFile) {
    videoFile.close();
  }
  resetFastReader();

  if (idx < 0 || idx >= (int)playlist.size()) {
    currentVideoIdx = -1;
    drawStandbyScreen();
    return;
  }

  currentVideoIdx = idx;
  String path = playlist[currentVideoIdx];
  String fullPath = path.startsWith("/") ? path : (String(VIDEO_DIR) + "/" + path);
  if (!storage->exists(fullPath)) {
    fullPath = "/" + path;
  }

  videoFile = storage->open(fullPath, "r");

  if (!videoFile) {
    Serial.printf("HATA: %s açılamadı!\n", fullPath.c_str());
    currentVideoIdx = -1;
    drawStandbyScreen();
  } else {
    Serial.printf("Oynatılıyor: %s (%u bayt) [25 FPS Akıcı Mod]\n", fullPath.c_str(), videoFile.size());
    drawCRTStatic(100);
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

  server.on("/list", HTTP_GET, []() {
    size_t total = isSDActive ? (SD.totalBytes()) : LittleFS.totalBytes();
    size_t used = isSDActive ? (SD.usedBytes()) : LittleFS.usedBytes();

    String json = "{\"total\":" + String(total) + 
                  ",\"used\":" + String(used) + 
                  ",\"active\":\"" + (currentVideoIdx >= 0 && currentVideoIdx < (int)playlist.size() ? playlist[currentVideoIdx] : "") + "\"" +
                  ",\"files\":[";

    bool first = true;
    for (const auto &name : playlist) {
      String p = name.startsWith("/") ? name : (String(VIDEO_DIR) + "/" + name);
      File f = storage->open(p, "r");
      size_t s = f ? f.size() : 0;
      if (f) f.close();

      if (!first) json += ",";
      first = false;
      json += "{\"name\":\"" + name + "\",\"size\":" + String(s) + "}";
    }
    json += "]}";
    server.send(200, "application/json", json);
  });

  server.on("/play", HTTP_GET, []() {
    if (server.hasArg("file")) {
      String name = server.arg("file");
      for (size_t i = 0; i < playlist.size(); i++) {
        if (playlist[i] == name) {
          playVideoByIndex(i);
          server.send(200, "text/plain", "OK");
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
        if (videoFile && currentVideoIdx >= 0 && playlist[currentVideoIdx] == name) {
          videoFile.close();
        }
        storage->remove(path);
        refreshPlaylist();
        if (playlist.empty()) drawStandbyScreen();
        else playVideoByIndex(0);
        server.send(200, "text/plain", "Silindi");
        return;
      }
    }
    server.send(400, "text/plain", "Dosya bulunamadı");
  });

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

      String filename = upload.filename;
      if (filename.lastIndexOf('/') >= 0) filename = filename.substring(filename.lastIndexOf('/') + 1);
      if (filename.lastIndexOf('\\') >= 0) filename = filename.substring(filename.lastIndexOf('\\') + 1);

      String path = String(VIDEO_DIR) + "/" + filename;
      Serial.printf("Yükleme Başlıyor: %s\n", path.c_str());
      if (storage->exists(path)) storage->remove(path);
      uploadFile = storage->open(path, "w");
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (uploadFile) uploadFile.write(upload.buf, upload.currentSize);
    } else if (upload.status == UPLOAD_FILE_END) {
      if (uploadFile) {
        uploadFile.close();
        Serial.printf("Yükleme Tamamlandı: %u bayt\n", upload.totalSize);
      }
    }
  });

  server.begin();
}

// =========================================================================
//  YÜKSEK HIZLI TAMPONLU KARE OKUYUCU (Chunk-Buffered Streaming)
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
  Serial.begin(115200);
  delay(1000);
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
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, -1);
  tft.init(SCREEN_W, SCREEN_H, SPI_MODE3);
  tft.setSPISpeed(SPI_SPEED);
  tft.setRotation(LCD_ROTATION);
  tft.fillScreen(ST77XX_BLACK);

  jpeg.setPixelType(RGB565_LITTLE_ENDIAN);

  frameBuffer = (uint8_t *)malloc(frameBufferSize);

  // 2. SD Kartı Başlat (HSPI: SCK=12, MISO=13, MOSI=11, CS=10)
  Serial.println("SD Kart baslatiliyor (HSPI: 10, 11, 12, 13)...");
  sdSPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

  if (SD.begin(SD_CS, sdSPI, 20000000) || SD.begin(SD_CS, sdSPI, 4000000)) {
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

  // 3. Wi-Fi AP Başlat
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.print("Wi-Fi AP: ");
  Serial.print(AP_SSID);
  Serial.print(" - IP: ");
  Serial.println(WiFi.softAPIP());

  setupWebServer();
  refreshPlaylist();

  if (!playlist.empty()) {
    playVideoByIndex(0);
  } else {
    drawStandbyScreen();
  }
}

// =========================================================================
//  LOOP
// =========================================================================
void loop() {
  server.handleClient();

  if (digitalRead(PIN_BUTTON) == LOW && (millis() - lastButtonPress > 400)) {
    lastButtonPress = millis();
    nextChannel();
  }

  if (isUploading) {
    delay(10);
    return;
  }

  if (!videoFile || currentVideoIdx < 0) {
    delay(20);
    return;
  }

  unsigned long now = millis();
  if (now - lastFrameTime >= frameDelayMs) {
    lastFrameTime = now;

    size_t frameSize = 0;
    if (readNextMJPEGFrameFast(videoFile, frameBuffer, frameSize, frameBufferSize)) {
      if (jpeg.openRAM(frameBuffer, frameSize, JPEGDraw)) {
        jpeg.decode(0, 0, 0);
        jpeg.close();
      }
    } else {
      videoFile.seek(0);
      resetFastReader();
    }
  }
}
