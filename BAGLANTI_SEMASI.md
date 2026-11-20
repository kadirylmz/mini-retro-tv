# 🔌 Mini Retro TV — Tam Pin Bağlantı Şemaları

> 💡 **İki Desteklenen Donanım Yapılandırması:**
> 1. **Doğrulanmış ve Çalışan Güncel Yapılandırma:** **1.3" ST7789 IPS Ekran + ESP32-S3 + 16GB MicroSD (Çift Donanımsal SPI, 25 FPS Akıcı Oynatıcı)**
> 2. **Alternatif Kompakt Yapılandırma:** **1.69" ST7789 + ESP32-C3 SuperMini + MAX98357A I2S Amfi**

---

## 🌟 1. Doğrulanmış Çalışan Sistem (1.3" ST7789 + ESP32-S3 + 16GB MicroSD)

Bu bağlantı bizzat fiziksel olarak test edilmiş ve **15.193 MB (16GB) MicroSD** ile **25 FPS akıcı video oynatımı** doğrulanmıştır. Ekran ile SD kart birbirinden bağımsız çift donanımsal SPI veriyolu (`FSPI` ve `HSPI`) üzerinden çalışır, böylece sıfır veri çakışması ve maksimum hız elde edilir.

<p align="center">
  <img src="docs/baglanti_semasi_13inch.png" alt="Mini Retro TV 1.3 inç Doğrulanmış Görsel Devre Şeması" width="100%">
</p>

### A. Voltaj 1.69" 240×280 ST7789 IPS Ekran (8-Pin) ↔ ESP32-S3 (FSPI Veriyolu - Sol Sıra)

<p align="center">
  <img src="docs/baglanti_semasi_13inch.png" alt="Voltaj 1.69 inç ST7789 ve ESP32-S3 Bağlantı Şeması" width="100%">
</p>

| # | Ekran Pini (1.69") | ESP32-S3 Pini | ESP32-C3 Pini | Görevi / Açıklama |
|:---:|:---|:---|:---|:---|
| **1** | **GND** | **GND** | **GND** | Ortak Toprak |
| **2** | **VCC** | **3.3V (3V3)** | **3.3V (3V3)** | ⚠️ **Sadece 3.3V! Asla 5V vermeyin!** |
| **3** | **SCL (CLK)** | **GPIO 4** | **GPIO 4** | Donanımsal FSPI Saat Sinyali |
| **4** | **SDA (MOSI)**| **GPIO 6** | **GPIO 6** | Donanımsal FSPI Veri Hattı |
| **5** | **RES (RST)** | **GPIO 2** | **GPIO 2** | Donanımsal Ekran Sıfırlama |
| **6** | **DC** | **GPIO 3** | **GPIO 3** | Veri / Komut Seçimi (Data/Command) |
| **7** | **CS** | **GPIO 5** *(veya GND)* | **GPIO 7** | Chip Select (Ekran Seçim Hattı) |
| **8** | **BLK (BL)** | **GPIO 1** | **GPIO 1** | PWM Arka Aydınlatma & Parlaklık Kontrolü |

---

### B. 16GB MicroSD Modülü ↔ ESP32-S3 (HSPI Veriyolu - Sağ Sıra)

> ⚠️ **KRİTİK GÜÇ NOTU:** Arduino MicroSD modülleri üzerinde dahili `AMS1117-3.3V` regülatör bulunur. Modülün `VCC` pinini ESP32-S3'ün **5V (5U)** pinine bağlayın! 3.3V verilirse voltaj 2.1V'a düşer ve kart başlatılamaz.

| MicroSD Modülü | ESP32-S3 Pini | Header Konumu | Görevi / Açıklama |
| :--- | :--- | :--- | :--- |
| **VCC** | **5V (5U)** | Üst Güç Pini | **5V Besleme (Dahili regülatör için şart)** |
| **GND** | **GND** | Üst Toprak | Ortak Toprak |
| **CS** | **GPIO 10** | Sağ Sıra 10 | Bağımsız SD Kart Seçim Pini |
| **MOSI** | **GPIO 11** | Sağ Sıra 11 | Donanımsal HSPI Veri Gönderme |
| **SCK (CLK)**| **GPIO 12** | Sağ Sıra 12 | Donanımsal HSPI Saat Sinyali |
| **MISO** | **GPIO 13** | Sağ Sıra 13 | Donanımsal HSPI Veri Okuma |

---

### C. MAX98357A I2S 3W Ses Amfisi (7-Pin Header) + Hoparlör ↔ ESP32-S3

Fiziksel kart üzerindeki 7 pinin sırasıyla bağlantı durumu:

