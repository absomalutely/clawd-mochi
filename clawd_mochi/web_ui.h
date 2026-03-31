#pragma once
#include <Arduino.h>

const char INDEX_HTML[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no">
<title>Clawd Mochi</title>
<style>
*{box-sizing:border-box;margin:0;padding:0;-webkit-tap-highlight-color:transparent}
body{background:#1c1c20;font-family:'Courier New',monospace;color:#e8e4dc;
  display:flex;flex-direction:column;align-items:center;
  padding:20px 14px 52px;gap:14px;min-height:100vh;position:relative}

.hdr{text-align:center;padding:2px 0 4px}
.mascot{font-size:15px;color:#c96a3e;line-height:1.3;font-weight:bold;
  font-family:'Courier New',monospace;display:block;letter-spacing:1px}
.sitename{font-size:10px;color:#5a5048;margin-top:8px;letter-spacing:3px}

.sec{width:100%;max-width:390px;font-size:10px;color:#8a8278;
  letter-spacing:2px;font-weight:bold;padding:0 2px}

.busy{width:100%;max-width:390px;height:2px;background:#2e2a28;
  border-radius:1px;overflow:hidden;opacity:0;transition:opacity .2s}
.busy.show{opacity:1}
.busy-i{height:100%;width:30%;background:#c96a3e;border-radius:1px;
  animation:sl 1s linear infinite}
@keyframes sl{0%{margin-left:-30%}100%{margin-left:100%}}

.ctrl{display:flex;gap:8px;width:100%;max-width:390px}
.cbtn{flex:1;background:#252428;border:1.5px solid #38343a;border-radius:10px;
  color:#b8b4ac;font-family:'Courier New',monospace;font-size:11px;font-weight:bold;
  padding:12px 4px;cursor:pointer;text-align:center;transition:all .12s}
.cbtn:active:not(:disabled){transform:scale(.94)}
.cbtn:disabled{opacity:.3;cursor:default}
.cbtn.on{border-color:#c96a3e;color:#c96a3e;background:#201408}
.cbtn.dim{border-color:#2e2a28;color:#4a4540}

.vgrid{display:grid;grid-template-columns:1fr 1fr;gap:8px;width:100%;max-width:390px}
.vbtn{background:#252428;border:1.5px solid #38343a;border-radius:12px;
  color:#d8d4cc;font-family:'Courier New',monospace;
  padding:14px 6px 10px;cursor:pointer;text-align:center;
  transition:all .12s;user-select:none}
.vbtn:active:not(:disabled){transform:scale(.94)}
.vbtn:disabled{opacity:.3;cursor:default}
.vbtn .ic{font-size:20px;display:block;margin-bottom:4px;line-height:1;color:#c96a3e}
.vbtn .nm{font-size:12px;font-weight:bold;color:#e8e4dc}
.vbtn .ht{font-size:9px;color:#8a8278;margin-top:3px}
.vbtn.active{border-color:#c96a3e;background:#201408}
.vbtn[data-v="1"].active{border-color:#c96a3e;background:#201408}
.vbtn[data-v="3"].active{border-color:#38343a;background:#201c18}
.vbtn[data-v="5"].active{border-color:#ffcc33;background:#181408}

.speed-row{width:100%;max-width:390px;display:flex;align-items:center;gap:10px}
.sl{font-size:10px;color:#6a6058;white-space:nowrap;min-width:36px}
input[type=range]{flex:1;accent-color:#c96a3e;cursor:pointer;height:20px}
.sv{font-size:11px;color:#c96a3e;min-width:44px;text-align:right;font-weight:bold}

.twrap{width:100%;max-width:390px;display:none;flex-direction:column;gap:8px}
.twrap.open{display:flex}
.thdr{display:flex;justify-content:space-between;align-items:center}
.tttl{font-size:11px;color:#28b878;letter-spacing:1px;font-weight:bold}
.tx{background:#0c1e12;border:2px solid #1a4828;border-radius:9px;
  color:#28b878;font-family:'Courier New',monospace;font-size:13px;
  font-weight:bold;padding:10px 18px;cursor:pointer}
.tx:active{background:#081410}
.trow{display:flex;gap:6px}
.tin{flex:1;background:#0c1018;border:1.5px solid #1a2820;border-radius:9px;
  color:#40d880;font-family:'Courier New',monospace;font-size:15px;
  padding:11px;outline:none}
.tin::placeholder{color:#2a3828}
.tgo{background:#1a9060;border:none;border-radius:9px;color:#fff;
  font-family:'Courier New',monospace;font-size:22px;font-weight:bold;
  padding:11px 16px;cursor:pointer;min-width:52px}
.tgo:active{background:#0f6040}

.cwrap{width:100%;max-width:390px;background:#222028;border:1.5px solid #38343a;
  border-radius:12px;padding:12px;flex-direction:column;gap:10px;display:none}
.cwrap.open{display:flex}
.crow{display:flex;gap:8px}
.ci{display:flex;flex-direction:column;align-items:center;gap:4px;flex:1}
.cl{font-size:10px;color:#7a7068;letter-spacing:1px;font-weight:bold}
.cs{width:100%;height:38px;border-radius:7px;border:1.5px solid #38343a;cursor:pointer;padding:0}
.dacts{display:flex;gap:7px}
.db{flex:1;background:#1c1820;border:1.5px solid #38343a;border-radius:9px;
  color:#c0bab8;font-family:'Courier New',monospace;font-size:11px;
  font-weight:bold;padding:11px 4px;cursor:pointer;transition:all .12s}
.db:active{transform:scale(.95);background:#281838}
.db.hi{border-color:#c96a3e;color:#c96a3e}
canvas{width:100%;border-radius:8px;border:1.5px solid #38343a;
  touch-action:none;cursor:crosshair;display:block}

.toast{position:fixed;bottom:18px;left:50%;transform:translateX(-50%);
  background:#252428;border:1.5px solid #38343a;border-radius:9px;
  font-size:12px;color:#d8d4cc;padding:7px 16px;opacity:0;
  transition:opacity .18s;pointer-events:none;white-space:nowrap;z-index:99}
.toast.show{opacity:1}

.cog-btn{position:fixed;top:14px;right:14px;background:#252428;border:1.5px solid #38343a;
  border-radius:50%;width:38px;height:38px;cursor:pointer;z-index:50;
  display:flex;align-items:center;justify-content:center;
  font-size:20px;color:#8a8278;transition:all .2s}
.cog-btn:active{transform:scale(.9)}
.cog-btn.open{color:#c96a3e;border-color:#c96a3e;transform:rotate(90deg)}

.settings-panel{width:100%;max-width:390px;display:none;flex-direction:column;gap:14px;
  background:#1a1a1e;border:1.5px solid #38343a;border-radius:12px;padding:14px}
.settings-panel.open{display:flex}
</style>
</head>
<body>

<button class="cog-btn" id="cogBtn" onclick="toggleSettings()">&#9881;</button>

<div class="hdr">
  <span class="mascot">&#x2590;&#x259B;&#x2588;&#x2588;&#x2588;&#x259C;&#x258C;<br>&#x259C;&#x2588;&#x2588;&#x2588;&#x2588;&#x2588;&#x259B;<br>&#x2598;&#x2598;&nbsp;&#x259D;&#x259D;</span>
  <div class="sitename">CLAWD &middot; MOCHI &middot; CONTROLLER</div>
</div>

<div class="busy" id="busy"><div class="busy-i"></div></div>

<div class="sec">// controls</div>
<div class="ctrl">
  <button class="cbtn on" id="blBtn" onclick="toggleBL()">&#9728; display on</button>
</div>
<div class="speed-row">
  <span class="sl">dim</span>
  <input type="range" id="bri" min="10" max="255" value="255" step="5" oninput="setBri(this.value)">
  <span class="sl">bright</span>
</div>

<div class="sec">// views</div>
<div class="vgrid">
  <button class="vbtn active" data-v="0" onclick="setView(0)">
    <span class="ic">&#9632; &#9632;</span>
    <span class="nm">Normal eyes</span>
    <span class="ht">wiggle + blink</span>
  </button>
  <button class="vbtn" data-v="1" onclick="setView(1)">
    <span class="ic">&gt; &lt;</span>
    <span class="nm">Squish eyes</span>
    <span class="ht">open / close</span>
  </button>
  <button class="vbtn" data-v="3" onclick="toggleCanvas()">
    <span class="ic">&#11035;</span>
    <span class="nm">Canvas</span>
    <span class="ht">draw on display</span>
  </button>
  <button class="vbtn" data-v="5" onclick="setView(5)">
    <span class="ic">&#9729;</span>
    <span class="nm">Weather</span>
    <span class="ht">current conditions</span>
  </button>
</div>

<div class="sec">// speed</div>
<div class="speed-row">
  <span class="sl">slow</span>
  <input type="range" id="spd" min="1" max="3" value="1" step="1" oninput="setSpeed(this.value)">
  <span class="sv" id="spdV">slow</span>
</div>

<div class="ctrl">
  <div class="ci" style="flex:1;display:flex;flex-direction:column;gap:4px;align-items:stretch">
    <span class="cl" style="font-size:10px;color:#8a8278;letter-spacing:1px;font-weight:bold;text-align:center">BACKGROUND</span>
    <input type="color" class="cs" id="bgCol" value="#aa4818" oninput="onBgChange(this.value)">
  </div>
  <div class="ci" style="flex:1;display:flex;flex-direction:column;gap:4px;align-items:stretch">
    <span class="cl" style="font-size:10px;color:#8a8278;letter-spacing:1px;font-weight:bold;text-align:center">PEN COLOR</span>
    <input type="color" class="cs" id="penCol" value="#000000">
  </div>
</div>

<div class="cwrap" id="cwrap">
  <div class="dacts">
    <button class="db hi" onclick="clearAll()">&#11035; clear</button>
    <button class="db" style="border-color:#28b878;color:#28b878" onclick="toggleCanvas()">&#10003; done</button>
  </div>
  <div class="speed-row" style="max-width:100%">
    <span class="sl">thin</span>
    <input type="range" id="penSize" min="1" max="20" value="8" step="1">
    <span class="sl">thick</span>
  </div>
  <canvas id="cvs" width="240" height="240"></canvas>
</div>

<div class="sec">// ticker</div>
<div class="twrap open" id="tkwrap">
  <div class="thdr">
    <span class="tttl" style="color:#c96a3e">&#9654; scrolling ticker</span>
    <button class="tx" style="background:#1c0e08;border-color:#3a2418;color:#c96a3e"
            onclick="stopTicker()">&#x2715; stop</button>
  </div>
  <div class="trow">
    <input class="tin" id="tkText" type="text" placeholder="enter message..."
           style="border-color:#3a2418;color:#c96a3e;background:#1c0e08"
           autocomplete="off" autocorrect="off" autocapitalize="off" spellcheck="false">
    <button class="tgo" style="background:#c96a3e" onclick="sendTicker()">&#9654;</button>
  </div>
  <div class="speed-row" style="max-width:100%">
    <span class="sl">slow</span>
    <input type="range" id="tkSpd" min="1" max="5" value="2" step="1">
    <span class="sl">fast</span>
  </div>
  <div class="speed-row" style="max-width:100%">
    <span class="sl">small</span>
    <input type="range" id="tkSize" min="1" max="5" value="3" step="1">
    <span class="sl">large</span>
  </div>
</div>

<div class="settings-panel" id="settingsPanel">
  <div class="sec" style="padding:0">// device hotspot</div>
  <div style="display:flex;flex-direction:column;gap:8px">
    <div class="thdr">
      <span class="tttl" style="color:#c96a3e">&#9881; mochi AP</span>
      <span id="apStatus" style="font-size:10px;color:#5a5048">...</span>
    </div>
    <div class="trow">
      <input class="tin" id="apSSID" type="text" placeholder="hotspot name"
             style="border-color:#3a2418;color:#c96a3e;background:#1c0e08"
             autocomplete="off" autocorrect="off" autocapitalize="off" spellcheck="false">
    </div>
    <div class="trow">
      <input class="tin" id="apPass" type="password" placeholder="password"
             style="border-color:#3a2418;color:#c96a3e;background:#1c0e08"
             autocomplete="off">
      <button class="tgo" style="background:#c96a3e" onclick="saveAP()">&#10003;</button>
    </div>
  </div>

  <div class="sec" style="padding:0">// home wifi</div>
  <div style="display:flex;flex-direction:column;gap:8px">
    <div class="thdr">
      <span class="tttl" style="color:#4a8acd">&#8801; home wifi</span>
      <span id="wfStatus" style="font-size:10px;color:#5a5048">...</span>
    </div>
    <div class="trow">
      <input class="tin" id="wfSSID" type="text" placeholder="SSID"
             style="border-color:#1a2838;color:#4a8acd;background:#0c1018"
             autocomplete="off" autocorrect="off" autocapitalize="off" spellcheck="false">
    </div>
    <div class="trow">
      <input class="tin" id="wfPass" type="password" placeholder="password"
             style="border-color:#1a2838;color:#4a8acd;background:#0c1018"
             autocomplete="off">
      <button class="tgo" style="background:#4a8acd" onclick="saveWifi()">&#10003;</button>
    </div>
  </div>

  <div class="sec" style="padding:0">// weather config</div>
  <div style="display:flex;flex-direction:column;gap:8px">
    <div class="thdr">
      <span class="tttl" style="color:#ffcc33">&#9729; openweathermap</span>
      <span id="wxStatus" style="font-size:10px;color:#5a5048">...</span>
    </div>
    <div class="trow">
      <input class="tin" id="wxKey" type="password" placeholder="API key"
             style="border-color:#38301a;color:#ffcc33;background:#181408"
             autocomplete="off">
    </div>
    <div class="trow">
      <input class="tin" id="wxLat" type="text" placeholder="latitude"
             style="border-color:#38301a;color:#ffcc33;background:#181408;flex:1"
             autocomplete="off">
      <input class="tin" id="wxLon" type="text" placeholder="longitude"
             style="border-color:#38301a;color:#ffcc33;background:#181408;flex:1"
             autocomplete="off">
      <button class="tgo" style="background:#cc9a20" onclick="saveWeather()">&#10003;</button>
    </div>
  </div>
</div>

<div class="toast" id="toast"></div>

<script>
let activeView  = 0;
let canvasOpen  = false;
let blOn        = true;
let isBusy      = false;
let drawing     = false;
let lastX = 0, lastY = 0;
let tt;

const spdLabels = ['','slow','normal','fast'];

function toast(msg, ok=true) {
  const el = document.getElementById('toast');
  el.textContent = msg;
  el.style.borderColor = ok ? '#28b878' : '#c96a3e';
  el.classList.add('show');
  clearTimeout(tt);
  tt = setTimeout(() => el.classList.remove('show'), 1300);
}

function setBusy(b) {
  isBusy = b;
  document.getElementById('busy').classList.toggle('show', b);
  document.querySelectorAll('.vbtn').forEach(el => {
    el.disabled = canvasOpen ? parseInt(el.dataset.v) !== 3 : b;
  });
  document.querySelectorAll('.cbtn').forEach(el => {
    if (el.id !== 'blBtn') el.disabled = b;
  });
}

async function req(path) {
  try { const r = await fetch(path); return r.ok; }
  catch(e) { toast('no connection', false); return false; }
}

async function waitNotBusy() {
  for (let i = 0; i < 100; i++) {
    try {
      const r = await fetch('/state');
      const j = await r.json();
      if (!j.busy) return;
    } catch(e) {}
    await new Promise(r => setTimeout(r, 150));
  }
}

async function onBgChange(hex) {
  if (canvasOpen) {
    await req('/draw/clear?bg=' + encodeURIComponent(hex));
  } else {
    await req('/redraw?bg=' + encodeURIComponent(hex));
  }
  redrawCanvas(hex);
}

async function setSpeed(v) {
  document.getElementById('spdV').textContent = spdLabels[v];
  await req('/speed?v=' + v);
}

async function setView(v) {
  if (isBusy || canvasOpen) return;
  if (v === 3) { toggleCanvas(); return; }
  const keys = {0:'w', 1:'s', 5:'x'};
  if (!await req('/cmd?k=' + keys[v])) return;
  activeView = v;
  document.querySelectorAll('.vbtn').forEach(b =>
    b.classList.toggle('active', parseInt(b.dataset.v) === v));
  setBusy(true);
  await waitNotBusy();
  setBusy(false);
}

async function setBri(v) {
  await req('/backlight?bri=' + v);
}

async function toggleBL() {
  blOn = !blOn;
  await req('/backlight?on=' + (blOn ? 1 : 0));
  const b = document.getElementById('blBtn');
  b.textContent = blOn ? '\u2600 display on' : '\u25cb display off';
  b.classList.toggle('on', blOn);
  b.classList.toggle('dim', !blOn);
}

async function toggleCanvas() {
  canvasOpen = !canvasOpen;
  document.getElementById('cwrap').classList.toggle('open', canvasOpen);
  document.querySelectorAll('.vbtn').forEach(btn =>
    btn.classList.toggle('active', canvasOpen && parseInt(btn.dataset.v) === 3));
  await req('/canvas?on=' + (canvasOpen ? 1 : 0));
  if (canvasOpen) {
    const bg = document.getElementById('bgCol').value;
    redrawCanvas(bg);
    await req('/draw/clear?bg=' + encodeURIComponent(bg));
    document.querySelectorAll('.vbtn').forEach(b => b.disabled = true);
    toast('canvas active');
  } else {
    setBusy(false);
    toast('canvas off');
  }
}

function toggleSettings() {
  const panel = document.getElementById('settingsPanel');
  const btn = document.getElementById('cogBtn');
  panel.classList.toggle('open');
  btn.classList.toggle('open');
}

const cvs = document.getElementById('cvs');
const ctx = cvs.getContext('2d');
let strokePts = [];

function getPenSize() { return parseInt(document.getElementById('penSize').value); }

function getPos(e) {
  const r = cvs.getBoundingClientRect();
  const sx = cvs.width / r.width, sy = cvs.height / r.height;
  const s = e.touches ? e.touches[0] : e;
  return { x: (s.clientX - r.left) * sx, y: (s.clientY - r.top) * sy };
}

function redrawCanvas(hex) {
  ctx.fillStyle = hex;
  ctx.fillRect(0, 0, cvs.width, cvs.height);
}

function startDraw(e) {
  e.preventDefault();
  drawing = true;
  strokePts = [];
  const p = getPos(e); lastX = p.x; lastY = p.y;
  strokePts.push({ x: Math.round(p.x), y: Math.round(p.y) });
  const sz = getPenSize();
  ctx.beginPath(); ctx.arc(p.x, p.y, sz/2, 0, Math.PI * 2);
  ctx.fillStyle = document.getElementById('penCol').value; ctx.fill();
}
function moveDraw(e) {
  if (!drawing) return; e.preventDefault();
  const p = getPos(e);
  const sz = getPenSize();
  ctx.beginPath(); ctx.moveTo(lastX, lastY); ctx.lineTo(p.x, p.y);
  ctx.strokeStyle = document.getElementById('penCol').value;
  ctx.lineWidth = sz; ctx.lineCap = 'round'; ctx.stroke();
  strokePts.push({ x: Math.round(p.x), y: Math.round(p.y) });
  lastX = p.x; lastY = p.y;
}
async function endDraw(e) {
  if (!drawing) return; drawing = false;
  if (!canvasOpen || strokePts.length < 1) return;
  const pen = document.getElementById('penCol').value.replace('#', '');
  const sz = getPenSize();
  const pts = strokePts.map(p => p.x + ',' + p.y).join(';');
  await req('/draw/stroke?pen=' + pen + '&sz=' + sz + '&pts=' + encodeURIComponent(pts));
  strokePts = [];
}

cvs.addEventListener('mousedown',  startDraw);
cvs.addEventListener('mousemove',  moveDraw);
cvs.addEventListener('mouseup',    endDraw);
cvs.addEventListener('mouseleave', endDraw);
cvs.addEventListener('touchstart', startDraw, {passive:false});
cvs.addEventListener('touchmove',  moveDraw,  {passive:false});
cvs.addEventListener('touchend',   endDraw);

async function sendTicker() {
  const text = document.getElementById('tkText').value.trim();
  if (!text) { toast('enter a message', false); return; }
  const speed = document.getElementById('tkSpd').value;
  const size  = document.getElementById('tkSize').value;
  const bg    = document.getElementById('bgCol').value.replace('#','');
  const pen   = document.getElementById('penCol').value.replace('#','');
  if (!await req('/ticker?text=' + encodeURIComponent(text)
    + '&speed=' + speed + '&size=' + size
    + '&bg=' + bg + '&pen=' + pen)) return;
  toast('ticker sent');
}
async function stopTicker() {
  if (!await req('/ticker?action=stop')) return;
  toast('ticker stopped');
}
document.getElementById('tkText').addEventListener('keydown', e => {
  if (e.key === 'Enter') { e.preventDefault(); sendTicker(); }
});

async function saveWifi() {
  const ssid = document.getElementById('wfSSID').value.trim();
  const pass = document.getElementById('wfPass').value;
  if (!ssid) { toast('enter SSID', false); return; }
  if (!await req('/config/wifi?ssid=' + encodeURIComponent(ssid) + '&pass=' + encodeURIComponent(pass))) return;
  toast('saved — reboot to connect');
}
async function checkWifiStatus() {
  try {
    const r = await fetch('/config/wifi');
    const j = await r.json();
    const el = document.getElementById('wfStatus');
    if (j.connected) {
      el.textContent = '\u2713 ' + j.ssid + ' (' + j.ip + ')';
      el.style.color = '#28b878';
    } else if (j.ssid) {
      el.textContent = '\u2717 ' + j.ssid;
      el.style.color = '#c96a3e';
    } else {
      el.textContent = 'not configured';
    }
  } catch(e) {}
}

async function saveAP() {
  const ssid = document.getElementById('apSSID').value.trim();
  const pass = document.getElementById('apPass').value;
  if (!ssid) { toast('enter hotspot name', false); return; }
  if (!await req('/config/ap?ssid=' + encodeURIComponent(ssid) + '&pass=' + encodeURIComponent(pass))) return;
  toast('saved — reboot to apply');
}
async function checkAPStatus() {
  try {
    const r = await fetch('/config/ap');
    const j = await r.json();
    const el = document.getElementById('apStatus');
    el.textContent = j.ssid;
    el.style.color = '#c96a3e';
  } catch(e) {}
}

async function saveWeather() {
  const key = document.getElementById('wxKey').value.trim();
  const lat = document.getElementById('wxLat').value.trim();
  const lon = document.getElementById('wxLon').value.trim();
  if (!key && !lat && !lon) { toast('enter at least one field', false); return; }
  let q = '/config/weather?';
  if (key) q += 'key=' + encodeURIComponent(key) + '&';
  if (lat) q += 'lat=' + encodeURIComponent(lat) + '&';
  if (lon) q += 'lon=' + encodeURIComponent(lon);
  if (!await req(q)) return;
  toast('weather config saved');
  checkWeatherStatus();
}
async function checkWeatherStatus() {
  try {
    const r = await fetch('/config/weather');
    const j = await r.json();
    const el = document.getElementById('wxStatus');
    if (j.hasKey && j.lat) {
      el.textContent = '\u2713 configured (' + j.lat + ', ' + j.lon + ')';
      el.style.color = '#28b878';
    } else if (j.hasKey) {
      el.textContent = 'need location';
      el.style.color = '#c96a3e';
    } else {
      el.textContent = 'need API key';
      el.style.color = '#5a5048';
    }
  } catch(e) {}
}

async function clearAll() {
  const bg = document.getElementById('bgCol').value;
  redrawCanvas(bg);
  await req('/draw/clear?bg=' + encodeURIComponent(bg));
  toast('cleared');
}

(async () => {
  try {
    const r = await fetch('/state');
    const j = await r.json();
    const spd = j.speed || 1;
    document.getElementById('spd').value = spd;
    document.getElementById('spdV').textContent = spdLabels[spd];
    if (j.bl === false) {
      blOn = false;
      const b = document.getElementById('blBtn');
      b.textContent = '\u25cb display off';
      b.classList.remove('on'); b.classList.add('dim');
    }
    if (j.bri) document.getElementById('bri').value = j.bri;
    if (j.bg) document.getElementById('bgCol').value = j.bg;
    if (j.pen) document.getElementById('penCol').value = j.pen;
  } catch(e) {}
  const bg = document.getElementById('bgCol').value;
  redrawCanvas(bg);
  checkAPStatus();
  checkWifiStatus();
  checkWeatherStatus();
})();
</script>
</body>
</html>
)rawhtml";
