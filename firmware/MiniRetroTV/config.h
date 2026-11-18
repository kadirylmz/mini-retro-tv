#pragma once
// =============================================================
//  MiniRetroTV - Ayarlar (Sesli + MicroSD + Wi-Fi Sürümü)
//  Donanım:
//    - TLS Robotik ESP32-C3 SuperMini (Lehimsiz)
//    - Voltaj 1.69" 240x280 SPI IPS TFT Ekran (ST7789, 8-pin)
//    - MAX98357A I2S 3W Dijital Amfi + 8 Ohm 1W Mini Hoparlör
//    - SPI MicroSD Kart Modülü (takılı değilse dahili hafızayı kullanır)
//    - TP4056 Type-C Korumalı Şarj Modülü + 3.7V 600mAh LiPo (603035)
// =============================================================

// ---- Ekran pinleri (Paylaşımlı SPI: SCK=4, MOSI=6) ----
// Ekran VCC -> 3V3 | Ekran GND -> GND
#define PIN_LCD_SCK   4   // Ekran SCL  + MicroSD SCK
#define PIN_LCD_MOSI  6   // Ekran SDA  + MicroSD MOSI
#define PIN_LCD_CS    7   // Ekran CS
#define PIN_LCD_DC    3   // Ekran DC
#define PIN_LCD_RST   2   // Ekran RES
#define PIN_LCD_BL    1   // Ekran BLK (PWM parlaklık kontrolü)

// ---- MicroSD Kart Modülü (Paylaşımlı SPI) ----
// SD SCK -> GPIO 4 | SD MOSI -> GPIO 6 | SD MISO -> GPIO 5 | SD CS -> GPIO 20 (RX)
#define USE_SD_CARD   true
#define PIN_SD_MISO   5   // MicroSD MISO
#define PIN_SD_CS     20  // MicroSD CS (ESP32-C3 SuperMini üzerindeki RX pini)
#define SD_SPI_HZ     20000000

// ---- Ses Amfisi (MAX98357A I2S) ----
// MAX98357A VIN -> 5V (TP4056 OUT+) | GND -> GND | +/- -> Hoparlör
#define USE_AUDIO     true
#define PIN_I2S_BCLK  0   // MAX98357A BCLK
#define PIN_I2S_LRC   10  // MAX98357A LRC
#define PIN_I2S_DIN   21  // MAX98357A DIN (ESP32-C3 SuperMini üzerindeki TX pini)
#define AUDIO_RATE    16000   // 16 kHz, 16-bit signed mono PCM (.pcm)
#define DEFAULT_VOLUME 65     // 0-100 (%)

// ---- Kanal / Wi-Fi / Uyku Butonu ----
// ESP32-C3 SuperMini üzerindeki dahili BOOT butonu GPIO 9'dur.
// Kısa bas: sonraki kanal | 1.5 sn: Wi-Fi aç/kapat | 4 sn: CRT kapanış + uyku
#define PIN_BUTTON    9
#define PIN_SYS_EN    -1

// ---- Ekran ----
#define LCD_ROTATION  1      // 1 veya 3 (görüntü ters ise değiştirin)
#define SCREEN_W      280    // yatay kullanım (4:3'e yakın CRT oranı)
#define SCREEN_H      240
#define SPI_HZ        40000000   // 40 MHz
#define DEFAULT_BRIGHTNESS 200   // 0-255

// ---- Video ----
#define VIDEO_DIR     "/v"
#define DEFAULT_FPS   12
#define FRAME_BUF_SIZE (36 * 1024)   // 36 KB — PSRAM'siz ESP32-C3 SRAM'ine rahat sığar
#define STATIC_MS     450            // kanal değişiminde "karlanma + parazit sesi" süresi (ms)

// ---- Wi-Fi ----
// Ev Wi-Fi'ı (boş bırakılırsa TV doğrudan kendi ağını açar)
#define WIFI_STA_SSID ""
#define WIFI_STA_PASS ""
// Kendi erişim noktası (ev ağı yoksa / bağlanamazsa)
#define WIFI_AP_SSID  "MiniTV"
#define WIFI_AP_PASS  "retro1234"     // en az 8 karakter
#define MDNS_NAME     "minitv"        // http://minitv.local
// Pil tasarrufu: web arayüzü bu süre kullanılmazsa Wi-Fi kapanır (0 = asla)
#define WIFI_AUTO_OFF_MIN 10
// Açılışta Wi-Fi otomatik açılsın mı?
#define WIFI_ON_BOOT  true
