# 🔌 Mini Retro TV — Tam Pin Bağlantı Şeması (Sesli + MicroSD Sürümü)

![Mini Retro TV Bağlantı Şeması](docs/baglanti_semasi.png)

**Tam Donanım Seti (7 Parça):**
1. **Voltaj 1.69" 240×280 SPI IPS TFT Ekran (ST7789)**
2. **TLS Robotik ESP32-C3 SuperMini (Lehimsiz)**
3. **MAX98357A I2S 3W Dijital Ses Amfisi**
4. **8Ω 1W Mini Hoparlör** *(20 mm yuvarlak veya 14×20 mm ince)*
5. **MicroSD Kart Modülü (SPI)** + FAT32 formatlı MicroSD kart
6. **Robocombo TP4056 Type-C Korumalı Şarj Devresi**
7. **Powermaster 603035 3.7V 600mAh LiPo Pil (6 × 30 × 35 mm)**

---

## 1. Ekran ↔ ESP32-C3 SuperMini (8 Kablo)

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
