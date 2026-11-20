#pragma once
// =============================================================
//  Collectible Display / Mini Retro TV - Donanım & Pin Ayarları
//  Ana Kart: TLS Robotik ESP32-C3 SuperMini (Lehimsiz, 16-Pin)
//  İşlemci : ESP32-C3 32-bit RISC-V @ 160 MHz
// =============================================================

// ---- EKRAN MODELİ SEÇİMİ ----
// Masadaki mevcut test ekranı 1.3" (240x240, 7-pin ST7789) ise: 1
// Yeni Voltaj 1.69" (240x280, 8-pin ST7789 Oval) ekran takıldığında: 0
#define IS_13_INCH_DISPLAY  1

#if IS_13_INCH_DISPLAY
  // 1.3" IPS 240x240 ST7789 (7-Pin, CS pini yok / Dahili GND)
  #define TFT_CS        -1
  #define SCREEN_W      240
  #define SCREEN_H      240
  #define LCD_ROTATION  1
#else
  // Voltaj 1.69" IPS 240x280 ST7789 (8-Pin)
  #define TFT_CS        7
  #define SCREEN_W      240
  #define SCREEN_H      280
  #define LCD_ROTATION  1
#endif

// ---- Ekran Pinleri (Donanımsal SPI: FSPI) ----
#define TFT_SCLK    4   // Ekran SCL (Turuncu) + MicroSD SCK (Paylaşımlı SPI)
#define TFT_MOSI    6   // Ekran SDA (Mavi)    + MicroSD MOSI (Paylaşımlı SPI)
#define TFT_RES     2   // Ekran RES (Yeşil)
#define TFT_DC      3   // Ekran DC  (Mor)
#define TFT_BL      1   // Ekran BLK (Sarı - PWM Parlaklık Kontrolü)
#define SPI_SPEED   32000000  // 32 MHz Kararlı Donanımsal SPI

// ---- MicroSD Kart Pinleri (Paylaşımlı Donanımsal SPI) ----
#define SD_SCK      4   // Ekran SCL ile ortak (Paylaşımlı SPI)
#define SD_MOSI     6   // Ekran SDA ile ortak (Paylaşımlı SPI)
#define SD_MISO     5   // MicroSD MISO (Yeşil)
#define SD_CS       20  // MicroSD CS (ESP32-C3 SuperMini üzerindeki RX pini)

// ---- MAX98357A I2S Ses Pinleri & Ayarları ----
#define USE_AUDIO          1       // 1: I2S Ses Aktif, 0: Devre Dışı
#define ENABLE_VOLUME_OSD  1       // 1: Dikişsiz flicker-free OSD açık, 0: Kapalı
#define PIN_I2S_BCLK       0       // I2S Bit Clock (Sol Sıra GPIO 0)
#define PIN_I2S_LRC        10      // I2S Word Select / WS (Sağ Sıra GPIO 10)
#define PIN_I2S_DIN        21      // I2S Data Out (ESP32-C3 SuperMini üzerindeki TX pini)
#define AUDIO_RATE         16000   // 16 kHz Mono 16-bit PCM
#define DEFAULT_VOLUME     65      // Pilde tasarruf ve temiz ses için %65 (Amfi akımını optimize eder)

// ---- Buton Pini (Dahili & Harici) ----
// ESP32-C3 SuperMini üzerindeki dahili BOOT butonu GPIO 9'dur (B tuşu).
// Kasa tavanındaki harici tactile buton da GPIO 9 ile GND arasına bağlanır.
#define PIN_BUTTON  9   // Tek tık: Sonraki video | Çift tık: Mute | Uzun bas: Ses seviyesi

// ---- Wi-Fi Ayarları ----
#define AP_SSID     "RetroTV-Display"
#define AP_PASS     ""   // Boş bırakıldığında ŞİFRESİZ (Açık Ağ / Kolay Bağlantı)
#define STA_SSID    ""
#define STA_PASS    ""

// ---- Video Oynatıcı Ayarları ----
#define VIDEO_DIR       "/v"
#define DEFAULT_FPS     20   // 20 FPS Akıcı ve Dengeli Kare Hızı
#define MAX_FRAME_SIZE  (48 * 1024)
