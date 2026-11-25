#ifndef WEB_UI_H
#define WEB_UI_H

#include "config.h"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define RELAY_COUNT_STR STR(RELAY_COUNT)
#define INPUT_COUNT_STR STR(INPUT_COUNT)

const char* HTML_PAGE = R"rawliteral(
<!DOCTYPE html>
<html lang="ru">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>)rawliteral" HostName R"rawliteral(</title>
<style>
:root {
  --bg-color:#f0f2f5; --text-color:#1a1a2e; --card-bg:#ffffff;
  --accent-color:#e94560; --secondary-color:#0f3460;
  --border-color:#dcdde1; --shadow:0 2px 8px rgba(0,0,0,.08);
  --relay-on:#27ae60; --relay-off:#95a5a6;
  --sidebar:#2c3e50; --sidebar-text:#ecf0f1;
  --btn-text:#ffffff;
}
body.dark {
  --bg-color:#121212; --text-color:#e0e0e0; --card-bg:#1e1e1e;
  --accent-color:#e94560; --secondary-color:#0f3460;
  --border-color:#333333; --shadow:0 2px 8px rgba(0,0,0,.5);
  --sidebar:#0f3460; --sidebar-text:#e0e0e0;
  --btn-text:#ffffff;
}
body.theme-blue {
  --bg-color:#e3f2fd; --text-color:#0d47a1; --card-bg:#ffffff;
  --accent-color:#1976d2; --secondary-color:#42a5f5;
  --border-color:#bbdefb; --shadow:0 2px 8px rgba(0,0,0,.1);
  --sidebar:#1565c0; --sidebar-text:#e3f2fd;
  --btn-text:#ffffff;
}
body.theme-green {
  --bg-color:#e8f5e9; --text-color:#1b5e20; --card-bg:#ffffff;
  --accent-color:#2e7d32; --secondary-color:#66bb6a;
  --border-color:#c8e6c9; --shadow:0 2px 8px rgba(0,0,0,.1);
  --sidebar:#2e7d32; --sidebar-text:#e8f5e9;
  --btn-text:#ffffff;
}
body.theme-purple {
  --bg-color:#f3e5f5; --text-color:#4a148c; --card-bg:#ffffff;
  --accent-color:#9c27b0; --secondary-color:#ce93d8;
  --border-color:#e1bee7; --shadow:0 2px 8px rgba(0,0,0,.1);
  --sidebar:#6a1b9a; --sidebar-text:#f3e5f5;
  --btn-text:#ffffff;
}
body.theme-orange {
  --bg-color:#fff3e0; --text-color:#e65100; --card-bg:#ffffff;
  --accent-color:#ff9800; --secondary-color:#ffb74d;
  --border-color:#ffe0b2; --shadow:0 2px 8px rgba(0,0,0,.1);
  --sidebar:#e65100; --sidebar-text:#fff3e0;
  --btn-text:#ffffff;
}
body.theme-cyberpunk {
  --bg-color:#0d0221; --text-color:#00ffff; --card-bg:#1a0b2e;
  --accent-color:#ff00ff; --secondary-color:#00ffff;
  --border-color:#2a0b3e; --shadow:0 2px 8px rgba(255,0,255,.2);
  --sidebar:#1a0b2e; --sidebar-text:#00ffff;
  --btn-text:#000000; /* Контрастный текст для яркой кнопки */
}

*{box-sizing:border-box;margin:0;padding:0}
body{font-family:'Segoe UI',Tahoma,sans-serif;background:var(--bg-color);color:var(--text-color);
  height:100vh;display:flex;overflow:hidden;transition:background .3s, color .3s}


.sidebar {
  width: 240px;
  background: var(--sidebar);
  color: var(--sidebar-text);
  display: flex;
  flex-direction: column;
  height: 100vh;
  flex-shrink: 0;
  overflow-y: auto;
  box-shadow: 2px 0 8px rgba(0,0,0,.15);
}

.sidebar-header {
  padding: 20px;
  font-size: 1.2rem;
  font-weight: 700;
  border-bottom: 1px solid rgba(255,255,255,.1);
  text-align: center;
}
.sidebar-header small {
  display: block;
  font-size: .75rem;
  opacity: .6;
  margin-top: 4px;
}

.menu-item {
  padding: 14px 20px;
  cursor: pointer;
  display: flex;
  align-items: center;
  gap: 12px;
  transition: .2s;
  border-left: 4px solid transparent;
  font-size: .95rem;
  color: var(--sidebar-text);
}

/* Наведение — лёгкая подсветка */
.menu-item:hover {
  background: rgba(255,255,255,.08);
}

/* === АКТИВНЫЙ ПУНКТ === */
.menu-item.active {
  background: var(--accent-color);          /* яркий фон */
  color: var(--btn-text);                   /* контрастный текст (белый или чёрный) */
  border-left: 4px solid var(--secondary-color); /* цветная полоска слева */
  font-weight: 700;
  box-shadow: inset 0 0 20px rgba(0,0,0,.15); /* лёгкая внутренняя тень для объёма */
  /* Опционально: небольшое скругление справа */
  border-radius: 0 8px 8px 0;
}

/* Иконка активного пункта — тоже контрастная */
.menu-item.active .icon {
  filter: drop-shadow(0 0 3px rgba(0,0,0,.3));
}


/* --- Main content --- */
.main{flex:1;display:flex;flex-direction:column;height:100vh;overflow:hidden}
.topbar{background:var(--card-bg);padding:12px 24px;border-bottom:1px solid var(--border-color);
  display:flex;justify-content:space-between;align-items:center;
  box-shadow:0 1px 4px rgba(0,0,0,.05);flex-shrink:0}
.topbar-title{font-size:1.1rem;font-weight:600}
.topbar-info{font-size:.85rem;opacity:.8;display:flex;gap:20px;align-items:center}
.content-area{flex:1;overflow-y:auto;padding:24px}

/* --- Cards --- */
.card{background:var(--card-bg);border-radius:12px;padding:24px;margin-bottom:20px;
  box-shadow:var(--shadow);border:1px solid var(--border-color)}
.card-title{font-size:1.1rem;font-weight:600;margin-bottom:16px;
  display:flex;align-items:center;gap:10px;border-bottom:1px solid var(--border-color);
  padding-bottom:12px}
.card-title .icon{font-size:1.5rem}

/* --- Forms --- */
.form-group{margin-bottom:16px}
.form-group label{display:block;font-weight:600;margin-bottom:6px;font-size:.9rem}
.form-group input{width:100%;padding:10px 12px;border:1px solid var(--border-color);
  border-radius:8px;background:var(--bg-color);color:var(--text-color);font-size:.95rem}
.form-group input:focus{outline:none;border-color:var(--accent-color);
  box-shadow:0 0 0 3px rgba(233,69,96,.15)}
.form-row{display:grid;grid-template-columns:1fr 1fr;gap:16px}
@media(max-width:600px){.form-row{grid-template-columns:1fr}}

/* --- Buttons --- */
.btn{padding:12px 20px;border:2px solid transparent;border-radius:8px;cursor:pointer;
  font-weight:600;font-size:.95rem;transition:.2s;display:inline-flex;
  align-items:center;justify-content:center;gap:8px;color:var(--btn-text)}
.btn-primary{background:var(--accent-color);border-color:var(--accent-color)}
.btn-primary:hover{opacity:.9;box-shadow:0 2px 6px rgba(0,0,0,.2)}
.btn-secondary{background:var(--relay-off);border-color:var(--relay-off)}
.btn-secondary:hover{opacity:.9;box-shadow:0 2px 6px rgba(0,0,0,.2)}
.btn-row{display:flex;gap:12px;flex-wrap:wrap;margin-top:16px}
.save-status{margin-top:12px;font-size:.9rem;min-height:1.2em}

/* --- Grids --- */
.sensor-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(200px,1fr));gap:16px}
.sensor-card{background:var(--bg-color);border-radius:10px;padding:16px;
  border:1px solid var(--border-color);text-align:center}