| # | MAX98357A Pini | ESP32-S3 Pini | Durum | Açıklama / Görevi |
|:---:|:---|:---|:---:|:---|
| **1** | **LRC** | **GPIO 8** | Bağlı | I2S Word Select / Sol-Sağ Kanal Sinyali |
| **2** | **BCLK** | **GPIO 7** | Bağlı | I2S Bit Clock (Ses Saat Sinyali) |
| **3** | **DIN** | **GPIO 9** | Bağlı | I2S Data In (Sayısal Ses Verisi) |
| **4** | **GAIN** | *(Boş)* | **BOŞ** | **Bağlanmaz!** Boş bırakıldığında varsayılan 9 dB temiz kazançta çalışır |
| **5** | **SD** | *(Boş veya VIN)* | **BOŞ / VIN KÖPRÜSÜ** | Standartta boştur `(L+R)/2 mono`. **Eğer ses gelmiyorsa:** Klon kartlarda dahili pull-up direnci eksik olduğundan amfi otomatik uyku moduna geçer. **5. SD pinini hemen yanındaki 7. VIN (5V) pinine küçük bir kabloyla köprüleyin!** Böylece amfi anında uyanır ve sol kanal sesi basar. |
| **6** | **GND** | **GND** | Bağlı | Ortak Toprak Hattı |
| **7** | **VIN** | **5V (5U)** | Bağlı | 5V Güç Beslemesi (Hoparlörden 3W tam ve net güç almak için) |
| — | **`+` / `−`** | **Hoparlör** | Bağlı | 8Ω 1W mini hoparlör uçlarına lehimlenir (+ Kırmızı, − Siyah) |

---

### D. Çok Fonksiyonlu Kontrol Butonu (`GPIO 0` - BOOT Butonu)

* **Tek Kısa Basış:** Sonraki video / kanala geçiş (`nextChannel()`).
* **Çift Tıklama (Hızlı 2 Basış):** **SESİ AÇ / KAPAT (MUTE TOGGLE)!** Ekranda anında kırmızı `[ SES KAPALI (MUTE) ]` veya yeşil `[ SES: %70 ]` OSD'si çıkar.
* **Uzun Basış (> 1.2 saniye):** Ses kademesi döngüsü (`%25` $\rightarrow$ `%50` $\rightarrow$ `%75` $\rightarrow$ `%100` $\rightarrow$ `%0`). Ekranda canlı seviye çubuğu gösterilir.

---

### E. TP4056 + 5V Boost Ayarlı Voltaj Yükseltici & Li-Po Pil

> ⚠️ **HAYATİ AYAR UYARISI:** Kart fabrikadan 9V - 12V ayarlı gelir. **ESP32'ye bağlamadan önce** pili karta lehimleyin, bir multimetre ile `VOUT+` ve `VOUT-` uçlarını ölçerek üzerindeki mavi trimpot vidasını çevirip voltajı **TAM 5.0V** seviyesine sabitleyin!

| Modül Pini | Bağlantı Hedefi | Kablo Rengi / Voltaj | Açıklama / Not |
| :--- | :--- | :--- | :--- |
| **BAT +** | 3.7V Li-Po Pil (+) | Kırmızı (3.7V Nominal) | 603035 / 703048 Lityum Polimer Pil Artı Ucu |
| **BAT -** | 3.7V Li-Po Pil (-) | Siyah (GND) | Lityum Pil Eksi Ucu |
| **VOUT +** | ESP32-S3 **5V (5U)** Pini | Kırmızı (**5.0V Sabit**) | Yükseltilmiş ve filtrelenmiş kararlı 5V güç |
| **VOUT -** | ESP32-S3 **GND** Pini | Siyah (GND) | Ana sistem toprak hattı |
| **Type-C** | 5V Telefon Şarj Kablosu | 5V DC Giriş | Pili şarj eder (Kırmızı: Şarjda, Mavi: Dolu) |

---

## 2. Alternatif Kompakt Yapılandırma (1.69" + ESP32-C3 SuperMini)

<p align="center">
  <img src="docs/baglanti_semasi.png" alt="Mini Retro TV 1.69 inç Orijinal Görsel Devre Şeması" width="100%">
</p>

| 1.69" ST7789 Ekran | ESP32-C3 SuperMini | Not |
| :--- | :--- | :--- |
| **GND** | **G (GND)** | Ortak toprak |
| **VCC** | **3V3** | 3.3V *(Kesinlikle 5V vermeyin)* |
| **SCL** | **GPIO 4** | SPI SCK *(MicroSD SCK ile ortak)* |
| **SDA** | **GPIO 6** | SPI MOSI *(MicroSD MOSI ile ortak)* |
| **RES** | **GPIO 2** | Reset |
| **DC** | **GPIO 3** | Data / Command |
| **CS** | **GPIO 7** | Ekran Chip Select |
| **BLK** | **GPIO 1** | Arka ışık parlaklık (PWM) |

