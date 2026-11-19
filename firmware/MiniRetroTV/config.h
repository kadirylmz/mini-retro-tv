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

// ---- MAX98357A I2S Ses Pinleri & Ayarları ----
#define USE_AUDIO       1       // 1: I2S Ses Aktif, 0: Devre Dışı
#define PIN_I2S_BCLK    7       // I2S Bit Clock (BCLK)
#define PIN_I2S_LRC     8       // I2S Word Select (LRC / WS)
#define PIN_I2S_DIN     9       // I2S Data Out (DIN)
#define AUDIO_RATE      16000   // 16 kHz Mono 16-bit PCM
#define DEFAULT_VOLUME  70      // Başlangıç Ses Seviyesi (%70)

// ---- Buton Pini ----
#define PIN_BUTTON  0   // Tek tık: Sonraki video | Çift tık: Sesi Kapat/Aç (Mute) | Uzun bas: Ses Seviyesi

// ---- Ekran & SPI Performans Ayarları ----
#define SCREEN_W    240
#define SCREEN_H    240
#define LCD_ROTATION 1  // Yatay kullanım
#define SPI_SPEED   24000000  // 24 MHz Kararlı Donanımsal SPI (Jumper kablolar için güvenilir sinyal)

// ---- Wi-Fi Ayarları ----
#define AP_SSID     "CollectibleDisplay"
#define AP_PASS     "display1234"
#define STA_SSID    ""
#define STA_PASS    ""

// ---- Video Oynatıcı Ayarları ----
#define VIDEO_DIR       "/v"
#define DEFAULT_FPS     20   // 20 FPS Akıcı ve Dengeli Kare Hızı (Wi-Fi ve Web Sunucusuna tam öncelik sağlar)
#define MAX_FRAME_SIZE  (64 * 1024)
