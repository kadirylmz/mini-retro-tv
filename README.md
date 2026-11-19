# 📺 Mini Retro TV (16GB MicroSD, Wi-Fi Otomatik Medya Yüklemeli, 25 FPS)

Figür ve diorama rafları için avuç içi / masaüstü boyutunda, **1.3" IPS canlı ekranlı**, **16GB MicroSD kart destekli**, **25 FPS akıcı video oynatıcılı**, kablosuz web panelli retro televizyon projesi.

<p align="center">
  <img src="enclosure/preview.png" alt="Mini Retro TV Ön Görünüm" width="48%">
  <img src="enclosure/preview_back.png" alt="Mini Retro TV Arka Görünüm" width="48%">
</p>

---

## 🌟 Öne Çıkan Özellikler

1. **🎬 Çift SPI & 25 FPS Akıcı Video Motoru:**
   - Ekran `FSPI` (GPIO 4, 6, 2, 3, 1) veriyolunda 30 MHz hızında çalışır.
   - 16GB MicroSD modülü bağımsız `HSPI` (GPIO 10, 11, 12, 13) veriyolunda çalışır; CS çakışması sıfıra indirilmiştir.
   - 4096 baytlık RAM tamponlu (chunk-buffered) okuma motoru sayesinde takılma olmadan pürüzsüz 25 FPS oynatım sağlar.

2. **🔊 Gerçek Zamanlı Ses Kontrolü, Kısma ve Kapatma (MUTE):**
   - **Web Panelinden Kontrol:** %0 – %100 ses kaydırıcısı, tek tıkla **"Sesi Kapat (MUTE)"** butonu ve hızlı ses kademeleri (%0, %25, %50, %75, %100).
   - **Fiziksel Buton Kontrolü (Cihaz Üzerinden):**
     - **Tek tık:** Sonraki kanala/videoya geçiş.
     - **Çift tık (hızlı 2 basış):** **SESİ AÇ / KAPAT (MUTE TOGGLE)!**
     - **Uzun basış (>1.2 sn):** Ses kademeleri arasında geçiş (%0 $\rightarrow$ %25 $\rightarrow$ %50 $\rightarrow$ %75 $\rightarrow$ %100).
   - **Tüplü TV OSD Göstergesi (On-Screen Display):** Ses seviyesi değiştiğinde veya sessize alındığında ekranda 1.8 saniye boyunca canlı `[ 🔇 SES KAPALI (MUTE) ]` veya `[ 🔊 SES: %70 ████▒▒▒▒ ]` retro çubuğu belirir.
   - **Kalıcı Hafıza (NVS):** Cihaz kapatılıp açılsa bile son ses seviyesi ve mute durumu otomatik hatırlanır.

3. **✨ Tarayıcıdan Tek Tıkla Fotoğraf & Video (+Ses) Yükleme:**
   - Herhangi bir ek programa veya FFmpeg komutlarına ihtiyaç yoktur!
   - `CollectibleDisplay` Wi-Fi ağına bağlanıp tarayıcıdan `http://192.168.4.1` adresine girildiğinde:
     - **Fotoğraf:** JPG, PNG, WEBP, BMP anında 240×240'a dönüştürülüp ekranda sabit gösterilir.
     - **Video + Ses:** MP4, MOV, WEBM seçildiğinde tarayıcı Web Audio API ile videonun sesini 16 kHz mono PCM formatına ayıklar, görüntüyü 240×240 20 FPS MJPEG yapar ve ikisini senkronize şekilde SD karta yükler.
     - İsteğe bağlı olarak *"Videonun sesini de aktar"* seçeneği kapatılarak sadece sessiz video yüklenebilir.

