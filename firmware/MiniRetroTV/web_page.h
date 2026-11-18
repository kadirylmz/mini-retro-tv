#pragma once
// =============================================================
//  MiniRetroTV - Gömülü Web Arayüzü (Sesli + MicroSD Destekli)
//  Telefondan/PC'den seçilen MP4/WebM/MOV dosyalarının HEM görüntüsünü
//  (280x240 MJPEG) HEM sesini (16 kHz mono PCM) tarayıcı içinde otomatik
//  dönüştürüp Wi-Fi üzerinden TV'ye yükler. Harici program gerekmez!
// =============================================================

static const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="tr">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Mini Retro TV</title>
<style>
:root{--bg:#181412;--card:#26201c;--acc:#e05a3a;--cream:#f3e7d0;--muted:#a79882}
*{box-sizing:border-box}
body{margin:0;padding:16px;font-family:system-ui,-apple-system,sans-serif;background:var(--bg);color:var(--cream);max-width:560px;margin-inline:auto}
h1{font-size:1.35rem;margin:0 0 4px;display:flex;align-items:center;gap:8px}
.sub{color:var(--muted);font-size:.85rem;margin-bottom:14px}
.card{background:var(--card);border:1px solid #3a312b;border-radius:14px;padding:14px;margin-bottom:14px}
label{display:block;font-size:.82rem;color:var(--muted);margin:8px 0 4px}
input[type=text],input[type=number],select{width:100%;padding:8px 10px;border-radius:8px;border:1px solid #473c35;background:#151210;color:var(--cream)}
input[type=range]{width:100%;accent-color:var(--acc)}
.row{display:grid;grid-template-columns:1fr 1fr 1fr;gap:8px}
.row2{display:grid;grid-template-columns:1fr 1fr;gap:12px}
button,.btn{background:var(--acc);color:#fff;border:0;padding:10px 14px;border-radius:9px;font-weight:600;cursor:pointer;font-size:.9rem}
button:disabled{opacity:.45;cursor:not-allowed}
.btn-sec{background:#3a312b;color:var(--cream)}
.btn-sm{padding:6px 10px;font-size:.8rem}
.drop{border:2px dashed #4a3f37;border-radius:12px;padding:16px;text-align:center;cursor:pointer}
.drop input{display:none}
canvas{width:180px;height:154px;background:#000;border-radius:12px;border:3px solid #3a312b;display:block;margin:10px auto}
.bar{height:8px;background:#151210;border-radius:4px;overflow:hidden;margin-top:8px}
.bar>div{height:100%;background:var(--acc);width:0%;transition:width .15s}
.ch{display:flex;align-items:center;justify-content:space-between;padding:8px 0;border-bottom:1px solid #332b26;gap:8px}
.ch:last-child{border:0}
.ch .name{font-weight:600;word-break:break-all}
.ch .sz{color:var(--muted);font-size:.78rem}
.badge{background:var(--acc);color:#fff;font-size:.7rem;padding:2px 6px;border-radius:6px;margin-left:6px}
.chk{display:flex;align-items:center;gap:8px;margin-top:10px;font-size:.86rem;color:var(--cream);cursor:pointer}
.chk input{width:auto;accent-color:var(--acc)}
#status{font-size:.83rem;color:var(--muted);margin-top:6px;min-height:1.1em}
</style>
</head>
<body>
<h1>📺 Mini Retro TV</h1>
<div class="sub">Sesli & Kablosuz Kanal Yöneticisi (280×240)</div>

<div class="card">
  <div style="display:flex;justify-content:space-between;font-size:.82rem">
    <span id="memLbl">Hafıza</span><span id="memTxt">-</span>
  </div>
  <div class="bar"><div id="memBar"></div></div>
  <div class="row2" style="margin-top:8px">
    <div>
      <label>Ekran Parlaklığı</label>
      <input type="range" id="bri" min="10" max="255" value="200" onchange="saveBri(this.value)">
    </div>
    <div>
      <label>🔊 Ses Seviyesi (<span id="volTxt">65</span>%)</label>
      <input type="range" id="vol" min="0" max="100" value="65" oninput="document.getElementById('volTxt').textContent=this.value" onchange="saveVol(this.value)">
    </div>
  </div>
</div>

<div class="card">
  <strong>Yeni Video Yükle</strong>
  <label class="drop" style="margin-top:8px">
    <input type="file" id="file" accept="video/*,image/*,.mjpeg,.pcm" onchange="onFile()">
    <div id="fileLbl">📁 MP4 / MOV / WebM veya hazır .mjpeg seçin</div>
  </label>
  <canvas id="cv" width="280" height="240"></canvas>
  <div class="row">
    <div><label>FPS</label><input type="number" id="fps" value="12" min="5" max="24"></div>
    <div><label>Max Süre (sn)</label><input type="number" id="maxDur" value="30" min="1" max="300"></div>
    <div><label>Kalite</label>
      <select id="qual">
        <option value="0.5">Küçük dosya</option>
        <option value="0.62" selected>Dengeli</option>
        <option value="0.75">Yüksek</option>
      </select>
    </div>
  </div>
  <label>Sığdırma</label>
  <select id="fit">
    <option value="cover" selected>Ekranı Doldur (4:3 kırp)</option>
    <option value="contain">Tamamını Göster (siyah kenar)</option>
  </select>
  <label class="chk"><input type="checkbox" id="withAudio" checked> 🔊 Videonun sesini de aktar (16 kHz)</label>
  <button id="upBtn" style="width:100%;margin-top:12px" onclick="startUpload()" disabled>Dönüştür ve TV'ye Gönder</button>
  <div class="bar"><div id="pBar"></div></div>
  <div id="status"></div>
</div>

<div class="card">
  <strong>Kanallar</strong>
  <div id="chList" style="margin-top:6px;color:var(--muted);font-size:.85rem">Yükleniyor...</div>
</div>

<script>
const W=280,H=240,SR=16000,cv=document.getElementById('cv'),ctx=cv.getContext('2d');
let selFile=null;

function fmtKB(kb){return kb>=1024?(kb/1024).toFixed(1)+' MB':kb+' KB';}
function setSt(t,p){document.getElementById('status').textContent=t;if(p!==undefined)document.getElementById('pBar').style.width=p+'%';}

async function refresh(){
  const r=await fetch('/api/list'); const d=await r.json();
  const pct=d.total?Math.round(d.used*100/d.total):0;
  document.getElementById('memLbl').textContent=d.sd?'💾 MicroSD Kart':'📦 Dahili Hafıza';
  document.getElementById('memTxt').textContent=`${fmtKB(d.used)} / ${fmtKB(d.total)} (%${pct})`;
  document.getElementById('memBar').style.width=pct+'%';
  document.getElementById('bri').value=d.brightness;
  document.getElementById('vol').value=d.volume;
  document.getElementById('volTxt').textContent=d.volume;
  const box=document.getElementById('chList');
  if(!d.files.length){box.innerHTML='Henüz video yok. Yukarıdan bir MP4 seçin.';return;}
  box.innerHTML=d.files.map((f,i)=>`
    <div class="ch">
      <div><span class="name">CH${i+1}: ${f.name}</span>${f.audio?' 🔊':''}${i===d.current?'<span class="badge">AÇIK</span>':''}<div class="sz">${(f.size/1024).toFixed(0)} KB</div></div>
      <div style="display:flex;gap:6px">
        <button class="btn-sm" onclick="playCh(${i})">Oynat</button>
        <button class="btn-sm btn-sec" onclick="delCh('${f.name}')">Sil</button>
      </div>
    </div>`).join('');
}

async function playCh(i){await fetch('/api/play?i='+i,{method:'POST'});refresh();}
async function delCh(n){if(!confirm(n+' silinsin mi?'))return;await fetch('/api/delete?name='+encodeURIComponent(n),{method:'POST'});refresh();}
async function saveBri(v){await fetch('/api/settings?b='+v,{method:'POST'});}
async function saveVol(v){await fetch('/api/settings?vol='+v,{method:'POST'});}

function drawFit(el,vw,vh){
  ctx.fillStyle='#000';ctx.fillRect(0,0,W,H);
  const cover=document.getElementById('fit').value==='cover';
  const s=cover?Math.max(W/vw,H/vh):Math.min(W/vw,H/vh);
  const dw=vw*s,dh=vh*s;
  ctx.drawImage(el,(W-dw)/2,(H-dh)/2,dw,dh);
}

function onFile(){
  selFile=document.getElementById('file').files[0];
  if(!selFile)return;
  document.getElementById('fileLbl').textContent='📄 '+selFile.name;
  document.getElementById('upBtn').disabled=false;
  setSt('Hazır.',0);
}

async function toJpegBytes(q){
  const blob=await new Promise(r=>cv.toBlob(r,'image/jpeg',q));
  return new Uint8Array(await blob.arrayBuffer());
}

async function extractPcm(file,maxDur){
  try{
    setSt('Ses çıkarılıyor (16 kHz)...',55);
    const ab=await file.arrayBuffer();
    const ac=new (window.AudioContext||window.webkitAudioContext)();
    const dec=await ac.decodeAudioData(ab);
    ac.close();
    const dur=Math.min(dec.duration,maxDur);
    const len=Math.max(1,Math.floor(dur*SR));
    const off=new OfflineAudioContext(1,len,SR);
    const src=off.createBufferSource();
    src.buffer=dec; src.connect(off.destination); src.start(0);
    const rendered=await off.startRendering();
    const ch=rendered.getChannelData(0);
    const pcm=new Int16Array(ch.length);
    for(let i=0;i<ch.length;i++){
      let s=Math.max(-1,Math.min(1,ch[i]));
      pcm[i]=s<0?s*32768:s*32767;
    }
    return new Blob([pcm.buffer],{type:'application/octet-stream'});
  }catch(e){return null;}
}

async function convertVideo(file,fps,maxDur,q){
  const url=URL.createObjectURL(file);
  const v=document.createElement('video');
  v.src=url; v.muted=true; v.playsInline=true; v.preload='auto';
  await new Promise((res,rej)=>{v.onloadedmetadata=res;v.onerror=()=>rej('Video açılamadı (H.264 MP4 deneyin)');});
  const dur=Math.min(v.duration||maxDur,maxDur);
  const total=Math.max(1,Math.floor(dur*fps));
  const parts=[]; let sz=0;
  for(let i=0;i<total;i++){
    v.currentTime=i/fps;
    await new Promise(r=>{v.onseeked=r;});
    drawFit(v,v.videoWidth,v.videoHeight);
    const b=await toJpegBytes(q);
    parts.push(b); sz+=b.byteLength;
    setSt(`Video dönüştürülüyor: ${i+1}/${total} kare (${(sz/1024).toFixed(0)} KB)`,Math.round((i+1)*50/total));
  }
  URL.revokeObjectURL(url);
  return {blob:new Blob(parts,{type:'application/octet-stream'}),dur};
}

async function convertImage(file,q){
  const bmp=await createImageBitmap(file);
  drawFit(bmp,bmp.width,bmp.height);
  const b=await toJpegBytes(q);
  return new Blob([b,b,b],{type:'application/octet-stream'});
}

function sendBlob(blob,name,pStart,pSpan,label){
  return new Promise((res,rej)=>{
    const fd=new FormData(); fd.append('file',blob,name);
    const x=new XMLHttpRequest();
    x.open('POST','/api/upload');
    x.upload.onprogress=e=>{if(e.lengthComputable)setSt(`${label} %${Math.round(e.loaded*100/e.total)}`,pStart+Math.round(e.loaded*pSpan/e.total));};
    x.onload=()=>x.status===200?res():rej('Yükleme hatası (hafıza dolu olabilir)');
    x.onerror=()=>rej('Bağlantı koptu');
    x.send(fd);
  });
}

async function startUpload(){
  if(!selFile)return;
  const btn=document.getElementById('upBtn'); btn.disabled=true;
  try{
    const fps=parseInt(document.getElementById('fps').value)||12;
    const maxDur=parseFloat(document.getElementById('maxDur').value)||30;
    const q=parseFloat(document.getElementById('qual').value)||0.62;
    const wantAudio=document.getElementById('withAudio').checked;
    let blob, pcmBlob=null, base=selFile.name.replace(/\.[^.]+$/,'').replace(/[^a-zA-Z0-9_-]/g,'_').slice(0,20)||'video';
    let outName, pcmName=null;
    const low=selFile.name.toLowerCase();
    if(low.endsWith('.mjpeg')||low.endsWith('.pcm')){
      blob=selFile; outName=base+(low.endsWith('.pcm')?'.pcm':'.mjpeg');
    }else if(selFile.type.startsWith('image/')){
      blob=await convertImage(selFile,q); outName=base+'_f1.mjpeg';
    }else{
      const res=await convertVideo(selFile,fps,maxDur,q);
      blob=res.blob; outName=`${base}_f${fps}.mjpeg`;
      if(wantAudio){ pcmBlob=await extractPcm(selFile,res.dur); pcmName=`${base}_f${fps}.pcm`; }
    }
    if(pcmBlob){
      await sendBlob(pcmBlob,pcmName,60,15,'Ses yükleniyor');
      await sendBlob(blob,outName,75,25,'Video yükleniyor');
    }else{
      await sendBlob(blob,outName,60,40,'Yükleniyor');
    }
    setSt('✅ Yüklendi ve oynatılıyor!',100);
    refresh();
  }catch(e){setSt('❌ '+e,0);}
  btn.disabled=false;
}
refresh();
</script>
</body>
</html>
)rawliteral";
