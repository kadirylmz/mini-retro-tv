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
  padding: 16px 0;
  border-bottom: 1px solid var(--border);
  margin-bottom: 20px;
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
  <div class="subtitle">1.3" IPS Yatay Ekran - Medya, Wi-Fi & Ses Paneli</div>
</header>

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
</div>

<!-- WI-FI & AĞ AYARLARI KARTI -->
<div class="card">
  <h2>
    <span>📶 Wi-Fi & Ağ Ayarları</span>
    <span id="wifi-status-badge" style="font-size:11px;color:var(--text-dim);">Kontrol ediliyor...</span>
  </h2>

  <!-- 1. Hotspot Şifresi Değiştirme -->
  <div style="background:rgba(0,0,0,0.25);padding:12px;border-radius:8px;margin-bottom:12px;border:1px solid var(--border);">
    <div style="font-weight:600;font-size:13px;color:var(--accent2);margin-bottom:8px;">📡 Cihaz Erişim Noktası (Hotspot)</div>
    <div style="display:flex;justify-content:space-between;font-size:12px;margin-bottom:6px;">
      <span style="color:var(--text-dim);">Ağ Adı (SSID):</span>
      <span id="disp-ap-ssid" style="font-weight:bold;">CollectibleDisplay</span>
    </div>
    <div style="display:flex;justify-content:space-between;font-size:12px;margin-bottom:10px;">
      <span style="color:var(--text-dim);">Mevcut Şifre:</span>
      <span id="disp-ap-pass" style="font-family:monospace;color:var(--warning);">display1234</span>
    </div>
    <div style="display:flex;gap:8px;">
      <input type="text" id="input-new-ap-pass" placeholder="Yeni Şifre (En az 8 hane)" style="flex:1;background:#0d0f17;border:1px solid var(--border);border-radius:6px;padding:8px 10px;color:#fff;font-size:12px;outline:none;">
      <button class="btn btn-small" onclick="changeApPassword()" style="margin:0;">Şifreyi Kaydet</button>
    </div>
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
      💡 <b>Avantaj:</b> Ev ağınıza bağlandığınızda, telefonunuzun Wi-Fi'ını değiştirmeden evinizin internetinden 
      <a id="mdns-link" href="http://collectibletv.local" target="_blank" style="color:var(--accent2);text-decoration:underline;">http://collectibletv.local</a> 
      adresinden doğrudan bu panele erişebilirsiniz.
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
  <h2><span>16 GB SD Kart Hafıza Durumu</span><span id="conn-badge" style="font-size:11px;color:var(--success);">● Yayında</span></h2>
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
    🛡️ <b>Kesintisiz Parçalı Yükleme Devrede:</b> Videolar 128 KB'lık parçalar halinde yüklenir; Wi-Fi sinyali dalgalansa bile otomatik olarak kaldığı parçadan devam eder ve kopma yaşanmaz.
  </div>
</div>

<!-- MEDYA LİSTESİ KARTI -->
<div class="card">
  <h2><span>Yüklü Medyalar</span><button class="btn btn-small" onclick="loadVideos()" style="background:#23273a">Yenile</button></h2>
  <div id="video-list">
    <div style="text-align:center;color:var(--text-dim);padding:16px;">Yükleniyor...</div>
  </div>
</div>

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
  return (bytes/(1024*1024)).toFixed(1) + ' MB';
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