4. **🖨️ Yenilenen 3D Kasa (1.3" Ekran ve Kablo Montajı İçin Genişletildi):**
   - 1.3" ST7789 ekranın sol kenarındaki 7-pin header ve dişi DuPont kablolar için sol taraftan **1 cm (10 mm) genişletildi** (`pcb_w = 59.0 mm`, toplam dış genişlik `65.9 mm`).
   - Ekran penceresi 1.3" kare aktif alan için `27.5 × 26.5 mm` olarak hassas ayarlandı.
   - Geçmeli kapak tasarımı sayesinde vidaya ihtiyaç duymaz.

---

## 1. Donanım Özeti

| Parça | Model | Görevi / Açıklama |
|---|---|---|
| **Ekran** | **1.3" 240×240 SPI IPS TFT LCD (ST7789, 7-pin)** | 240×240 piksel, geniş açılı IPS panel |
| **İşlemci Kartı** | **ESP32-S3 Mini / ESP32-C3** | Çift çekirdek / Wi-Fi / Bluetooth, çift donanımsal SPI desteği |
| **Depolama** | **16GB MicroSD Kart Modülü (SPI)** | Onlarca saat video ve yüzlerce fotoğraf hafızası |
| **Kasa** | **3D Baskı (OpenSCAD Parametrik)** | 1 cm kablo geçiş paylı, 65.9 × 36.4 × 28.8 mm |

👉 **Orijinal Satın Alma Listesi:** [SATIN_ALMA_LISTESI.md](SATIN_ALMA_LISTESI.md) *(Değiştirilmeden korunmuştur)*  
👉 **Ayrıntılı Pin Bağlantı Tablosu:** [BAGLANTI_SEMASI.md](BAGLANTI_SEMASI.md)

---

## 2. 🔌 Doğrulanmış Çalışan Pin Bağlantıları (1.3" ST7789 + ESP32-S3)

### Ekran (FSPI Veriyolu)
* **GND** $\rightarrow$ `GND`
* **VCC** $\rightarrow$ `3.3V`
* **SCL** $\rightarrow$ `GPIO 4`
* **SDA** $\rightarrow$ `GPIO 6`
* **RES** $\rightarrow$ `GPIO 2`
* **DC**  $\rightarrow$ `GPIO 3`
* **BLK** $\rightarrow$ `GPIO 1`

### 16GB MicroSD Kart Modülü (HSPI Veriyolu)
* **VCC** $\rightarrow$ **`5V (5U)`** *(Dahili AMS1117 regülatörün sağlıklı çalışması için 5V şarttır)*
* **GND** $\rightarrow$ `GND`
* **CS**  $\rightarrow$ `GPIO 10`
* **MOSI** $\rightarrow$ `GPIO 11`
* **SCK** $\rightarrow$ `GPIO 12`
* **MISO** $\rightarrow$ `GPIO 13`

---

## 3. 🌐 Kablosuz Web Arayüzü Kullanımı

1. Cihaza güç verdiğinizde **`CollectibleDisplay`** Wi-Fi erişim noktası yayınlanır.
2. Telefon veya bilgisayarınızdan bu ağa bağlanın *(Şifre: **`display1234`**)*.
3. Tarayıcınızda **`http://192.168.4.1`** adresine gidin.
4. Karşınıza gelen modern panelde:
   - 16GB SD kartınızın kalan ve kullanılan alanını canlı olarak görebilirsiniz.
   - Sürükle-bırak veya dosya seçici ile herhangi bir fotoğraf veya videoyu seçebilirsiniz.
   - Tarayıcınız görseli anında 240×240 boyutuna ölçekler ve SD karta yükler.
   - Yüklenen medya anında TV ekranında oynamaya başlar!
   - Alt kısımdaki listeden yüklü videolar arasında geçiş yapabilir veya silebilirsiniz.

---

## 4. 🖨️ 3D Baskı Dosyaları (`enclosure/stl/`)

* [body.stl](enclosure/stl/body.stl) — Ana TV gövdesi (Sol tarafta 1 cm kablo payı eklenmiş)
* [back.stl](enclosure/stl/back.stl) — Geçmeli arka kapak
* [plate.stl](enclosure/stl/plate.stl) — Tüm parçalar tek baskı tablasında (Gövde + Kapak + Düğmeler + Ayaklar + Anten)
* [knobs.stl](enclosure/stl/knobs.stl) — Ön dekoratif çevirme düğmeleri
* [legs.stl](enclosure/stl/legs.stl) — Retro konik TV ayakları
* [antenna.stl](enclosure/stl/antenna.stl) — V anten tabanı ve boncukları
* [mini_retro_tv.scad](enclosure/mini_retro_tv.scad) — Parametrik OpenSCAD kaynak kodu

---

## 5. 🛠️ Proje Dizin Yapısı

```
mini-retro-tv/
├── BAGLANTI_SEMASI.md       # Ayrıntılı pin bağlantı rehberi
├── README.md                # Proje genel tanıtımı ve kullanım kılavuzu
├── SATIN_ALMA_LISTESI.md    # Korunan orijinal malzeme sepeti listesi
├── enclosure/               # 3D Baskı ve CAD kaynak dosyaları
│   ├── mini_retro_tv.scad   # OpenSCAD kaynak modeli (+10 mm sol paylı)
│   ├── preview.png          # Kasa ön görseli
│   ├── preview_back.png     # Kasa arka görseli
│   └── stl/                 # Baskıya hazır STL dosyaları (body, back, plate vb.)
├── firmware/
│   └── MiniRetroTV/         # Arduino IDE yazılımı
│       ├── MiniRetroTV.ino  # 25 FPS çift SPI video motoru
│       ├── config.h         # Pin tanımları ve ayarlar
│       └── web_page.h       # Otomatik fotoğraf/video dönüştürücülü web panel
├── sample_media/            # Örnek hazır MJPEG videolar
│   ├── korku_filmi_ornegi.mjpeg
│   └── ornek_retro_animasyon.mjpeg
└── tools/                   # PC dönüştürücü araçları (FFmpeg & Python)
    ├── video_donusturucu.py
    └── video_donusturucu.bat
```
