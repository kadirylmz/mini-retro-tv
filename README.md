# 📺 Mini Retro TV (Sesli, MicroSD'li, Şarjlı, Wi-Fi Video Yüklemeli)

Figür ve diorama rafları için **55.9 × 36.2 × 28.3 mm** (gerçek derin tüplü CRT oranında, avuç içi/saat boyutunda) tam renkli, **gerçek sesli**, **MicroSD kartlı**, şarjlı ve kablosuz video yüklenebilen retro televizyon projesi.

<p align="center">
  <img src="enclosure/preview.png" alt="Mini Retro TV Ön Görünüm" width="48%">
  <img src="enclosure/preview_back.png" alt="Mini Retro TV Arka Görünüm" width="48%">
</p>

<p align="center">
  <img src="enclosure/preview_plate.png" alt="3D Baskı Parçaları (Tek Tabla)" width="70%">
</p>

---

## 1. Donanım Seti

| Parça | Seçilen Model | Görevi |
|---|---|---|
| **Ekran** | **1.69" 240×280 SPI IPS TFT LCD (ST7789, 8-pin)** | Yuvarlak köşeli canlı IPS ekran (gerçek tüplü TV görünümü) |
| **Wi-Fi Beyin** | **ESP32-C3 SuperMini (Lehimsiz)** | 18 × 22.5 mm, Wi-Fi, 4 MB Flash, Type-C |
| **Ses Sistemi** | **MAX98357A I2S 3W Dijital Amfi + 8Ω 1W Mini Hoparlör** | Video sesleri + kanal değişiminde gerçek retro TV cızırtısı |
| **Depolama** | **SPI MicroSD Kart Modülü** *(takılı değilse dahili hafıza)* | Gigabaytlarca çizgi film / dizi / film klibi |
| **Şarj & Pil** | **TP4056 Type-C Korumalı + 3.7V 600 mAh LiPo (603035)** | Tek şarjla ~4–6 saat sesli oynatma |
| **Kasa & Anten** | **3D Baskı (~20g PLA) + 1 adet metal ataş** | Destek (support) ve vida gerektirmez, açık ön hoparlör ızgaralı |

👉 **Satın Alma Linkleri:** [SATIN_ALMA_LISTESI.md](SATIN_ALMA_LISTESI.md)  
👉 **Ayrıntılı Pin Tablosu & Montaj:** [BAGLANTI_SEMASI.md](BAGLANTI_SEMASI.md)

---

## 2. 🔌 Görsel Bağlantı Şeması ve Lehim Sırası

![Mini Retro TV Bağlantı Şeması](docs/baglanti_semasi.png)

### Kartı Doğru Tutun (`ESP32-C3 SuperMini`)
USB-C soketi **yukarı**, çipli yüz **size** bakacak şekilde:
* **Sol sıra (yukarıdan aşağı):** `5`, `6`, `7`, `8`, `9`, `10`, `20 (RX)`, `21 (TX)`
* **Sağ sıra (yukarıdan aşağı):** `5V`, `G (GND)`, `3V3`, `4`, `3`, `2`, `1`, `0`
*(Header pinlerini takmayın; ince 28–30 AWG kabloları doğrudan deliklere lehimleyin.)*

### Adım Adım 4 Aşamalı Bağlantı
1. **Ekran (8 Kablo — Sadece 3.3V!):**
   `GND→G`, `VCC→3V3`, `SCL→4`, `SDA→6`, `RES→2`, `DC→3`, `CS→7`, `BLK→1`
   *(Önce pili bağlamadan USB-C ile kodu yükleyip ekranda renk çubuklarını test edin.)*
2. **Ses Amfisi (`MAX98357A`) + Mini Hoparlör:**
   `VIN→5V`, `GND→G`, `BCLK→0`, `LRC→10`, `DIN→21 (TX)`, `+ / - → Hoparlör` *(SD ve GAIN boş kalır)*
3. **MicroSD Kart Modülü (Paylaşımlı SPI):**
   `GND→G`, `VCC→3V3`, `SCK→4` *(Ekran SCL ile ortak)*, `MOSI→6` *(Ekran SDA ile ortak)*, `MISO→5`, `CS→20 (RX)`
   *(5V regülatörlü modül pille çalışırken kartı görmezse `3V3`'ü modüldeki 3 bacaklı AMS1117 regülatörünün orta bacağına bağlayın.)*
4. **Güç & Şarj (En Son, USB Sökülüyken):**
   Pil Kırmızı (`+`) → TP4056 `B+` | Pil Siyah (`-`) → TP4056 `B-` | TP4056 `OUT+` → ESP32 `5V` (+ Amfi `VIN`) | TP4056 `OUT-` → Ortak `GND`.

> **🛡️ 3 Altın Kural:** (1) İlk kod yüklerken sadece ESP32-C3 USB'sini kullanın (pil ayrık olsun). (2) Günlük şarjı sadece sol yandaki **TP4056 Type-C** portundan yapın. (3) Kasa içinde tüm kartların arkasına **elektrik/kapton bant** yapıştırın.

---

## 3. 📱 Web Arayüzü Kullanımı (`http://192.168.4.1` / `http://minitv.local`)

ESP32 içinde gömülü çalışan web paneli sayesinde **telefondan veya bilgisayardan ekstra uygulama kurmadan** TV'yi yönetip video+ses yükleyebilirsiniz:

### İki Farklı Bağlanma Yöntemi
* **A) Kendi Ağı Üzerinden (`http://192.168.4.1`) — Varsayılan, Ayar Gerektirmez:**
  1. Telefondan **`MiniTV`** Wi-Fi ağına bağlanın *(Şifre: **`retro1234`**)*.
  2. Tarayıcıdan **`http://192.168.4.1`** adresini açın *(İnternet yok uyarısında "Bağlı kal" deyin)*.