.sensor-value{font-size:2rem;font-weight:700;color:var(--accent-color);line-height:1.2}
.sensor-unit{font-size:.9rem;opacity:.7;font-weight:400}
.sensor-label{font-size:.85rem;opacity:.8;margin-bottom:8px}
.status-badge{display:inline-block;padding:3px 10px;border-radius:12px;
  font-size:.75rem;font-weight:600;margin-top:8px}
.status-ok{background:var(--relay-on);color:#fff}
.status-err{background:#e74c3c;color:#fff}

.relay-grid{display:grid;grid-template-columns:repeat(auto-fill,minmax(120px,1fr));gap:12px}
.relay-item{background:var(--bg-color);border:2px solid var(--border-color);border-radius:10px;
  padding:12px;text-align:center;transition:.2s}
.relay-item.on{border-color:var(--relay-on);box-shadow:0 0 10px rgba(39,174,96,.3)}
.relay-label{font-weight:700;font-size:.9rem;margin-bottom:6px}
.relay-state{font-size:.8rem;margin-bottom:8px;color:var(--relay-off)}
.relay-item.on .relay-state{color:var(--relay-on);font-weight:700}
.relay-toggle{padding:8px 12px;border:none;border-radius:6px;cursor:pointer;
  font-weight:700;color:#fff;background:var(--relay-off);width:100%;font-size:.85rem}
.relay-item.on .relay-toggle{background:var(--relay-on)}

.input-grid{display:grid;grid-template-columns:repeat(auto-fill,minmax(100px,1fr));gap:12px}
.input-item{background:var(--bg-color);border:2px solid var(--border-color);border-radius:10px;
  padding:12px;text-align:center}
.input-item.active{border-color:var(--accent-color);background:rgba(233,69,96,.1)}
.input-dot{display:inline-block;width:14px;height:14px;border-radius:50%;
  background:var(--relay-off);margin-bottom:6px}
.input-item.active .input-dot{background:var(--accent-color);box-shadow:0 0 8px var(--accent-color)}
.input-label{font-weight:700;font-size:.8rem}

/* --- Theme selector --- */
.theme-options{display:grid;grid-template-columns:repeat(auto-fill,minmax(140px,1fr));gap:12px}
.theme-btn{padding:16px;border:2px solid var(--border-color);border-radius:10px;
  background:var(--card-bg);color:var(--text-color);font-weight:600;cursor:pointer;
  text-align:center;transition:.2s}
.theme-btn:hover{border-color:var(--accent-color)}
.theme-btn.selected{border-color:var(--accent-color);background:var(--accent-color);color:#fff}
.theme-preview{display:flex;gap:6px;justify-content:center;margin-top:10px}
.color-dot{width:20px;height:20px;border-radius:50%;border:1px solid rgba(0,0,0,.2)}

/* --- Utility --- */
.hidden{display:none !important}
.section-title{font-size:1.3rem;font-weight:700;margin-bottom:20px}
</style>
</head>
<body>


<!-- ===== SIDEBAR ===== -->
<div class="sidebar">
  <div class="sidebar-header">
    )rawliteral" HostName R"rawliteral(
    <small>)rawliteral" FW_VERSION R"rawliteral(</small>
  </div>
  <div class="menu-item active" data-page="monitor" onclick="showPage('monitor')">
    <span class="icon">📊</span> Мониторинг
  </div>
  <div class="menu-item" data-page="network" onclick="showPage('network')">
    <span class="icon">🌐</span> Сеть
  </div>
  <div class="menu-item" data-page="security" onclick="showPage('security')">
    <span class="icon">🔒</span> Безопасность
  </div>
  <div class="menu-item" data-page="wifi" onclick="showPage('wifi')">
    <span class="icon">📶</span> Wi-Fi AP
  </div>
  <div class="menu-item" data-page="theme" onclick="showPage('theme')">
    <span class="icon">🎨</span> Тема
  </div>
</div>

<!-- ===== MAIN ===== -->
<div class="main">
  <div class="topbar">
    <div class="topbar-title" id="page-title">Мониторинг</div>
    <div class="topbar-info">
      <span id="top-eth">ETH: —</span>
      <span id="top-wifi">AP: —</span>
    </div>
  </div>

  <div class="content-area">

    <!-- ===== MONITOR PAGE ===== -->
    <div id="page-monitor" class="page">
      <div class="sensor-grid">
        <div class="sensor-card">
          <div class="sensor-label">Температура (THS)</div>
          <div class="sensor-value" id="temp-ths">-- <span class="sensor-unit">°C</span></div>
          <div id="status-ths-temp" class="status-badge status-err">Нет данных</div>
        </div>
        <div class="sensor-card">
          <div class="sensor-label">Влажность (THS)</div>
          <div class="sensor-value" id="hum-ths">-- <span class="sensor-unit">%</span></div>
          <div id="status-ths-hum" class="status-badge status-err">Нет данных</div>
        </div>
        <div class="sensor-card">
          <div class="sensor-label">Температура (PT100)</div>
          <div class="sensor-value" id="temp-pt100">-- <span class="sensor-unit">°C</span></div>
          <div id="status-pt100" class="status-badge status-err">Нет данных</div>
        </div>
      </div>

      <div class="card" style="margin-top:20px">
        <div class="card-title"><span class="icon">⚡</span> Реле</div>
        <div class="relay-grid" id="relay-grid"></div>
      </div>

      <div class="card">
        <div class="card-title"><span class="icon">🔌</span> Входы</div>
        <div class="input-grid" id="input-grid"></div>
      </div>
    </div>

    <!-- ===== NETWORK PAGE ===== -->
    <div id="page-network" class="page hidden">
      <div class="card">
        <div class="card-title"><span class="icon">🌐</span> Сетевые настройки (Ethernet)</div>
        <div class="form-group">
          <label>IP-адрес</label>
          <input id="net-ip" type="text" placeholder="192.168.1.100">
        </div>
        <div class="form-group">
          <label>Маска подсети</label>
          <input id="net-mask" type="text" placeholder="255.255.255.0">
        </div>
        <div class="form-row">
          <div class="form-group">
            <label>Шлюз</label>
            <input id="net-gw" type="text" placeholder="192.168.1.1">
          </div>
          <div class="form-group">
            <label>DNS</label>
            <input id="net-dns" type="text" placeholder="192.168.1.1">
          </div>
        </div>
        <div class="btn-row">
          <button class="btn btn-primary" onclick="saveNetwork()">💾 Сохранить</button>
          <button class="btn btn-secondary" onclick="resetNetwork()">↺ Сброс</button>
        </div>
        <div class="save-status" id="net-status"></div>
        <div style="margin-top:20px;padding:12px;background:var(--bg);border-radius:8px;font-size:.9rem">
          <div><b>Wi-Fi AP:</b> <span id="net-ap-ip">192.168.4.1</span></div>
          <div><b>Ethernet:</b> <span id="net-eth-status">—</span></div>
        </div>
      </div>
    </div>

    <!-- ===== SECURITY PAGE ===== -->
    <div id="page-security" class="page hidden">
      <div class="card">
        <div class="card-title"><span class="icon">🔒</span> Смена логина и пароля</div>
        <p style="opacity:.7;margin-bottom:16px">
          Логин и пароль используются для доступа к веб-интерфейсу по любому интерфейсу (AP, Ethernet).
        </p>
        <div class="form-group">
          <label>Логин</label>
          <input id="auth-user" type="text" placeholder="admin">
        </div>
        <div class="form-group">
          <label>Новый пароль</label>
          <input id="auth-pass" type="password" placeholder="••••••">
        </div>
        <div class="form-group">
          <label>Подтвердите пароль</label>
          <input id="auth-pass2" type="password" placeholder="••••••">
        </div>
        <button class="btn btn-primary" onclick="saveAuth()">💾 Сохранить</button>
        <div class="save-status" id="auth-status"></div>
      </div>
    </div>

    <!-- ===== THEME PAGE ===== -->
    <div id="page-theme" class="page hidden">
      <div class="card">
        <div class="card-title"><span class="icon">🎨</span> Тема интерфейса</div>
        <div class="theme-options" id="theme-options"></div>
        <button class="btn btn-primary" style="margin-top:16px" onclick="saveTheme()">💾 Сохранить</button>
        <div class="save-status" id="theme-status"></div>
      </div>
    </div>

    <!-- ===== WIFI PAGE ===== -->
    <div id="page-wifi" class="page hidden">
      <div class="card">
        <div class="card-title"><span class="icon">📶</span> Точка доступа Wi-Fi</div>
        <p style="opacity:.7;margin-bottom:16px;color:#e74c3c">
          ⚠️ Внимание! После сохранения контроллер перезагрузится. Устройство, с которого вы вносите изменения, отключится от сети. Вам нужно будет переподключиться к новой точке доступа.
        </p>
        <div class="form-group">
          <label>Имя сети (SSID)</label>
          <input id="wifi-ssid" type="text" placeholder="PIE_Controller">
        </div>
        <div class="form-group">
          <label>Пароль (минимум 8 символов)</label>
          <input id="wifi-pass" type="text" placeholder="••••••••">
        </div>
        <button class="btn btn-primary" onclick="saveWifi()">💾 Сохранить и перезагрузить</button>
        <div class="save-status" id="wifi-status"></div>
      </div>
    </div>

  </div>
</div>

<script>
const RELAY_COUNT = )rawliteral" RELAY_COUNT_STR R"rawliteral(;
const INPUT_COUNT = )rawliteral" INPUT_COUNT_STR R"rawliteral(;

const themes = [
  {id:0, name:'Светлая', className:'', colors:['#f0f2f5','#e94560','#2c3e50']},
  {id:1, name:'Тёмная', className:'dark', colors:['#1a1a2e','#e94560','#0f3460']},
  {id:2, name:'Синяя', className:'theme-blue', colors:['#e3f2fd','#1976d2','#42a5f5']},
  {id:3, name:'Зелёная', className:'theme-green', colors:['#e8f5e9','#2e7d32','#66bb6a']},
  {id:4, name:'Фиолетовая', className:'theme-purple', colors:['#f3e5f5','#9c27b0','#ce93d8']},
  {id:5, name:'Оранжевая', className:'theme-orange', colors:['#fff3e0','#ff9800','#ffb74d']},
  {id:6, name:'Киберпанк', className:'theme-cyberpunk', colors:['#0d0221','#ff00ff','#00ffff']}
];

let selectedTheme = 0, currentTheme = 0;
let relayState = new Array(RELAY_COUNT).fill(false);
let inputState = new Array(INPUT_COUNT).fill(false);

function showPage(name){
  document.querySelectorAll('.page').forEach(p=>p.classList.add('hidden'));
  document.getElementById('page-'+name).classList.remove('hidden');
  document.querySelectorAll('.menu-item').forEach(m=>m.classList.remove('active'));
  document.querySelector(`.menu-item[data-page="${name}"]`).classList.add('active');
  document.getElementById('page-title').textContent = 
    {monitor:'Мониторинг', network:'Сеть', security:'Безопасность', theme:'Тема'}[name];
  if(name==='network') loadNetwork();
  if(name==='theme') renderThemeOptions();
  if(name==='security') loadAuth();
  if(name==='wifi') loadWifi();
}

/* --- Theme --- */
function applyTheme(id){
  const t = themes[id] || themes[0];
  document.body.className = t.className || '';
}
function renderThemeOptions(){
  const c = document.getElementById('theme-options'); c.innerHTML='';
  themes.forEach(t=>{
    const b = document.createElement('button');
    b.className = 'theme-btn' + (selectedTheme===t.id?' selected':'');
    b.onclick = ()=>{ selectedTheme=t.id; renderThemeOptions(); applyTheme(selectedTheme); };
    const dots = t.colors.map(x=>`<span class="color-dot" style="background:${x}"></span>`).join('');
    b.innerHTML = `${t.name}<div class="theme-preview">${dots}</div>`;
    c.appendChild(b);
  });
}
async function saveTheme(){
  const s = document.getElementById('theme-status');
  s.textContent='Сохранение...'; s.style.color='var(--accent)';
  try{
    const r = await fetch(`/api/theme?set=${selectedTheme}`);
    if(r.ok){ currentTheme=selectedTheme; s.textContent='✅ Сохранено'; s.style.color='var(--green)'; }
    else throw 0;
  }catch(e){ s.textContent='❌ Ошибка'; s.style.color='var(--red)'; }
  setTimeout(()=>s.textContent='',3000);
}
/* --- Wi-Fi AP --- */
async function loadWifi() {
  try {
    const r = await fetch('/api/wifi');
    const d = await r.json();
    document.getElementById('wifi-ssid').value = d.ssid || '';
    document.getElementById('wifi-pass').value = d.pass || '';
  } catch(e){ console.error(e); }
}

async function saveWifi() {
  const s = document.getElementById('wifi-status');
  const ssid = document.getElementById('wifi-ssid').value.trim();
  const pass = document.getElementById('wifi-pass').value.trim();
  
  if (ssid.length < 1) { s.textContent='⚠️ Введите SSID'; s.style.color='#e74c3c'; return; }
  if (pass.length < 8) { s.textContent='⚠️ Пароль должен быть не менее 8 символов'; s.style.color='#e74c3c'; return; }
  
  s.textContent='Сохранение и перезагрузка...'; s.style.color='var(--accent-color)';
  
  try {
    const fd = new URLSearchParams({ ssid: ssid, pass: pass });
    const r = await fetch('/api/wifi/save', { method:'POST',
      headers:{'Content-Type':'application/x-www-form-urlencoded'}, body: fd.toString() });
    const d = await r.json();
    
    if (r.ok && d.ok) {
      s.innerHTML = '✅ Сохранено!<br><small>Контроллер перезагружается. Переподключитесь к новой сети.</small>';
      s.style.color='#27ae60';
    } else {
      s.textContent='❌ Ошибка сохранения'; s.style.color='#e74c3c';
    }
  } catch(e) { 
    // Так как ESP32 перезагрузится, соединение оборвется, и fetch выдаст ошибку. 
    // Это нормально.
    s.innerHTML = '✅ Применено!<br><small>Контроллер перезагружается. Переподключитесь к новой сети.</small>';
    s.style.color='#27ae60';
  }
}
/* --- Relays --- */
function renderRelays(){
  const g = document.getElementById('relay-grid');
  if(g.children.length !== RELAY_COUNT){
    g.innerHTML='';
    for(let i=0;i<RELAY_COUNT;i++){
      const d = document.createElement('div');
      d.className='relay-item'; d.id='relay-'+i;
      d.innerHTML = `<div class="relay-label">CH ${i+1}</div>
        <div class="relay-state" id="relay-state-${i}">OFF</div>
        <button class="relay-toggle" onclick="toggleRelay(${i})">Переключить</button>`;
      g.appendChild(d);
    }
  }
  for(let i=0;i<RELAY_COUNT;i++){
    const item = document.getElementById('relay-'+i);
    const st = document.getElementById('relay-state-'+i);
    if(relayState[i]){ item.classList.add('on'); st.textContent='ON'; }
    else { item.classList.remove('on'); st.textContent='OFF'; }
  }
}
async function toggleRelay(ch){
  const ns = !relayState[ch];
  try{
    const r = await fetch(`/api/relay?ch=${ch}&state=${ns?1:0}`);
    if(r.ok){ relayState[ch]=ns; renderRelays(); }
  }catch(e){ console.error(e); }
}

/* --- Inputs --- */
function renderInputs(){
  const g = document.getElementById('input-grid');
  if(g.children.length !== INPUT_COUNT){
    g.innerHTML='';
    for(let i=0;i<INPUT_COUNT;i++){
      const d = document.createElement('div');
      d.className='input-item'; d.id='input-'+i;
      d.innerHTML = `<div class="input-dot"></div><div class="input-label">IN ${i+1}</div>`;
      g.appendChild(d);
    }
  }
  for(let i=0;i<INPUT_COUNT;i++){
    const item = document.getElementById('input-'+i);
    if(inputState[i]) item.classList.add('active');
    else item.classList.remove('active');
  }
}

/* --- Data update --- */
async function updateAll(){
  try{
    const r = await fetch('/api/data');
    const d = await r.json();

    const t = document.getElementById('temp-ths');
    const sT = document.getElementById('status-ths-temp');
    if(d.ths_valid){ t.innerHTML=`${d.temperature_ths.toFixed(1)} <span class="sensor-unit">°C</span>`;
      sT.textContent='OK'; sT.className='status-badge status-ok'; }
    else { t.innerHTML=`-- <span class="sensor-unit">°C</span>`;
      sT.textContent='Ошибка'; sT.className='status-badge status-err'; }

    const h = document.getElementById('hum-ths');
    const sH = document.getElementById('status-ths-hum');
    if(d.ths_valid){ h.innerHTML=`${d.humidity_ths.toFixed(1)} <span class="sensor-unit">%</span>`;
      sH.textContent='OK'; sH.className='status-badge status-ok'; }
    else { h.innerHTML=`-- <span class="sensor-unit">%</span>`;
      sH.textContent='Ошибка'; sH.className='status-badge status-err'; }

    const p = document.getElementById('temp-pt100');
    const sP = document.getElementById('status-pt100');
    if(d.pt100_valid){ p.innerHTML=`${d.temperature_pt100.toFixed(1)} <span class="sensor-unit">°C</span>`;
      sP.textContent='OK'; sP.className='status-badge status-ok'; }
    else { p.innerHTML=`-- <span class="sensor-unit">°C</span>`;
      sP.textContent='Ошибка'; sP.className='status-badge status-err'; }

    for(let i=0;i<RELAY_COUNT;i++) relayState[i] = (d.relay_mask >> i) & 1 ? true : false;
    renderRelays();
    for(let i=0;i<INPUT_COUNT;i++) inputState[i] = (d.input_mask >> i) & 1 ? true : false;
    renderInputs();

    document.getElementById('top-eth').textContent = d.eth_connected ? `ETH: ${d.eth_ip}` : 'ETH: нет линка';
    document.getElementById('top-wifi').textContent = `AP: ${d.wifi_ip}`;

  }catch(e){ console.error('update failed',e); }
}

/* --- Network --- */
async function loadNetwork() {
  try {
    const r = await fetch('/api/net');
    const d = await r.json();
    document.getElementById('net-ip').value   = d.ip      || '';
    document.getElementById('net-mask').value = d.subnet  || '';
    document.getElementById('net-gw').value   = d.gateway || '';
    document.getElementById('net-dns').value  = d.dns     || '';
  } catch(e){ console.error(e); }
}

async function saveNetwork() {
  const s = document.getElementById('net-status');
  const ip   = document.getElementById('net-ip').value.trim();
  const mask = document.getElementById('net-mask').value.trim();
  const gw   = document.getElementById('net-gw').value.trim();
  const dns  = document.getElementById('net-dns').value.trim();

  if (!ip || !mask) { s.textContent='⚠️ Заполните IP и маску'; s.style.color='var(--red)'; return; }
  s.textContent='Применение...'; s.style.color='var(--accent)';

  const fd = new URLSearchParams({ ip, mask });
  if (gw) fd.append('gw', gw);
  if (dns) fd.append('dns', dns);

  try {
    const r = await fetch('/api/net/save', { method:'POST',
      headers:{'Content-Type':'application/x-www-form-urlencoded'}, body: fd.toString() });
    const d = await r.json();
    if (r.ok && d.ok) {
      s.innerHTML = '✅ Сохранено!<br><small>Перезагрузка по новому адресу...</small>';
      s.style.color='var(--green)';
      const cur = window.location.hostname;
      if (ip !== cur) setTimeout(()=>{ window.location.href = `http://${ip}`; }, 2500);
      else setTimeout(()=>{ s.textContent=''; }, 3000);
    } else throw 0;
  } catch(e) {
    s.innerHTML = '✅ Применено!<br><small>Перезагрузка...</small>';
    s.style.color='var(--green)';
    setTimeout(()=>{ window.location.href = `http://${ip}`; }, 2000);
  }
}

async function resetNetwork() {
  const s = document.getElementById('net-status');
  s.textContent='Сброс...'; s.style.color='var(--accent)';
  try {
    await fetch('/api/net/reset');
    s.innerHTML = '↺ Сброшено!<br><small>Перезагрузка...</small>';
    s.style.color='var(--green)';
    setTimeout(()=>{ window.location.href = `http://192.168.1.100`; }, 2500);
  } catch(e) { s.textContent='❌ Ошибка'; s.style.color='var(--red)'; }
}

/* --- Auth --- */
async function loadAuth() {
  try {
    const r = await fetch('/api/auth');
    const d = await r.json();
    document.getElementById('auth-user').value = d.user || '';
  } catch(e){}
}
async function saveAuth() {
  const s = document.getElementById('auth-status');
  const u = document.getElementById('auth-user').value.trim();
  const p = document.getElementById('auth-pass').value;
  const p2 = document.getElementById('auth-pass2').value;
  if (u.length < 3) { s.textContent='⚠️ Логин слишком короткий'; s.style.color='var(--red)'; return; }
  if (p.length < 3) { s.textContent='⚠️ Пароль слишком короткий'; s.style.color='var(--red)'; return; }
  if (p !== p2) { s.textContent='⚠️ Пароли не совпадают'; s.style.color='var(--red)'; return; }
  s.textContent='Сохранение...'; s.style.color='var(--accent)';
  try {
    const fd = new URLSearchParams({ user: u, pass: p });
    const r = await fetch('/api/auth/save', { method:'POST',
      headers:{'Content-Type':'application/x-www-form-urlencoded'}, body: fd.toString() });
    const d = await r.json();
    if (r.ok && d.ok) { s.textContent='✅ Сохранено'; s.style.color='var(--green)'; }
    else throw 0;
  } catch(e) { s.textContent='❌ Ошибка'; s.style.color='var(--red)'; }
  setTimeout(()=>s.textContent='', 3000);
}

window.onload = async ()=>{
  try{
    const r = await fetch('/api/theme'); const d = await r.json();
    currentTheme = d.theme||0; selectedTheme = currentTheme;
  }catch(e){ currentTheme=0; selectedTheme=0; }
  applyTheme(currentTheme);
  renderRelays(); renderInputs();
  updateAll();
  setInterval(updateAll, 2000);
};
</script>
</body>
</html>
)rawliteral";

#endif
