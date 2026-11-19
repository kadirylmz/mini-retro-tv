#pragma once
// =============================================================
//  Collectible Display - 1.3" ST7789 + ESP32-S3 + 16GB MicroSD
// =============================================================

// ---- Ekran Pinleri (FSPI) ----
#define TFT_SCLK    4   // Ekran SCL (Turuncu)
#define TFT_MOSI    6   // Ekran SDA (Mavi)
#define TFT_RES     2   // Ekran RES (Yeşil)
#define TFT_DC      3   // Ekran DC  (Mor)
#define TFT_BL      1   // Ekran BLK (Sarı)
#define TFT_CS     -1   // CS pini yok (Dahili GND)

// ---- SD Kart Pinleri (HSPI - Doğrulanan ve Çalışan Pinler) ----
#define SD_SCK     12   // Sağ sıra 12
#define SD_MOSI    11   // Sağ sıra 11
#define SD_MISO    13   // Sağ sıra 13
#define SD_CS      10   // Sağ sıra 10

// ---- Buton Pini ----
#define PIN_BUTTON  0   // Kısa basış: Sonraki video/kanal

// ---- Ekran & SPI Performans Ayarları ----
#define SCREEN_W    240
#define SCREEN_H    240
#define LCD_ROTATION 1  // Yatay kullanım
#define SPI_SPEED   30000000  // 30 MHz Donanımsal SPI (Akıcı 25 FPS)

// ---- Wi-Fi Ayarları ----
#define AP_SSID     "CollectibleDisplay"
#define AP_PASS     "display1234"
#define STA_SSID    ""
#define STA_PASS    ""

// ---- Video Oynatıcı Ayarları ----
#define VIDEO_DIR       "/v"
#define DEFAULT_FPS     25   // 25 FPS Akıcı Sinematik Kare Hızı
#define MAX_FRAME_SIZE  (64 * 1024)
