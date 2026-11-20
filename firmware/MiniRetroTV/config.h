#pragma once
// =============================================================
//  Collectible Display - Ekran & Donanım Seçimi
// =============================================================

// ---- EKRAN MODELİ SEÇİMİ ----
// Masadaki mevcut test ekranı 1.3" (240x240) ise: 1
// Yeni Voltaj 1.69" (240x280 Oval) ekran takıldığında: 0
#define IS_13_INCH_DISPLAY  1

#if IS_13_INCH_DISPLAY
  // 1.3" IPS 240x240 ST7789 (7-Pin, CS pini yok / Dahili GND)
  #define TFT_CS        -1
  #define SCREEN_W      240
  #define SCREEN_H      240
  #define LCD_ROTATION  1
#else
  // Voltaj 1.69" IPS 240x280 ST7789 (8-Pin)
  #define TFT_CS        5
  #define SCREEN_W      240
  #define SCREEN_H      280
  #define LCD_ROTATION  1
#endif

// ---- Ekran Pinleri (FSPI) ----
#define TFT_SCLK    4   // Ekran SCL (Turuncu)
#define TFT_MOSI    6   // Ekran SDA (Mavi)
#define TFT_RES     2   // Ekran RES (Yeşil)
#define TFT_DC      3   // Ekran DC  (Mor)
#define TFT_BL      1   // Ekran BLK (Sarı)
#define SPI_SPEED   24000000  // 24 MHz Kararlı Donanımsal SPI

// ---- SD Kart Pinleri (HSPI - Doğrulanan ve Çalışan Pinler) ----
#define SD_SCK     12   // Sağ sıra 12
#define SD_MOSI    11   // Sağ sıra 11
#define SD_MISO    13   // Sağ sıra 13
#define SD_CS      10   // Sağ sıra 10

// ---- MAX98357A I2S Ses Pinleri & Ayarları ----
#define USE_AUDIO          1       // 1: I2S Ses Aktif, 0: Devre Dışı
#define ENABLE_VOLUME_OSD  0       // 0: Temiz retro ekran (videonun önüne yazı/kutu çıkmaz, pırpır yapmaz), 1: OSD göster
#define PIN_I2S_BCLK       7       // I2S Bit Clock (BCLK)
#define PIN_I2S_LRC        8       // I2S Word Select (LRC / WS)
#define PIN_I2S_DIN        9       // I2S Data Out (DIN)
#define AUDIO_RATE         16000   // 16 kHz Mono 16-bit PCM
#define DEFAULT_VOLUME     45      // Pilde tasarruf ve temiz ses için %45 (Amfi akımını yarıya indirir)

// ---- Buton Pini ----
#define PIN_BUTTON  0   // Tek tık: Sonraki video | Çift tık: Sesi Kapat/Aç (Mute) | Uzun bas: Ses Seviyesi

// ---- Wi-Fi Ayarları ----
#define AP_SSID     "CollectibleDisplay"
#define AP_PASS     ""   // Boş bırakıldığında ŞİFRESİZ (Açık Ağ / Kolay Bağlantı)
#define STA_SSID    ""
#define STA_PASS    ""

// ---- Video Oynatıcı Ayarları ----
#define VIDEO_DIR       "/v"
#define DEFAULT_FPS     20   // 20 FPS Akıcı ve Dengeli Kare Hızı (Wi-Fi ve Web Sunucusuna tam öncelik sağlar)
#define MAX_FRAME_SIZE  (64 * 1024)
