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
}
.status-msg.active { display: block; }
.status-info { background: rgba(0,229,255,0.1); color: var(--accent2); }
.status-err { background: rgba(255,68,68,0.1); color: var(--danger); }
.status-ok { background: rgba(0,230,118,0.1); color: var(--success); }
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
  <div class="subtitle">1.3" IPS Yatay Ekran - Medya & Ses Paneli</div>
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
    <div style="font-size:12px;color:var(--text-dim);">MP4, MOV, GIF, JPG, PNG, WEBP — 240x240'a Otomatik Dönüştürülür</div>
    <input type="file" id="file-input" accept="image/*,video/*,.mjpeg,.pcm" style="display:none" onchange="handleFileSelect(this.files)">
  </div>

  <div class="crop-options">
    <div class="crop-opt active" id="opt-cover" onclick="setFitMode('cover')">Tam Ekran Doldur (Kırp)</div>
    <div class="crop-opt" id="opt-contain" onclick="setFitMode('contain')">Sığdır (Siyah Çerçeve)</div>
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
    uploadRawFile(file);
  } else if (file.type.startsWith('image/') || name.endsWith('.jpg') || name.endsWith('.jpeg') || name.endsWith('.png') || name.endsWith('.webp') || name.endsWith('.bmp')) {
    convertAndUploadPhoto(file);
  } else {
    convertAndUploadVideo(file);
  }
}

function uploadRawFile(file, onDone) {
  showStatus('Yükleniyor: ' + file.name, 'info', true);
  const progBg = document.getElementById('upload-prog-bg');
  const progBar = document.getElementById('upload-prog-bar');
  progBg.style.display = 'block';
  progBar.style.width = '0%';

  const formData = new FormData();
  formData.append('video', file, file.name);

  const xhr = new XMLHttpRequest();
  xhr.open('POST', '/upload', true);
  xhr.upload.onprogress = (e) => {
    if (e.lengthComputable) {
      const p = Math.round((e.loaded / e.total) * 100);
      progBar.style.width = p + '%';
      showStatus(`Yükleniyor: ${file.name} (%${p})`, 'info', true);
    }
  };
  xhr.onload = () => {
    progBg.style.display = 'none';
    if (xhr.status === 200) {
      if (onDone) onDone();
      else {
        showStatus('Medya başarıyla yüklendi! 🎉', 'ok');
        loadVideos();
        setTimeout(hideStatus, 4000);
      }
    } else {
      showStatus('Yükleme hatası: ' + xhr.responseText, 'err');
    }
  };
  xhr.onerror = () => {
    progBg.style.display = 'none';
    showStatus('Bağlantı hatası oluştu!', 'err');
  };
  xhr.send(formData);
}

// 1. FOTOĞRAF DÖNÜŞTÜRÜCÜ
function convertAndUploadPhoto(file) {
  showStatus('Fotoğraf işleniyor ve 240x240 boyutuna ölçekleniyor...', 'info', true);
  const img = new Image();
  img.src = URL.createObjectURL(file);
  img.onload = () => {
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

    canvas.toBlob((blob) => {
      const outName = file.name.substring(0, file.name.lastIndexOf('.')) + '.mjpeg';
      const mjpegFile = new File([blob], outName, { type: 'video/x-motion-jpeg' });
      URL.revokeObjectURL(img.src);
      uploadRawFile(mjpegFile);
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
  showStatus('Video inceleniyor ve karelere ayrılıyor...', 'info', true);
  const video = document.createElement('video');
  video.muted = true;
  video.playsInline = true;
  video.src = URL.createObjectURL(file);

  await new Promise((res, rej) => {
    video.onloadedmetadata = () => res();
    video.onerror = rej;
  });

  const duration = Math.min(video.duration, 45); // Maksimum 45 saniye
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
    await new Promise(r => { video.onseeked = r; });

    ctx.fillStyle = '#000000';
    ctx.fillRect(0, 0, 240, 240);

    if (fitMode === 'cover') {
      const scale = Math.max(240 / video.videoWidth, 240 / video.videoHeight);
      const sw = 240 / scale;
      const sh = 240 / scale;
      const sx = (video.videoWidth - sw) / 2;
      sy = (video.videoHeight - sh) / 2;
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

  if (pcmFile) {
    uploadRawFile(pcmFile, () => {
      uploadRawFile(mjpegFile, () => {
        showStatus('Video ve Ses başarıyla yüklendi ve açıldı! 🎉', 'ok');
        loadVideos();
        setTimeout(hideStatus, 4000);
      });
    });
  } else {
    uploadRawFile(mjpegFile, () => {
      showStatus('Video başarıyla yüklendi ve açıldı! 🎉', 'ok');
      loadVideos();
      setTimeout(hideStatus, 4000);
    });
  }
}

loadVideos();
setInterval(loadVideos, 10000);
</script>
</body>
</html>
)rawliteral";