* **B) Ev Wi-Fi'ınız Üzerinden (`http://minitv.local`) — İsteğe Bağlı:**
  * Kodu yüklemeden önce [config.h](firmware/MiniRetroTV/config.h) içindeki `WIFI_STA_SSID` ve `WIFI_STA_PASS` alanlarına ev Wi-Fi bilgilerinizi yazarsanız TV ev modeminize bağlanır; telefon Wi-Fi'ını değiştirmeden **`http://minitv.local`** adresinden girebilirsiniz.

### Web Panelinde Neler Var?
* **Hafıza, Parlaklık ve 🔊 Ses Seviyesi (%0–100):** MicroSD kart (veya dahili hafıza) doluluğunu canlı gösterir; ekran parlaklığını ve hoparlör sesini anında ayarlayıp kaydeder.
* **Program Gerektirmeyen Kablosuz MP4 Yükleme:** Telefon galerisinden seçtiğiniz MP4 / MOV / WebM videosunu tarayıcı içinde otomatik olarak **280×240 `.mjpeg`** + **16 kHz mono `.pcm` ses** dosyasına çevirip kablosuz olarak TV'ye gönderir; biter bitmez karlanma efektiyle oynatmaya başlar.
* **Kanallar (Uzaktan Kumanda):** Yüklü tüm videoları `CH1`, `CH2`, `CH3` şeklinde listeler; tek tıkla kanal değiştirebilir veya silebilirsiniz.
* **Otomatik Wi-Fi Pil Tasarrufu:** 10 dakika işlem yapılmazsa Wi-Fi pil için otomatik kapanır (videolar oynamaya devam eder). Tekrar açmak için üstteki `BOOT` butonuna **1.5 saniye** basılı tutmanız yeterlidir.

> 💡 **Uzun Çizgi Film / Dizi Bölümleri İçin:** 5–45 saniyelik klipleri telefondan kablosuz yüklemek çok pratiktir. 15–20 dakikalık uzun bölümler için MicroSD kartı PC'ye takıp MP4 dosyasını [convert_to_mjpeg.bat](tools/convert_to_mjpeg.bat) üzerine sürükleyip çıkan `.mjpeg` + `.pcm` dosyalarını doğrudan SD karttaki **`/v/`** klasörüne atın.

---

## 4. ⚙️ Fiziksel Buton Kontrolleri & Arduino IDE Ayarları

* **Üstteki Buton (`BOOT` / GPIO 9):**
  * **Kısa bas:** Karlanma görüntüsü + cızırtı sesi + yeşil `CH1 / CH2` yazısıyla sonraki kanala geçer *(TV uykudaysa CRT efektiyle uyandırır)*.
  * **1.5 sn basılı tut:** Wi-Fi açar / kapatır.
  * **4 sn basılı tut:** Tüplü TV kapanış çizgisiyle TV'yi kapatır / uyku moduna alır.
* **Arduino IDE Ayarları (`ESP32-C3 SuperMini`):**
  * Kütüphaneler: `GFX Library for Arduino`, `JPEGDEC` *(ESP32 paketi: `3.1.3`)*
  * **Board:** `ESP32C3 Dev Module` | **USB CDC On Boot:** `Enabled` | **Flash Size:** `4MB (32Mb)` | **Partition Scheme:** `No OTA (2MB APP/2MB SPIFFS)`

---

## 5. Proje Dosyaları

* **3D Kasa (`enclosure/`):**
  * [mini_retro_tv.scad](enclosure/mini_retro_tv.scad) — Parametrik OpenSCAD tasarımı
  * **Baskıya Hazır STL'ler (`enclosure/stl/`):** [body.stl](enclosure/stl/body.stl), [back.stl](enclosure/stl/back.stl), [knobs.stl](enclosure/stl/knobs.stl), [legs.stl](enclosure/stl/legs.stl), [antenna.stl](enclosure/stl/antenna.stl), [plate.stl](enclosure/stl/plate.stl)
* **Yazılım (`firmware/MiniRetroTV/`):** [MiniRetroTV.ino](firmware/MiniRetroTV/MiniRetroTV.ino), [config.h](firmware/MiniRetroTV/config.h), [web_page.h](firmware/MiniRetroTV/web_page.h)
* **Görseller & Dokümanlar:** [baglanti_semasi.png](docs/baglanti_semasi.png), [BAGLANTI_SEMASI.md](BAGLANTI_SEMASI.md), [SATIN_ALMA_LISTESI.md](SATIN_ALMA_LISTESI.md)
* **PC Video+Ses Dönüştürücü (`tools/`):** [convert_to_mjpeg.bat](tools/convert_to_mjpeg.bat)