---

## 2. Ses Amfisi (`MAX98357A`) + Mini Hoparlör ↔ ESP32-C3 (5 Kablo + Hoparlör)

| MAX98357A Pini | Bağlanacağı Yer | Not |
| :--- | :--- | :--- |
| **VIN** | **TP4056 `OUT+` (veya ESP32 `5V`)** | 2.5V–5.5V pil voltajından direkt beslenir (ESP32 3.3V'unu yormaz!) |
| **GND** | **G (GND)** | Ortak toprak |
| **BCLK** | **ESP32 `GPIO 0`** | I2S Saat |
| **LRC** | **ESP32 `GPIO 10`** | I2S Sol/Sağ saat |
| **DIN** | **ESP32 `GPIO 21` (`TX`)** | I2S Ses verisi |
| **`+` / `-`** | **Mini Hoparlör telleri** | Kutuplu/kutupsuz bağlanabilir |

*(Not: `SD` ve `GAIN` pinleri boş bırakılır — varsayılan 9 dB ses kazanımı ve sürekli aktif modda çalışır.)*

---

## 3. MicroSD Kart Modülü (SPI) ↔ ESP32-C3 (6 Kablo)

| MicroSD Modülü | ESP32-C3 SuperMini | Not |
| :--- | :--- | :--- |
| **GND** | **G (GND)** | Ortak toprak |
| **VCC** | **3V3** *(veya 5V)* | ⚠️ **Önemli not aşağıda!** |
| **SCK** | **GPIO 4** | Ekran `SCL` ile aynı pin |
| **MOSI** | **GPIO 6** | Ekran `SDA` ile aynı pin |
| **MISO** | **GPIO 5** | SPI veri okuma |
| **CS** | **GPIO 20 (`RX`)** | MicroSD Chip Select |

> **⚠️ MicroSD Modülü 3.3V / 5V Besleme Notu:**
> * Standart Arduino MicroSD modüllerinin üzerinde 5V→3.3V düşüren `AMS1117-3.3` çipi bulunur. Pilden gelen 3.7V bu regülatörden geçerse 2.6V'a düşüp SD kartı görmeyebilir.
> * **Çözüm:** İlk testte SD modülünün `VCC` pinini ESP32'nin `5V` (pil) pinine bağlayıp deneyin; pille çalışırken SD kartı okumazsa (USB'de okuyup pilde okumazsa), SD modülündeki `AMS1117` regülatörünün **orta bacağına (3.3V çıkışına)** ESP32'nin **`3V3`** pininden ince bir tel lehimlemek sorunu kalıcı olarak çözer.

---

## 4. Pil + TP4056 ↔ Güç Dağıtımı

```text
[ Powermaster 603035 3.7V 600mAh Pil ]
   Kırmızı (+) ────────► TP4056  B+
   Siyah   (-) ────────► TP4056  B-

[ TP4056 Type-C (Korumalı) ]
   OUT+ ──► ESP32-C3  5V  +  MAX98357A VIN
   OUT- ──► ESP32-C3  G   +  MAX98357A GND  +  Ekran GND  +  MicroSD GND
```

---

## 5. Kasa İçi Yerleşim (`55.9 × 36.2 × 28.3 mm` — Derin Tüplü CRT Oranı)

* **Ön Katman:** 1.69" Ekran PCB'si *(8-pin sağda)* + sağ alttaki açık ızgaranın hemen arkasına ince **Mini Hoparlör**
* **Orta Katman:** Sol tarafta **TP4056** *(USB-C portu sol deliğe bakar)*, ortada **ESP32-C3 SuperMini** + **MAX98357A**, sağda **MicroSD Modülü** *(kart yuvası sağ yandaki slota bakar)* — aralarına mutlaka elektrik bandı!
* **Arka Katman:** **Powermaster 603035 (6×30×35 mm)** pil + geçmeli arka kapak.

---

## 6. Video & Ses Yükleme (2 Kolay Yol)

1. **Telefondan Kablosuz (Wi-Fi):** `MiniTV` ağına bağlanıp `http://192.168.4.1` üzerinden herhangi bir **MP4** seçin — tarayıcı hem **görüntüyü (.mjpeg)** hem **sesi (.pcm)** otomatik dönüştürüp MicroSD karta kaydeder!
2. **PC'den Toplu Uzun Video / Çizgi Film (En Hızlı):** MicroSD kartı PC'ye takın, MP4/MKV dosyalarınızı [convert_to_mjpeg.bat](tools/convert_to_mjpeg.bat) üzerine sürükleyip bırakın; oluşan `.mjpeg` + `.pcm` dosyalarını SD karttaki **`/v/`** klasörüne atın!
