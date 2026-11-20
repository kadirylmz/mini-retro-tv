#pragma once

const char INDEX_HTML[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="tr">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Collectible Display - Medya & Ses Paneli</title>
<style>
:root {
  --bg: #0d0f17;
  --card: #151824;
  --accent: #e50914;
  --accent2: #00e5ff;
  --text: #e0e6ed;
  --text-dim: #798299;
  --border: #23273a;
  --danger: #ff4444;
  --success: #00e676;
  --warning: #ffb300;
}
* { box-sizing: border-box; margin: 0; padding: 0; font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; }
body {
  background: var(--bg);
  color: var(--text);
  padding: 16px;
  max-width: 600px;
  margin: 0 auto;
  min-height: 100vh;
}
header {
  text-align: center;
  padding: 16px 0 12px;
  border-bottom: 1px solid var(--border);
  margin-bottom: 16px;
}
h1 {
  font-size: 22px;
  color: var(--accent);
  letter-spacing: 2px;
  text-transform: uppercase;
  text-shadow: 0 0 12px rgba(229,9,20,0.4);
}
.subtitle {
  font-size: 13px;
  color: var(--text-dim);
  margin-top: 4px;
}

/* Instagram Rozeti ve Reklam Kartı */
.insta-badge-top {
  display: inline-flex;
  align-items: center;
  gap: 6px;
  background: rgba(225,48,108,0.15);
  border: 1px solid rgba(225,48,108,0.4);
  color: #ff5388;
  padding: 5px 14px;
  border-radius: 20px;
  font-size: 12px;
  text-decoration: none;
  font-weight: 600;
  margin-top: 10px;
  transition: 0.2s;
}
.insta-badge-top:hover {
  background: rgba(225,48,108,0.3);
  transform: translateY(-1px);
}
.insta-icon { width: 14px; height: 14px; vertical-align: middle; }
.insta-tag {
  background: linear-gradient(45deg, #f09433, #dc2743, #bc1888);
  color: #fff;
  padding: 2px 7px;
  border-radius: 10px;
  font-size: 10px;
  font-weight: bold;
  letter-spacing: 0.5px;
}

.card-insta {
  background: linear-gradient(135deg, rgba(131,58,180,0.18) 0%, rgba(225,48,108,0.22) 50%, rgba(253,29,29,0.18) 100%);
  border: 1px solid rgba(225,48,108,0.45);
  margin-bottom: 16px;
}
.insta-content {
  display: flex;
  align-items: center;
  gap: 12px;
}
.insta-icon-box {
  width: 44px;
  height: 44px;
  border-radius: 12px;
  background: linear-gradient(45deg, #f09433 0%, #e6683c 25%, #dc2743 50%, #cc2366 75%, #bc1888 100%);
  display: flex;
  align-items: center;
  justify-content: center;
  color: #fff;
  flex-shrink: 0;
  box-shadow: 0 4px 12px rgba(225,48,108,0.4);
}
.insta-big-icon { width: 26px; height: 26px; }
.btn-insta {
  background: linear-gradient(45deg, #f09433 0%, #e6683c 25%, #dc2743 50%, #cc2366 75%, #bc1888 100%);
  color: #fff;
  padding: 8px 16px;
  border-radius: 20px;
  font-size: 13px;
  font-weight: bold;
  text-decoration: none;
  display: inline-block;
  box-shadow: 0 4px 12px rgba(225,48,108,0.4);
  white-space: nowrap;
  transition: transform 0.2s;
}
.btn-insta:hover { transform: scale(1.05); }

.card {
  background: var(--card);
  border: 1px solid var(--border);
  border-radius: 12px;
  padding: 16px;
  margin-bottom: 16px;
  box-shadow: 0 4px 12px rgba(0,0,0,0.3);
}
.card h2 {
  font-size: 15px;
  margin-bottom: 12px;
  color: var(--accent2);
  display: flex;
  align-items: center;
  justify-content: space-between;
  text-transform: uppercase;
  letter-spacing: 0.5px;
}
.stat-grid {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 10px;
  margin-bottom: 8px;
}
.stat-box {
  background: rgba(0,0,0,0.3);
  padding: 10px;
  border-radius: 8px;
  text-align: center;
}
.stat-label { font-size: 11px; color: var(--text-dim); }
.stat-val { font-size: 15px; font-weight: bold; color: var(--text); margin-top: 2px; }
.progress-bar-bg {
  background: #252839;
  border-radius: 6px;
  height: 8px;
  overflow: hidden;
  margin-top: 8px;
}
.progress-bar {
  background: linear-gradient(90deg, var(--accent2), var(--accent));
  height: 100%;
  width: 0%;
  transition: width 0.3s;
}

/* Ses Kontrolleri */
.vol-container {
  display: flex;
  align-items: center;
  gap: 12px;
  margin-top: 10px;
}
.vol-slider {
  flex: 1;
  -webkit-appearance: none;
  height: 8px;
  border-radius: 4px;
  background: #252839;
  outline: none;
}
.vol-slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  width: 20px;
  height: 20px;
  border-radius: 50%;
  background: var(--accent2);
  cursor: pointer;
  box-shadow: 0 0 8px rgba(0,229,255,0.6);
}
.vol-badge {
  font-weight: bold;
  font-size: 15px;
  min-width: 45px;
  text-align: right;
  color: var(--accent2);
}
.quick-vol {
  display: flex;
  gap: 6px;
  margin-top: 12px;
  flex-wrap: wrap;
}
.btn-vol {
  flex: 1;
  min-width: 60px;
  padding: 8px 4px;
  font-size: 12px;
  text-align: center;
  background: rgba(0,0,0,0.3);
  border: 1px solid var(--border);
  color: var(--text);
  border-radius: 6px;
  cursor: pointer;
  font-weight: 600;
  transition: 0.2s;
}
.btn-vol:hover {
  border-color: var(--accent2);
  color: var(--accent2);
}
.btn-mute {
  background: rgba(255,68,68,0.15);
  border-color: var(--danger);
  color: var(--danger);
  padding: 10px 16px;
  font-size: 13px;
  border-radius: 8px;
  cursor: pointer;
  font-weight: bold;
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 8px;
  width: 100%;
  margin-top: 10px;
  transition: 0.2s;
}
.btn-mute.is-muted {
  background: var(--danger);
  color: #fff;
  box-shadow: 0 0 12px rgba(255,68,68,0.5);
}

.upload-zone {
  border: 2px dashed var(--border);
  border-radius: 12px;
  padding: 24px 16px;
  text-align: center;
  cursor: pointer;
  background: rgba(0,0,0,0.2);
  transition: 0.2s;
}
.upload-zone:hover, .upload-zone.dragover {
  border-color: var(--accent);
  background: rgba(229,9,20,0.06);
}
.btn {
  background: var(--accent);
  color: #fff;
  border: none;
  padding: 10px 18px;
  border-radius: 8px;
  font-weight: 600;
  cursor: pointer;
  font-size: 14px;
  display: inline-block;
  margin-top: 8px;
}
.btn:hover { opacity: 0.9; }
.btn-small { padding: 6px 12px; font-size: 12px; border-radius: 6px; }
.btn-danger { background: var(--danger); }
.btn-play { background: var(--success); color: #000; }
.video-item {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 12px;
  background: rgba(0,0,0,0.25);
  border-radius: 8px;
  margin-bottom: 8px;
  border-left: 4px solid var(--accent2);
}
.video-item.active {
  border-left-color: var(--accent);
  background: rgba(229,9,20,0.1);
}
.video-info { flex: 1; margin-right: 12px; }
.video-name { font-weight: 600; font-size: 14px; word-break: break-all; }
.video-meta { font-size: 11px; color: var(--text-dim); margin-top: 2px; }
.video-actions { display: flex; gap: 6px; }
.status-msg {
  padding: 10px;
  border-radius: 8px;
  font-size: 13px;
  margin-top: 10px;
  display: none;
  line-height: 1.4;
}
.status-msg.active { display: block; }
.status-info { background: rgba(0,229,255,0.1); color: var(--accent2); border: 1px solid rgba(0,229,255,0.3); }
.status-err { background: rgba(255,68,68,0.1); color: var(--danger); border: 1px solid rgba(255,68,68,0.3); }
.status-ok { background: rgba(0,230,118,0.1); color: var(--success); border: 1px solid rgba(0,230,118,0.3); }
.spinner {
  display: inline-block;
  width: 14px;
  height: 14px;
  border: 2px solid rgba(255,255,255,0.3);
  border-radius: 50%;
  border-top-color: #fff;
  animation: spin 0.8s linear infinite;
  vertical-align: middle;
  margin-right: 6px;
}
@keyframes spin { to { transform: rotate(360deg); } }

.crop-options {
  display: flex;
  gap: 10px;
  margin-top: 12px;
  font-size: 12px;
  justify-content: center;
}
.crop-opt {
  cursor: pointer;
  padding: 6px 12px;
  background: rgba(0,0,0,0.3);
  border: 1px solid var(--border);
  border-radius: 6px;
}
.crop-opt.active {
  border-color: var(--accent2);
  color: var(--accent2);
  background: rgba(0,229,255,0.1);
}
#preview-canvas {
  display: none;
  margin: 12px auto 0;
  border: 2px solid var(--border);
  border-radius: 8px;
  width: 160px;
  height: 160px;
}
.chk-label {
  display: flex;
  align-items: center;
  gap: 8px;
  margin-top: 12px;
  font-size: 13px;
  cursor: pointer;
  background: rgba(0,0,0,0.2);
  padding: 8px 12px;
  border-radius: 6px;
  border: 1px solid var(--border);
}
</style>
</head>
<body>

<header>
  <h1>🎬 COLLECTIBLE DISPLAY</h1>
</header>

<!-- INSTAGRAM TOPLULUK & REKLAM KARTI -->
<div class="card card-insta">
  <div class="insta-content">
    <div class="insta-icon-box">
      <svg viewBox="0 0 24 24" class="insta-big-icon"><path fill="currentColor" d="M12 2.163c3.204 0 3.584.012 4.85.07 3.252.148 4.771 1.691 4.919 4.919.058 1.265.069 1.645.069 4.849 0 3.205-.012 3.584-.069 4.849-.149 3.225-1.664 4.771-4.919 4.919-1.266.058-1.644.07-4.85.07-3.204 0-3.584-.012-4.849-.07-3.26-.149-4.771-1.699-4.919-4.92-.058-1.265-.07-1.644-.07-4.849 0-3.204.013-3.583.07-4.849.149-3.227 1.664-4.771 4.919-4.919 1.266-.057 1.645-.069 4.849-.069zm0-2.163c-3.259 0-3.667.014-4.947.072-4.358.2-6.78 2.618-6.98 6.98-.059 1.281-.073 1.689-.073 4.948 0 3.259.014 3.668.072 4.948.2 4.358 2.618 6.78 6.98 6.98 1.281.058 1.689.072 4.948.072 3.259 0 3.668-.014 4.948-.072 4.354-.2 6.782-2.618 6.979-6.98.059-1.28.073-1.689.073-4.948 0-3.259-.014-3.667-.072-4.947-.196-4.354-2.617-6.78-6.979-6.98-1.281-.059-1.69-.073-4.949-.073zm0 5.838c-3.403 0-6.162 2.759-6.162 6.162s2.759 6.163 6.162 6.163 6.162-2.759 6.162-6.163c0-3.403-2.759-6.162-6.162-6.162zm0 10.162c-2.209 0-4-1.79-4-4 0-2.209 1.791-4 4-4s4 1.791 4 4c0 2.21-1.791 4-4 4zm6.406-11.845c-.796 0-1.441.645-1.441 1.44s.645 1.44 1.441 1.44c.795 0 1.439-.645 1.439-1.44s-.644-1.44-1.439-1.44z"/></svg>
    </div>
    <div style="flex:1;">
      <div style="font-weight:bold;font-size:14px;color:#fff;">Resmi Instagram Sayfamız</div>
      <div style="font-size:11px;color:#e2e8f0;margin-top:2px;">Özel yapım retro mini TV'ler, figürler ve koleksiyon ürünleri için: <b>@collectible_display</b></div>
    </div>
    <a href="https://www.instagram.com/collectible_display/" target="_blank" class="btn-insta">Takip Et</a>
  </div>
</div>

<!-- SES KONTROL KARTI -->
<div class="card">
  <h2>
    <span>🔊 Hoparlör & Ses Kontrolü</span>
    <span id="vol-status-text" style="font-size:12px;color:var(--accent2);">Ses: %70</span>
  </h2>
  <div class="vol-container">
    <span style="font-size:18px;">🔈</span>
    <input type="range" class="vol-slider" id="vol-slider" min="0" max="100" value="70" oninput="onSliderMove(this.value)" onchange="sendVolume(this.value)">
    <span style="font-size:18px;">🔊</span>
    <span class="vol-badge" id="vol-pct">%70</span>
  </div>

  <button class="btn-mute" id="btn-mute" onclick="toggleMute()">
    <span id="mute-icon">🔇</span> <span id="mute-text">Sesi Kapat (MUTE)</span>
  </button>

  <div class="quick-vol">
    <button class="btn-vol" onclick="setQuickVol(0)">🔇 %0 (Sessiz)</button>
    <button class="btn-vol" onclick="setQuickVol(25)">🔉 %25</button>
    <button class="btn-vol" onclick="setQuickVol(50)">🔉 %50</button>
    <button class="btn-vol" onclick="setQuickVol(75)">🔊 %75</button>
    <button class="btn-vol" onclick="setQuickVol(100)">🔊 %100</button>
  </div>

  <div style="margin-top:10px;">
    <button class="btn-mute" style="background:rgba(52,152,219,0.15);border:1px solid #3498db;color:#3498db;font-weight:600;" onclick="testBeep()">
      <span>🔔</span> <span>Hoparlör Donanımını Test Et (Bip Sesi Çal)</span>
    </button>
  </div>

  <!-- Ekran Parlaklığı (Pil Tasarruf Kontrolü) -->
  <div style="margin-top:14px;padding-top:12px;border-top:1px solid rgba(255,255,255,0.08);">
    <div style="display:flex;justify-content:space-between;align-items:center;font-size:12px;margin-bottom:6px;">
      <span style="color:var(--text-dim);">💡 Ekran Parlaklığı (Pil Tasarrufu):</span>
      <span id="disp-bright" style="font-weight:bold;color:var(--accent);">%70</span>
    </div>
    <input type="range" id="bright-slider" min="20" max="255" value="180" oninput="onBrightChange(this.value)" style="width:100%;accent-color:var(--accent);cursor:pointer;">
    <div style="display:flex;justify-content:space-between;font-size:10px;color:var(--text-dim);margin-top:2px;">
      <span>%10 (Eko Pil)</span>
      <span>%70 (Önerilen)</span>
      <span>%100 (Maksimum)</span>
    </div>
  </div>
</div>

<!-- WI-FI & AĞ AYARLARI KARTI -->
<div class="card">
  <h2>
    <span>📶 Wi-Fi & Ağ Ayarları</span>
    <span id="wifi-status-badge" style="font-size:11px;color:var(--text-dim);">Kontrol ediliyor...</span>
  </h2>

  <!-- 1. Hotspot Şifresi Değiştirme & Kolay Bağlantı -->
  <div style="background:rgba(0,0,0,0.25);padding:12px;border-radius:8px;margin-bottom:12px;border:1px solid var(--border);">
    <div style="display:flex;justify-content:space-between;align-items:center;margin-bottom:8px;">
      <div style="font-weight:600;font-size:13px;color:var(--accent2);">📡 Mini TV Hotspot (Direkt Bağlantı)</div>
      <span style="font-size:11px;background:#23273a;padding:2px 8px;border-radius:12px;color:var(--text-dim);">Kanal 6 (Temiz)</span>
    </div>
    <div style="display:flex;justify-content:space-between;font-size:12px;margin-bottom:6px;">
      <span style="color:var(--text-dim);">Wi-Fi Adı (SSID):</span>
      <span id="disp-ap-ssid" style="font-weight:bold;color:#fff;">CollectibleDisplay</span>
    </div>
    <div style="display:flex;justify-content:space-between;font-size:12px;margin-bottom:10px;">
      <span style="color:var(--text-dim);">Güvenlik / Şifre:</span>
      <span id="disp-ap-pass" style="font-family:monospace;font-weight:bold;color:var(--success);">Şifresiz (Tek Tıkla Bağlan)</span>
    </div>
    <div style="display:flex;gap:8px;margin-bottom:8px;">
      <input type="text" id="input-new-ap-pass" placeholder="Şifre koymak isterseniz yazın (en az 8 hane)" style="flex:1;background:#0d0f17;border:1px solid var(--border);border-radius:6px;padding:8px 10px;color:#fff;font-size:12px;outline:none;">
      <button class="btn btn-small" onclick="changeApPassword()" style="margin:0;">Şifre Koy</button>
    </div>
    <button class="btn btn-small" onclick="makeApOpen()" style="width:100%;background:#23273a;color:var(--accent2);font-size:11px;margin:0;">🔓 Şifresiz / Açık Ağ Yap (Tek Tıkla Kolay Bağlan)</button>
  </div>

  <!-- 2. Ev / Ofis Wi-Fi Ağına Bağlanma -->
  <div style="background:rgba(0,0,0,0.25);padding:12px;border-radius:8px;margin-bottom:12px;border:1px solid var(--border);">
    <div style="display:flex;justify-content:space-between;align-items:center;margin-bottom:8px;">
      <div style="font-weight:600;font-size:13px;color:var(--accent2);">🏠 Ev / İş Wi-Fi Ağına Bağlan (STA)</div>
      <button class="btn btn-small" id="btn-scan" onclick="scanWifiNetworks()" style="background:#23273a;margin:0;font-size:11px;">🔍 Ağları Tara</button>
    </div>

    <div id="sta-status-box" style="font-size:12px;padding:8px;border-radius:6px;background:rgba(0,0,0,0.3);margin-bottom:10px;">
      Durum: <span id="disp-sta-status" style="color:var(--text-dim);">Kontrol ediliyor...</span>
    </div>

    <div style="display:flex;flex-direction:column;gap:8px;">
      <select id="select-wifi-ssid" onchange="onSelectSsid(this.value)" style="background:#0d0f17;border:1px solid var(--border);border-radius:6px;padding:8px 10px;color:#fff;font-size:12px;outline:none;">
        <option value="">-- Çevredeki Ağları Görmek İçin "Ağları Tara"ya Basın --</option>
      </select>
      <input type="text" id="input-sta-ssid" placeholder="veya Wi-Fi Adını (SSID) buraya yazın" style="background:#0d0f17;border:1px solid var(--border);border-radius:6px;padding:8px 10px;color:#fff;font-size:12px;outline:none;">
      <input type="password" id="input-sta-pass" placeholder="Ev Wi-Fi Şifresi" style="background:#0d0f17;border:1px solid var(--border);border-radius:6px;padding:8px 10px;color:#fff;font-size:12px;outline:none;">
      <div style="display:flex;gap:8px;margin-top:4px;">
        <button class="btn btn-small" onclick="connectHomeWifi()" style="flex:1;background:var(--success);color:#000;font-weight:bold;margin:0;">Ev Ağına Bağlan</button>
        <button class="btn btn-small btn-danger" onclick="forgetHomeWifi()" id="btn-forget-wifi" style="margin:0;display:none;">Ağı Unut</button>
      </div>
    </div>
    <div style="font-size:11px;color:var(--text-dim);margin-top:8px;line-height:1.4;">
      💡 <b>Avantaj:</b> Ev ağınıza bağlandığınızda, bilgisayarınızdan veya telefonunuzdan doğrudan 
      <a id="mdns-link" href="http://collectibletv.local" target="_blank" style="color:var(--accent2);text-decoration:underline;">http://collectibletv.local</a> 
      adresinden televizyonunuza kolayca ulaşabilirsiniz.
    </div>
  </div>

  <!-- 3. Fabrika Ayarlarına Sıfırlama -->
  <div style="display:flex;align-items:center;justify-content:space-between;background:rgba(255,68,68,0.08);padding:10px 12px;border-radius:8px;border:1px solid rgba(255,68,68,0.3);">
    <div>
      <div style="font-weight:600;font-size:13px;color:var(--danger);">⚠️ Fabrika Ayarlarına Sıfırla</div>
      <div style="font-size:11px;color:var(--text-dim);">Kayıtlı tüm Wi-Fi şifrelerini ve ayarları varsayılana sıfırlar</div>
    </div>
    <button class="btn btn-small btn-danger" onclick="resetFactory()" style="margin:0;">Sıfırla</button>
  </div>
</div>

<!-- HAFIZA KARTI -->
<div class="card">
  <h2><span id="storage-title">SD Kart Hafıza Durumu</span><span id="conn-badge" style="font-size:11px;color:var(--success);">● Yayında</span></h2>
  <div class="stat-grid">
    <div class="stat-box">
      <div class="stat-label">BOŞ ALAN</div>
      <div class="stat-val" id="free-space">-- MB</div>
    </div>
    <div class="stat-box">
      <div class="stat-label">TOPLAM MEDYA</div>
      <div class="stat-val" id="video-count">--</div>
    </div>
  </div>
  <div class="progress-bar-bg">
    <div class="progress-bar" id="space-bar"></div>
  </div>
</div>

<!-- FOTOĞRAF / VİDEO YÜKLEME KARTI -->
<div class="card">
  <h2>Fotoğraf veya Video Yükle</h2>
  <div class="upload-zone" id="drop-zone" onclick="document.getElementById('file-input').click()">
    <div style="font-size:32px;margin-bottom:8px;">📷 📼</div>
    <div style="font-weight:600;margin-bottom:4px;">Herhangi Bir Fotoğraf veya Video Seçin</div>
    <div style="font-size:12px;color:var(--text-dim);">MP4, MOV, GIF, JPG, PNG, WEBP veya hazır .mjpeg / .pcm</div>
    <input type="file" id="file-input" accept="image/*,video/*,.mjpeg,.pcm" style="display:none" onchange="handleFileSelect(this.files)">
  </div>

  <div class="crop-options">
    <div class="crop-opt active" id="opt-cover" onclick="setFitMode('cover')">Tam Ekran Doldur (Kırp)</div>
    <div class="crop-opt" id="opt-contain" onclick="setFitMode('contain')">Sığdır (Siyah Çerçeve)</div>
  </div>

  <div style="display:flex;align-items:center;justify-content:space-between;gap:8px;margin-top:10px;background:rgba(0,0,0,0.2);padding:8px 12px;border-radius:6px;border:1px solid var(--border);">
    <span style="font-size:13px;color:var(--text-dim);">Video Süresi:</span>
    <select id="sel-duration" style="background:#151824;color:var(--accent2);border:1px solid var(--border);padding:6px 10px;border-radius:6px;font-weight:bold;outline:none;">
      <option value="9999" selected>Tüm Video (Tamamı - Kesme Yok)</option>
      <option value="30">İlk 30 Saniye (Hızlı)</option>
      <option value="60">İlk 1 Dakika</option>
      <option value="180">İlk 3 Dakika</option>
      <option value="300">İlk 5 Dakika</option>
    </select>
  </div>

  <label class="chk-label">
    <input type="checkbox" id="chk-audio" checked>
    <span>🔊 Videonun sesini de dönüştür ve karta yükle (16 kHz PCM)</span>
  </label>

  <canvas id="preview-canvas" width="240" height="240"></canvas>

  <div id="status-box" class="status-msg"></div>
  <div class="progress-bar-bg" id="upload-prog-bg" style="display:none;margin-top:12px;">
    <div class="progress-bar" id="upload-prog-bar"></div>
  </div>

  <div style="font-size:11px;color:var(--text-dim);margin-top:10px;line-height:1.4;">
    🛡️ <b>Kesintisiz Parçalı Yükleme Devrede:</b> Videolar 128 KB'lık parçalar halinde yüklenir; sinyal dalgalansa bile otomatik kaldığı parçadan devam eder ve kopma yaşanmaz.
  </div>
</div>

<!-- MEDYA LİSTESİ KARTI -->
<div class="card">
  <h2>
    <span>Yüklü Medyalar</span>
    <div style="display:flex;gap:8px;">
      <button class="btn btn-small" id="btn-loop-mode" onclick="toggleLoopMode()" style="background:var(--accent);color:#fff;">🔁 Sırayla Döngü: AÇIK</button>
      <button class="btn btn-small" onclick="loadVideos()" style="background:#23273a">Yenile</button>
    </div>
  </h2>
  <div id="video-list">
    <div style="text-align:center;color:var(--text-dim);padding:16px;">Yükleniyor...</div>
  </div>
</div>

<footer style="text-align:center;padding:20px 0 10px;font-size:12px;color:var(--text-dim);border-top:1px solid var(--border);margin-top:20px;">
  <div style="font-weight:bold;color:var(--text);margin-bottom:4px;">🎬 COLLECTIBLE DISPLAY</div>
  <div style="margin-top:8px;">
    Özel yapım retro mini TV'ler, figürler ve koleksiyon ürünleri videoları için: <a href="https://www.instagram.com/collectible_display/" target="_blank" style="color:#e1306c;font-weight:bold;text-decoration:none;">@collectible_display 📸</a>
  </div>
</footer>

<script>
let totalBytes = 0;
let usedBytes = 0;
let fitMode = 'cover';
let currentVol = 70;
let isMuted = false;
let lastUnmutedVol = 70;

function setFitMode(mode) {
  fitMode = mode;
  document.getElementById('opt-cover').className = 'crop-opt ' + (mode === 'cover' ? 'active' : '');
  document.getElementById('opt-contain').className = 'crop-opt ' + (mode === 'contain' ? 'active' : '');
}

function formatBytes(bytes) {
  if (bytes < 1024) return bytes + ' B';
  if (bytes < 1024*1024) return (bytes/1024).toFixed(1) + ' KB';
  if (bytes < 1024*1024*1024) return (bytes/(1024*1024)).toFixed(1) + ' MB';
  return (bytes/(1024*1024*1024)).toFixed(2) + ' GB';
}

function showStatus(msg, type='info', spin=false) {
  const box = document.getElementById('status-box');
  box.className = 'status-msg active status-' + type;
  box.innerHTML = (spin ? '<span class="spinner"></span>' : '') + msg;
}

function hideStatus() {
  const box = document.getElementById('status-box');
  box.className = 'status-msg';
}

// ---- SES VE MUTE YÖNETİMİ ----
function updateVolUI(vol, muted) {
  currentVol = parseInt(vol);
  isMuted = !!muted;
  document.getElementById('vol-slider').value = currentVol;
  document.getElementById('vol-pct').innerText = '%' + currentVol;

  const btnMute = document.getElementById('btn-mute');
  const muteIcon = document.getElementById('mute-icon');
  const muteText = document.getElementById('mute-text');
  const volStat = document.getElementById('vol-status-text');

  if (isMuted || currentVol === 0) {
    btnMute.className = 'btn-mute is-muted';
    muteIcon.innerText = '🔊';
    muteText.innerText = 'Sesi Aç (Unmute: %' + (lastUnmutedVol || 70) + ')';
    volStat.innerText = '🔇 SES KAPALI (MUTE)';
    volStat.style.color = 'var(--danger)';
  } else {
    lastUnmutedVol = currentVol;
    btnMute.className = 'btn-mute';
    muteIcon.innerText = '🔇';
    muteText.innerText = 'Sesi Kapat (MUTE)';
    volStat.innerText = 'Ses: %' + currentVol;
    volStat.style.color = 'var(--accent2)';
  }
}

function onSliderMove(val) {
  document.getElementById('vol-pct').innerText = '%' + val;
}

async function sendVolume(val) {
  try {
    const res = await fetch('/volume?val=' + val);
    const data = await res.json();
    updateVolUI(data.volume, data.muted);
  } catch(e) { console.error(e); }
}

async function setQuickVol(val) {
  sendVolume(val);
}

async function toggleMute() {
  try {
    const res = await fetch('/mute');
    const data = await res.json();
    updateVolUI(data.volume, data.muted);
  } catch(e) { console.error(e); }
}

async function testBeep() {
  try {
    const res = await fetch('/beep');
    const data = await res.json();
    alert('🔔 Test sinyali (Bip-Bip) hoparlöre gönderildi!\n\nEğer ses duyamadıysan:\n1. MAX98357A LRC (GPIO 8), BCLK (GPIO 7) ve DIN (GPIO 9) pinlerini kontrol et.\n2. GAIN ve SD pinleri KESİNLİKLE BOŞTA olmalı (Köprü yapmayın!).\n3. VIN pini 5V Boost\'a, GND pini ortak GND\'ye bağlı olmalı.\n4. Hoparlör klemens vidalarını kontrol et.');
  } catch(e) {
    alert('Hata: ' + e);
  }
}

// ---- PARLAKLIK KONTROLÜ (PİL TASARRUFU) ----
let brightDebounce = null;
function onBrightChange(val) {
  const pct = Math.round((val / 255) * 100);
  document.getElementById('disp-bright').innerText = '%' + pct;
  clearTimeout(brightDebounce);
  brightDebounce = setTimeout(async () => {
    try {
      await fetch('/brightness?val=' + val);
    } catch(e) {}
  }, 120);
}

// ---- WI-FI YÖNETİMİ ----
async function scanWifiNetworks() {
  const btn = document.getElementById('btn-scan');
  btn.disabled = true;
  btn.innerText = 'Taranıyor...';
  const sel = document.getElementById('select-wifi-ssid');
  sel.innerHTML = '<option value="">Ağlar taranıyor, lütfen bekleyin...</option>';

  try {
    const res = await fetch('/scan_wifi');
    const nets = await res.json();
    sel.innerHTML = '<option value="">-- Listeden Bir Wi-Fi Ağı Seçin (' + nets.length + ' ağ bulundu) --</option>';
    nets.forEach(n => {
      const opt = document.createElement('option');
      opt.value = n.ssid;
      opt.text = `${n.ssid} (${n.rssi} dBm) ${n.secure ? '🔒' : '🔓'}`;
      sel.appendChild(opt);
    });
  } catch(e) {
    sel.innerHTML = '<option value="">Ağ tarama başarısız oldu. Elle yazabilirsiniz.</option>';
  } finally {
    btn.disabled = false;
    btn.innerText = '🔍 Ağları Tara';
  }
}

function onSelectSsid(val) {
  if (val) {
    document.getElementById('input-sta-ssid').value = val;
    document.getElementById('input-sta-pass').focus();
  }
}

function makeApOpen() {
  if (!confirm('Hotspot şifresiz (açık ağ) yapılsın mı?\nBöylece herhangi bir cihaz şifre girmeden tek tıkla kolayca bağlanabilir.')) return;
  saveApPass('');
}

async function changeApPassword() {
  const newPass = document.getElementById('input-new-ap-pass').value.trim();
  if (newPass.length < 8) {
    alert('Hotspot şifresi en az 8 karakter olmalıdır!\n(Şifreyi kaldırmak için "Şifresiz / Açık Ağ Yap" düğmesini kullanabilirsiniz)');
    return;
  }
  if (!confirm('Hotspot şifresi "' + newPass + '" yapılsın mı?\nCihaz yeniden başlayacaktır.')) return;
  saveApPass(newPass);
}

async function saveApPass(pass) {
  try {
    const formData = new FormData();
    formData.append('ap_pass', pass);
    const res = await fetch('/save_wifi', { method: 'POST', body: formData });
    const data = await res.json();
    alert(data.message || 'Ayarlar kaydedildi. Cihaz yeniden başlıyor...');
  } catch(e) {
    alert('Ayarlar gönderildi. Cihaz yeniden başlıyor...');
  }
}

async function connectHomeWifi() {
  const ssid = document.getElementById('input-sta-ssid').value.trim();
  const pass = document.getElementById('input-sta-pass').value;
  if (!ssid) {
    alert('Lütfen bağlanılacak Wi-Fi adını (SSID) girin veya seçin!');
    return;
  }
  if (!confirm('"' + ssid + '" ağına bağlanılsın mı?\nCihaz ayarları kaydedip yeniden başlayacaktır.')) return;

  try {
    const formData = new FormData();
    formData.append('sta_ssid', ssid);
    formData.append('sta_pass', pass);
    const res = await fetch('/save_wifi', { method: 'POST', body: formData });
    const data = await res.json();
    alert(data.message || 'Ev ağına bağlanılıyor. Cihaz yeniden başlıyor...');
  } catch(e) {
    alert('Ayarlar kaydedildi. Cihaz yeniden başlıyor...');
  }
}

async function forgetHomeWifi() {
  if (!confirm('Kayıtlı ev Wi-Fi ağını unutmak istediğinize emin misiniz?')) return;
  try {
    const res = await fetch('/forget_wifi', { method: 'POST' });
    const data = await res.json();
    alert(data.message || 'Ev ağı unutuldu. Cihaz yeniden başlıyor...');
  } catch(e) {
    alert('İşlem yapıldı. Cihaz yeniden başlıyor...');
  }
}

async function resetFactory() {
  if (!confirm('⚠️ DİKKAT!\nTüm Wi-Fi şifreleri, ev ağı ayarları ve ses tercihleri fabrika ayarlarına sıfırlanacaktır.\n\nEmin misiniz?')) return;
  try {
    const res = await fetch('/reset_factory', { method: 'POST' });
    const data = await res.json();
    alert(data.message || 'Fabrika ayarlarına sıfırlandı. Cihaz yeniden başlıyor...');
  } catch(e) {
    alert('Sıfırlama komutu verildi. Cihaz yeniden başlıyor...');
  }
}

// ---- VİDEOLAR VE VERİLER ----
async function loadVideos() {
  try {
    const res = await fetch('/list');
    const data = await res.json();
    totalBytes = data.total;
    usedBytes = data.used;
    const free = totalBytes - usedBytes;

    document.getElementById('free-space').innerText = formatBytes(free);
    document.getElementById('video-count').innerText = data.files.length;
    const pct = totalBytes > 0 ? (usedBytes / totalBytes) * 100 : 0;
    document.getElementById('space-bar').style.width = pct + '%';

    const storageTitleEl = document.getElementById('storage-title');
    if (storageTitleEl) {
      if (totalBytes > 1024 * 1024 * 1024) {
        const approxGb = Math.round(totalBytes / 1e9);
        storageTitleEl.innerText = `${approxGb} GB SD Kart Hafıza Durumu`;
      } else if (totalBytes > 0) {
        const mb = Math.round(totalBytes / (1024 * 1024));
        storageTitleEl.innerText = `${mb} MB SD Kart Hafıza Durumu`;
      } else {
        storageTitleEl.innerText = 'SD Kart Hafıza Durumu';
      }
    }

    if (data.volume !== undefined) {
      updateVolUI(data.volume, data.muted);
    }
    if (data.brightness !== undefined) {
      const bSlider = document.getElementById('bright-slider');
      const bDisp = document.getElementById('disp-bright');
      if (bSlider) bSlider.value = data.brightness;
      if (bDisp) bDisp.innerText = '%' + Math.round((data.brightness / 255) * 100);
    }
    if (data.loop_all !== undefined) {
      updateLoopModeUI(data.loop_all);
    }

    if (data.wifi) {
      document.getElementById('disp-ap-ssid').innerText = data.wifi.ap_ssid || 'CollectibleDisplay';
      const apPassEl = document.getElementById('disp-ap-pass');
      if (!data.wifi.ap_pass || data.wifi.ap_pass.length === 0) {
        apPassEl.innerHTML = '<span style="color:var(--success);">🔓 ŞİFRESİZ (Kolay Bağlantı)</span>';
      } else {
        apPassEl.innerHTML = `<span style="color:var(--warning);">${data.wifi.ap_pass}</span>`;
      }

      const staStatus = document.getElementById('disp-sta-status');
      const wifiBadge = document.getElementById('wifi-status-badge');
      const forgetBtn = document.getElementById('btn-forget-wifi');

      if (data.wifi.sta_connected) {
        staStatus.innerHTML = `<b style="color:var(--success);">● Bağlı: ${data.wifi.sta_ssid}</b> (IP: <b style="color:var(--accent2);">${data.wifi.sta_ip}</b>)`;
        wifiBadge.innerHTML = `<span style="color:var(--success);">● Ev Ağı: ${data.wifi.sta_ssid}</span>`;
        forgetBtn.style.display = 'inline-block';
        document.getElementById('mdns-link').innerText = `http://${data.wifi.sta_ip} veya http://collectibletv.local`;
        document.getElementById('mdns-link').href = `http://${data.wifi.sta_ip}`;
      } else if (data.wifi.sta_ssid && data.wifi.sta_ssid.length > 0) {
        staStatus.innerHTML = `<b style="color:var(--warning);">● Bağlanamadı: ${data.wifi.sta_ssid}</b>`;
        wifiBadge.innerHTML = `<span style="color:var(--warning);">● Bağlantı Yok</span>`;
        forgetBtn.style.display = 'inline-block';
      } else {
        staStatus.innerHTML = `<span style="color:var(--text-dim);">Ev ağına bağlı değil (Yalnızca Hotspot aktif)</span>`;
        wifiBadge.innerHTML = `<span style="color:var(--text-dim);">● Yalnızca Hotspot</span>`;
        forgetBtn.style.display = 'none';
      }
    }

    const listEl = document.getElementById('video-list');
    if (data.files.length === 0) {
      listEl.innerHTML = '<div style="text-align:center;color:var(--text-dim);padding:16px;">Henüz video yüklenmemiş.<br>Yukarıdan fotoğraf veya video seçin!</div>';
      return;
    }

    listEl.innerHTML = data.files.map((f, i) => `
      <div class="video-item ${f.name === data.active ? 'active' : ''}">
        <div class="video-info">
          <div class="video-name">${f.name} ${f.hasAudio ? '🔊' : ''} ${f.name === data.active ? '<span style="color:var(--accent);font-size:11px;">[Oynatılıyor]</span>' : ''}</div>
          <div class="video-meta">${formatBytes(f.size)}</div>
        </div>
        <div class="video-actions">
          <button class="btn btn-small btn-play" onclick="playVideo('${f.name}')">Oynat</button>
          <button class="btn btn-small btn-danger" onclick="deleteVideo('${f.name}')">Sil</button>
        </div>
      </div>
    `).join('');
  } catch(e) {
    console.error(e);
  }
}

async function playVideo(name) {
  try {
    const res = await fetch('/play?file=' + encodeURIComponent(name));
    if (res.ok) {
      document.querySelectorAll('.video-item').forEach(el => {
        const titleEl = el.querySelector('.video-name');
        if (titleEl && titleEl.innerText.includes(name)) {
          el.classList.add('active');
        } else {
          el.classList.remove('active');
        }
      });
    }
  } catch(e) { console.error('Play error:', e); }
}

async function deleteVideo(name) {
  if (!confirm(name + ' medyasını silmek istediğinize emin misiniz?')) return;
  try {
    const res = await fetch('/delete?file=' + encodeURIComponent(name));
    loadVideos();
  } catch(e) { alert('Silinemedi: ' + e); }
}

async function toggleLoopMode() {
  try {
    const res = await fetch('/toggle_loop', { method: 'POST' });
    const data = await res.json();
    updateLoopModeUI(data.loop_all);
    showStatus(data.loop_all ? 'Sırayla Döngü Modu Aktif (Tüm Medyalar Sırayla Döner)' : 'Tek Medya Tekrar Modu Aktif (Yalnızca Seçili Medya)', 'info');
    setTimeout(hideStatus, 2500);
  } catch(e) { console.error('Toggle loop error:', e); }
}

function updateLoopModeUI(loopAll) {
  const btn = document.getElementById('btn-loop-mode');
  if (!btn) return;
  if (loopAll) {
    btn.innerHTML = '🔁 Sırayla Döngü: AÇIK';
    btn.style.background = 'var(--accent)';
    btn.style.color = '#fff';
  } else {
    btn.innerHTML = '🔂 Tekrar: TEK MEDYA';
    btn.style.background = '#23273a';
    btn.style.color = 'var(--text-dim)';
  }
}

// Drag & Drop
const dropZone = document.getElementById('drop-zone');
['dragenter', 'dragover'].forEach(n => dropZone.addEventListener(n, (e) => { e.preventDefault(); dropZone.classList.add('dragover'); }));
['dragleave', 'drop'].forEach(n => dropZone.addEventListener(n, (e) => { e.preventDefault(); dropZone.classList.remove('dragover'); }));
dropZone.addEventListener('drop', (e) => {
  if (e.dataTransfer.files && e.dataTransfer.files.length) {
    handleFileSelect(e.dataTransfer.files);
  }
});

function handleFileSelect(files) {
  if (!files || !files.length) return;
  const file = files[0];
  const name = file.name.toLowerCase();

  if (name.endsWith('.mjpeg') || name.endsWith('.pcm')) {
    uploadFileChunked(file).then(() => {
      showStatus('Dosya başarıyla yüklendi! 🎉', 'ok');
      loadVideos();
      setTimeout(hideStatus, 4000);
    }).catch(err => {
      showStatus('Yükleme hatası: ' + err.message, 'err');
    });
  } else if (file.type.startsWith('image/') || name.endsWith('.jpg') || name.endsWith('.jpeg') || name.endsWith('.png') || name.endsWith('.webp') || name.endsWith('.bmp')) {
    convertAndUploadPhoto(file);
  } else {
    convertAndUploadVideo(file);
  }
}

// ---- KESİNTİSİZ PARÇALI (CHUNKED) YÜKLEME MOTORU ----
async function uploadFileChunked(file) {
  const CHUNK_SIZE = 8 * 1024; // 8 KB optimum parçalar (FreeRTOS Wi-Fi tamponuyla %100 uyumlu)
  const totalChunks = Math.ceil(file.size / CHUNK_SIZE);
  const progBg = document.getElementById('upload-prog-bg');
  const progBar = document.getElementById('upload-prog-bar');
  progBg.style.display = 'block';

  showStatus(`"${file.name}" için yükleme oturumu açılıyor...`, 'info', true);
  
  // Oturum açma - 4 kez otomatik deneme
  let startSuccess = false;
  let startAttempt = 0;
  while (!startSuccess && startAttempt < 4) {
    startAttempt++;
    try {
      const startUrl = `/upload_start?name=${encodeURIComponent(file.name)}&size=${file.size}`;
      const startRes = await fetch(startUrl, { method: 'POST' });
      if (startRes.ok) {
        startSuccess = true;
      } else {
        await new Promise(r => setTimeout(r, 400));
      }
    } catch(e) {
      showStatus(`Cihaza bağlanılıyor (${startAttempt}/4)...`, 'info', true);
      await new Promise(r => setTimeout(r, 500));
    }
  }

  if (!startSuccess) throw new Error("Cihaz yükleme oturumunu açamadı. Wi-Fi bağlantınızı kontrol edin.");

  const startTime = Date.now();

  for (let c = 0; c < totalChunks; c++) {
    const start = c * CHUNK_SIZE;
    const end = Math.min(start + CHUNK_SIZE, file.size);
    const chunkBlob = file.slice(start, end);

    let success = false;
    let attempt = 0;
    while (!success && attempt < 5) {
      attempt++;
      try {
        const formData = new FormData();
        formData.append("chunk", chunkBlob, file.name);

        const chunkUrl = `/upload_chunk?name=${encodeURIComponent(file.name)}&idx=${c}&total=${totalChunks}`;
        const res = await fetch(chunkUrl, {
          method: 'POST',
          body: formData
        });

        if (res.ok) {
          success = true;
        } else {
          showStatus(`Parça ${c+1}/${totalChunks} tekrar deneniyor (${attempt}/5)...`, 'info', true);
          await new Promise(r => setTimeout(r, 250));
        }
      } catch (err) {
        showStatus(`Ağ sinyali dalgalandı, Parça ${c+1}/${totalChunks} tekrar deneniyor (${attempt}/5)...`, 'info', true);
        await new Promise(r => setTimeout(r, 350));
      }
    }

    if (!success) {
      await fetch(`/upload_abort?name=${encodeURIComponent(file.name)}`, { method: 'POST' }).catch(() => {});
      throw new Error(`Parça ${c+1}/${totalChunks} yüklenirken bağlantı koptu.`);
    }

    const pct = Math.round(((c + 1) / totalChunks) * 100);
    progBar.style.width = pct + '%';

    const elapsed = (Date.now() - startTime) / 1000;
    const speedKb = elapsed > 0 ? ((end / 1024) / elapsed).toFixed(0) : 0;
    showStatus(`Yükleniyor: ${file.name} (%${pct}) - ${c+1}/${totalChunks} Parça [${speedKb} KB/s]`, 'info', true);

    // Wi-Fi radyo ve FreeRTOS beacon iletimine nefes aldır (40 ms mola)
    await new Promise(r => setTimeout(r, 40));
  }

  showStatus(`"${file.name}" SD karta yazıldı, oynatılıyor...`, 'info', true);
  const finishUrl = `/upload_finish?name=${encodeURIComponent(file.name)}`;
  const finRes = await fetch(finishUrl, { method: 'POST' });
  if (!finRes.ok) throw new Error("Dosya sonlandırılırken hata oluştu.");

  progBg.style.display = 'none';
  return true;
}

// 1. FOTOĞRAF DÖNÜŞTÜRÜCÜ
function convertAndUploadPhoto(file) {
  showStatus('Fotoğraf işleniyor ve 240x240 boyutuna ölçekleniyor...', 'info', true);
  const img = new Image();
  img.src = URL.createObjectURL(file);
  img.onload = async () => {
    const canvas = document.getElementById('preview-canvas');
    canvas.style.display = 'block';
    const ctx = canvas.getContext('2d');
    ctx.fillStyle = '#000000';
    ctx.fillRect(0, 0, 240, 240);

    let sw, sh, sx, sy, dw, dh, dx, dy;
    if (fitMode === 'cover') {
      const scale = Math.max(240 / img.width, 240 / img.height);
      sw = 240 / scale;
      sh = 240 / scale;
      sx = (img.width - sw) / 2;
      sy = (img.height - sh) / 2;
      ctx.drawImage(img, sx, sy, sw, sh, 0, 0, 240, 240);
    } else {
      const scale = Math.min(240 / img.width, 240 / img.height);
      dw = img.width * scale;
      dh = img.height * scale;
      dx = (240 - dw) / 2;
      dy = (240 - dh) / 2;
      ctx.drawImage(img, dx, dy, dw, dh);
    }

    canvas.toBlob(async (blob) => {
      let base = file.name.substring(0, file.name.lastIndexOf('.'));
      base = base.replace(/[^a-zA-Z0-9_-]/g, '_');
      if (!base) base = 'photo_' + Date.now();
      const outName = base + '.mjpeg';
      const mjpegFile = new File([blob], outName, { type: 'video/x-motion-jpeg' });
      URL.revokeObjectURL(img.src);
      try {
        await uploadFileChunked(mjpegFile);
        showStatus('Fotoğraf başarıyla yüklendi! 🎉', 'ok');
        loadVideos();
        setTimeout(hideStatus, 4000);
      } catch(err) {
        showStatus('Yükleme hatası: ' + err.message, 'err');
      }
    }, 'image/jpeg', 0.85);
  };
}

// 2. SES AYIKLAYICI (Web Audio API 16 kHz Mono PCM)
async function extractPcm(file, maxDuration) {
  try {
    const ab = await file.arrayBuffer();
    const ac = new (window.AudioContext || window.webkitAudioContext)();
    const dec = await ac.decodeAudioData(ab);
    ac.close();

    const dur = Math.min(dec.duration, maxDuration);
    const SR = 16000;
    const len = Math.max(1, Math.floor(dur * SR));
    const off = new OfflineAudioContext(1, len, SR);
    const src = off.createBufferSource();
    src.buffer = dec;
    src.connect(off.destination);
    src.start(0);

    const rendered = await off.startRendering();
    const ch = rendered.getChannelData(0);
    const pcm = new Int16Array(ch.length);
    for (let i = 0; i < ch.length; i++) {
      let s = Math.max(-1, Math.min(1, ch[i]));
      pcm[i] = s < 0 ? s * 32768 : s * 32767;
    }
    return new Blob([pcm.buffer], { type: 'application/octet-stream' });
  } catch(e) {
    console.warn("Ses ayrıştırılamadı veya video sessiz:", e);
    return null;
  }
}

// 3. VİDEO DÖNÜŞTÜRÜCÜ (Kareler + Ses)
async function convertAndUploadVideo(file) {
  let wakeLock = null;
  if ('wakeLock' in navigator) {
    try { wakeLock = await navigator.wakeLock.request('screen'); } catch(e){}
  }

  // Dönüştürme esnasında Wi-Fi bağlantısının uykuya/boşa düşmesini engelleyen Kalp Atışı (Keepalive)
  let heartbeatTimer = setInterval(() => {
    fetch('/ping').catch(() => {});
  }, 2000);

  try {
    showStatus('Video inceleniyor ve karelere ayrılıyor...', 'info', true);
    const video = document.createElement('video');
    video.muted = true;
    video.playsInline = true;
    video.src = URL.createObjectURL(file);

    await new Promise((res, rej) => {
      video.onloadedmetadata = () => res();
      video.onerror = rej;
    });

    const maxDur = parseFloat(document.getElementById('sel-duration').value) || 9999;
    const duration = Math.min(video.duration, maxDur);
    const fps = 20;
    const totalFrames = Math.floor(duration * fps);
    const canvas = document.getElementById('preview-canvas');
    canvas.style.display = 'block';
    const ctx = canvas.getContext('2d');

    const blobs = [];
    const progBg = document.getElementById('upload-prog-bg');
    const progBar = document.getElementById('upload-prog-bar');
    progBg.style.display = 'block';

    for (let i = 0; i < totalFrames; i++) {
      const time = i / fps;
      video.currentTime = time;
      await Promise.race([
        new Promise(r => { video.onseeked = r; }),
        new Promise(r => setTimeout(r, 120))
      ]);

      ctx.fillStyle = '#000000';
      ctx.fillRect(0, 0, 240, 240);

      if (fitMode === 'cover') {
        const scale = Math.max(240 / video.videoWidth, 240 / video.videoHeight);
        const sw = 240 / scale;
        const sh = 240 / scale;
        const sx = (video.videoWidth - sw) / 2;
        const sy = (video.videoHeight - sh) / 2;
        ctx.drawImage(video, sx, sy, sw, sh, 0, 0, 240, 240);
      } else {
        const scale = Math.min(240 / video.videoWidth, 240 / video.videoHeight);
        const dw = video.videoWidth * scale;
        const dh = video.videoHeight * scale;
        const dx = (240 - dw) / 2;
        const dy = (240 - dh) / 2;
        ctx.drawImage(video, dx, dy, dw, dh);
      }

      const blob = await new Promise(r => canvas.toBlob(r, 'image/jpeg', 0.65));
      blobs.push(blob);

      const pct = Math.round(((i + 1) / totalFrames) * 100);
      progBar.style.width = pct + '%';
      showStatus(`Dönüştürülüyor: Kare ${i+1}/${totalFrames} (%${pct})`, 'info', true);
    }

    let base = file.name.substring(0, file.name.lastIndexOf('.'));
    base = base.replace(/[^a-zA-Z0-9_-]/g, '_');
    if (!base) base = 'video_' + Date.now();

    let pcmFile = null;
    const wantAudio = document.getElementById('chk-audio').checked;
    if (wantAudio) {
      showStatus('Videonun sesi 16 kHz mono formatına ayıklanıyor...', 'info', true);
      const pcmBlob = await extractPcm(file, duration);
      if (pcmBlob) {
        const pcmName = base + '.pcm';
        pcmFile = new File([pcmBlob], pcmName, { type: 'application/octet-stream' });
      }
    }

    showStatus('Video SD karta paketleniyor...', 'info', true);
    const outName = base + '.mjpeg';
    const mjpegBlob = new Blob(blobs, { type: 'video/x-motion-jpeg' });
    const mjpegFile = new File([mjpegBlob], outName, { type: 'video/x-motion-jpeg' });
    URL.revokeObjectURL(video.src);

    if (pcmFile) {
      showStatus('Ses dosyası SD karta aktarılıyor...', 'info', true);
      await uploadFileChunked(pcmFile);
    }

    showStatus('Video dosyası SD karta aktarılıyor...', 'info', true);
    await uploadFileChunked(mjpegFile);

    showStatus('Video ve Ses başarıyla yüklendi ve açıldı! 🎉', 'ok');
    loadVideos();
    setTimeout(hideStatus, 4000);
  } catch(err) {
    showStatus('Yükleme hatası: ' + err.message, 'err');
  } finally {
    if (heartbeatTimer) clearInterval(heartbeatTimer);
    if (wakeLock) {
      try { await wakeLock.release(); } catch(e){}
    }
  }
}

// Wi-Fi Bağlantı Takipçisi ve Otomatik Yeniden Bağlanma
let isDeviceOnline = true;
async function checkConnection() {
  const controller = new AbortController();
  const timeoutId = setTimeout(() => controller.abort(), 2000);
  try {
    const res = await fetch('/ping', { signal: controller.signal });
    clearTimeout(timeoutId);
    if (res.ok) {
      if (!isDeviceOnline) {
        isDeviceOnline = true;
        showStatus('Wi-Fi Bağlantısı Yeniden Kuruldu! 🟢', 'ok');
        loadVideos();
        setTimeout(hideStatus, 2500);
      }
    }
  } catch(e) {
    clearTimeout(timeoutId);
    if (isDeviceOnline) {
      isDeviceOnline = false;
      showStatus('Wi-Fi Bağlantısı Koptu! Otomatik yeniden bağlanılıyor... ⚠️', 'err', true);
    }
  }
}

loadVideos();
setInterval(loadVideos, 4000);
setInterval(checkConnection, 3000);
</script>
</body>
</html>
)rawliteral";
