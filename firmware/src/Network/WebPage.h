#pragma once
#include <Arduino.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <title>Roslina SOS</title>
  <style>
    body { font-family: Helvetica, Arial, sans-serif; text-align: center; background-color: #f4f4f4; color: #333; margin: 0; padding: 20px;}
    h1 { color: #d32f2f; margin-bottom: 5px; }
    p.sub { color: #666; font-size: 0.9em; margin-bottom: 25px; }
    .container { max-width: 400px; margin: 0 auto; }
    .card { background: white; padding: 20px; margin-bottom: 15px; border-radius: 12px; box-shadow: 0 4px 10px rgba(0,0,0,0.1); }
    .val { font-size: 2em; font-weight: bold; color: #2e7d32; margin: 10px 0; }
    .label { font-size: 0.85em; text-transform: uppercase; letter-spacing: 1px; color: #888; }
    .status-box { background: #ffebee; color: #c62828; padding: 10px; border-radius: 8px; font-weight: bold; border: 1px solid #ef9a9a; }
    .btn { display: inline-block; padding: 12px 30px; background: #1976d2; color: white; text-decoration: none; border-radius: 25px; font-weight: bold; box-shadow: 0 4px 6px rgba(33,150,243,0.3); transition: 0.3s; }
    .btn:active { transform: scale(0.95); }
  </style>
</head>
<body>
  <div class="container">
    <h1>⚠️ Tryb Awaryjny</h1>
    <p class="sub">Brak polaczenia z WiFi domowym</p>

    <p class="time">🕒 %TIME%</p>
    
    <div class="card">
      <div class="label">Wilgotnosc Gleby</div>
      <div class="val">%MOISTURE% %</div>
    </div>

    <div class="card">
      <div class="label">Temperatura</div>
      <div class="val">%TEMP% &deg;C</div>
    </div>

    <div class="card">
      <div class="label">Wilgotnosc Powietrza</div>
      <div class="val">%HUM% %</div>
    </div>

    <div class="card">
      <div class="label">Cisnienie</div>
      <div class="val">%PRESS% hPa</div>
    </div>
    <div class="card">
      <div class="label">Swiatlo</div>
      <div class="val">%LUX% lx</div>
    </div>
    
    <div class="card">
      <div class="label">Status Systemu</div>
      <div style="margin-top:10px;" class="status-box">%STATUS%</div>
    </div>

    <br>
    <a href="/" class="btn">Odswiez Dane</a>
  </div>
</body>
</html>
)rawliteral";