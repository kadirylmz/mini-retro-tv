# 🛒 Mini Retro TV — Kesinleşen Sepet / Malzeme Listesi (Hepsiburada)

Proje için belirlenen ve birbiriyle %100 uyumlu tam donanım satın alma listesi:

| # | Parça | Seçilen Ürün (Hepsiburada Doğrudan Bağlantı) | Ölçü / Temel Özellikler |
|:---:|:---|:---|:---|
| **1** | **Ekran** | [Voltaj 1.69” Renkli Tft Display Modülü Hd IPS LCD LED Ekran 240X280 Spı ST7789 - 1 Adet](https://www.hepsiburada.com/voltaj-169-renkli-tft-display-modulu-hd-ips-lcd-led-ekran-240x280-spi-st7789--p-HBCV0000FYBBU8?magaza=Voltaj%20Elektronik) | 1.69" IPS, 240×280 piksel, oval / kavisli kenarlar, ST7789 SPI (3.3V). *(Alternatif: 1.3" 240×240 düz kare ST7789 ekran da yazılımla ve pinlerle %100 uyumludur)* |
| **2** | **İşlemci Kartı** | [TLS Robotik ESP32-C3 Super Mini Geliştirme Kartı (Lehimsiz) - 1 Adet](https://www.hepsiburada.com/tls-robotik-esp32-c3-super-mini-gelitirme-karti-lehimsiz--p-HBCV0000BI75NO?magaza=TLS%20Robotik) | 18 × 22.5 mm ultra kompakt boyut, dahili Wi-Fi & BLE, Type-C |
| **3** | **Yüksek Kapasiteli Pil** | [Powermaster 703048 3.7 Volt 1200 Mah Lityum Polimer Pil - 1 Adet](https://www.hepsiburada.com/powermaster-703048-37-volt-1200-mah-lityum-polimer-pil--p-HBCV00009MIVEB?magaza=zemheri) | **703048** (7 × 30 × 48 mm), 3.7V, **1200 mAh** (~3.5 – 4 saat kesintisiz oynatma!) |
| **4** | **Voltaj Yükseltici (Boost)** | [Robo Dünya Ayarlanabilir Voltaj Yükseltici Kart - MT3608 2-24V Giriş Gerilimi ile 2A Çıkış Akımı](https://www.hepsiburada.com/robo-dunya-ayarlanabilir-voltaj-yukseltici-kart-mt3608-2-24v-giri-gerilimi-ile-2a-iki-akimi--p-HBCV00003Z3XGB?magaza=RoboD%C3%BCnya) | MT3608 DC-DC Step-Up; pili kararlı **5.0V**'a yükselterek titreşimi ve SD kart hatalarını önler |
| **5** | **Dijital Ses Amfisi** | [Sensör Modül MAX98357A I2S 3W Class-D Dijital Amplifikatör Modülü Lehimli Mono Amfi Arduino ESP32](https://www.hepsiburada.com/sensor-modul-max98357a-i2s-3w-class-d-dijital-amplifikator-modulu-lehimli-mono-amfi-arduino-esp32--p-HBCV0000AY022T?magaza=Sens%C3%B6r%20Modul) | MAX98357A I2S 3W Class-D dahili DAC, lehimli pinler, kristal netliğinde ses |
| **6** | **Mini Hoparlör** | [Robiz Hoparlör 1W 8ohm 20MM Yuvarlak 10CM Kablolu Mini 2cm Speaker Ses Müzik Amfi Ürün Müze Otomobil](https://www.hepsiburada.com/robiz-hoparlor-1w-8ohm-20mm-yuvarlak-10cm-kablolu-mini-2cm-speaker-ses-muzik-amfi-run-muze-otomobil--p-HBCV0000698PRA?magaza=RobizStore) | 8Ω 1W, 20 mm yuvarlak, kablolu hazır montaj, 3D TV kasası ön ızgarasına tam oturan boyut |
| **7** | **Depolama Modülü** | [Arduino Micro Sd Kart Modülü](https://www.hepsiburada.com/arduino-micro-sd-kart-modulu--p-HBCV0000IX3AM4?magaza=Robotnom) | SPI MicroSD kart yuvası, FAT32 formatında saatlerce video ve müzik depolama |

---

### 🔋 Şarj Modülü Notu
* Pil şarjı için standart **TP4056 Type-C Korumalı Şarj Devresi** kullanılır.
* Bağlantı Sırası: `[3.7V LiPo Pil]` $\rightarrow$ `[TP4056 Şarj Modülü]` $\rightarrow$ `[MT3608 Voltaj Yükseltici (5.0V)]` $\rightarrow$ `[ESP32 / SD / Amfi 5V Hattı]`.

---

### 💡 Montaj İçin Gerekli Yardımcı Malzemeler
* İnce esnek montaj kablosu (26–30 AWG jumper kablo)
* Yalıtım için elektrik bandı veya kapton bant
* 1 adet standart metal ataş (retro TV anteni için, 2 × 24 mm boyunda kesilir)