async function changeApPassword() {
  const newPass = document.getElementById('input-new-ap-pass').value.trim();
  if (newPass.length < 8) {
    alert('Hotspot şifresi en az 8 karakter olmalıdır!');
    return;
  }
  if (!confirm('Hotspot şifresini "' + newPass + '" olarak değiştirmek istediğinize emin misiniz?\nCihaz yeniden başlayacaktır.')) return;

  try {
    const formData = new FormData();
    formData.append('ap_pass', newPass);
    const res = await fetch('/save_wifi', { method: 'POST', body: formData });
    const data = await res.json();
    alert(data.message || 'Şifre güncellendi. Yeni şifreyle bağlanın.');
  } catch(e) {
    alert('Ayarlar kaydedildi. Cihaz yeniden başlıyor...');
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

    if (data.volume !== undefined) {
      updateVolUI(data.volume, data.muted);
    }

    if (data.wifi) {
      document.getElementById('disp-ap-ssid').innerText = data.wifi.ap_ssid || 'CollectibleDisplay';
      document.getElementById('disp-ap-pass').innerText = data.wifi.ap_pass || 'display1234';

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
    await fetch('/play?file=' + encodeURIComponent(name));
    loadVideos();
  } catch(e) { alert('Hata: ' + e); }
}

async function deleteVideo(name) {
  if (!confirm(name + ' medyasını silmek istediğinize emin misiniz?')) return;
  try {
    const res = await fetch('/delete?file=' + encodeURIComponent(name));
    loadVideos();
  } catch(e) { alert('Silinemedi: ' + e); }
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
  const CHUNK_SIZE = 128 * 1024; // 128 KB parçalar
  const totalChunks = Math.ceil(file.size / CHUNK_SIZE);
  const progBg = document.getElementById('upload-prog-bg');
  const progBar = document.getElementById('upload-prog-bar');
  progBg.style.display = 'block';

  // 1. Cihaza yükleme başlatma oturumu aç
  showStatus(`"${file.name}" için yükleme oturumu açılıyor...`, 'info', true);
  const startUrl = `/upload_start?name=${encodeURIComponent(file.name)}&size=${file.size}`;
  const startRes = await fetch(startUrl, { method: 'POST' });
  if (!startRes.ok) throw new Error("Cihaz yükleme oturumunu açamadı.");

  const startTime = Date.now();

  // 2. Parça parça gönder (Her parça için otomatik 3 deneme)
  for (let c = 0; c < totalChunks; c++) {
    const start = c * CHUNK_SIZE;
    const end = Math.min(start + CHUNK_SIZE, file.size);
    const chunkBlob = file.slice(start, end);

    let success = false;
    let attempt = 0;
    while (!success && attempt < 4) {
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
          showStatus(`Parça ${c+1}/${totalChunks} tekrar deneniyor (${attempt}/3)...`, 'info', true);
          await new Promise(r => setTimeout(r, 300));
        }
      } catch (err) {
        showStatus(`Ağ sinyali dalgalandı, Parça ${c+1}/${totalChunks} tekrar deneniyor (${attempt}/3)...`, 'info', true);
        await new Promise(r => setTimeout(r, 400));
      }
    }

    if (!success) {
      await fetch(`/upload_abort?name=${encodeURIComponent(file.name)}`, { method: 'POST' });
      throw new Error(`Parça ${c+1}/${totalChunks} yüklenirken bağlantı koptu.`);
    }

    const pct = Math.round(((c + 1) / totalChunks) * 100);
    progBar.style.width = pct + '%';

    const elapsed = (Date.now() - startTime) / 1000;
    const speedKb = elapsed > 0 ? ((end / 1024) / elapsed).toFixed(0) : 0;
    showStatus(`Yükleniyor: ${file.name} (%${pct}) - ${c+1}/${totalChunks} Parça [${speedKb} KB/s]`, 'info', true);
  }

  // 3. Dosyayı tamamla ve oynatmaya başla
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
      const outName = file.name.substring(0, file.name.lastIndexOf('.')) + '.mjpeg';
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
    const fps = 20; // 20 FPS ideal
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

    // Ses ayıklama (seçiliyse)
    let pcmFile = null;
    const wantAudio = document.getElementById('chk-audio').checked;
    if (wantAudio) {
      showStatus('Videonun sesi 16 kHz mono formatına ayıklanıyor...', 'info', true);
      const pcmBlob = await extractPcm(file, duration);
      if (pcmBlob) {
        const pcmName = file.name.substring(0, file.name.lastIndexOf('.')) + '.pcm';
        pcmFile = new File([pcmBlob], pcmName, { type: 'application/octet-stream' });
      }
    }

    showStatus('Video SD karta paketleniyor...', 'info', true);
    const outName = file.name.substring(0, file.name.lastIndexOf('.')) + '.mjpeg';
    const mjpegBlob = new Blob(blobs, { type: 'video/x-motion-jpeg' });
    const mjpegFile = new File([mjpegBlob], outName, { type: 'video/x-motion-jpeg' });
    URL.revokeObjectURL(video.src);

    // Kesintisiz Parçalı Yükleme
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
    if (wakeLock) {
      try { await wakeLock.release(); } catch(e){}
    }
  }
}

loadVideos();
setInterval(loadVideos, 10000);
</script>
</body>
</html>
)rawliteral";
