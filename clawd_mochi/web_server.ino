// ═════════════════════════════════════════════════════════════
//  WEB ROUTES
// ═════════════════════════════════════════════════════════════

void routeRoot() {
  server.sendHeader("Cache-Control", "no-store, no-cache, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.send_P(200, "text/html", INDEX_HTML);
}

void routeCmd() {
  lastInteractionMs = millis();
  if (!server.hasArg("k") || server.arg("k").isEmpty()) {
    server.send(400, "application/json", "{\"e\":1}"); return;
  }
  const char c = server.arg("k")[0];

  server.send(200, "application/json", "{\"ok\":1}");
  switch (c) {
    case 'w': currentView = VIEW_EYES_NORMAL; animNormalEyes(); break;
    case 's': currentView = VIEW_EYES_SQUISH; animSquishEyes(); break;
    case 'a':
      currentView = VIEW_EYES_NORMAL;
      animLogoReveal();
      break;
    case 'x':
      currentView = VIEW_WEATHER;
      wxValid = false;
      drawWeatherView();
      break;
  }
  saveSettings();
}

void routeSpeed() {
  lastInteractionMs = millis();
  if (server.hasArg("v")) animSpeed = constrain(server.arg("v").toInt(), 1, 3);
  server.send(200, "application/json", "{\"ok\":1}");
}

void routeRedraw() {
  lastInteractionMs = millis();
  if (server.hasArg("bg")) {
    animBgColor = hexToRgb565(server.arg("bg"));
    saveSettings();
  }
  switch (currentView) {
    case VIEW_EYES_NORMAL: drawNormalEyes(); break;
    case VIEW_EYES_SQUISH: drawSquishEyes(); break;
    case VIEW_DRAW:        gfx->fillScreen(drawBgColor); break;
  }
  server.send(200, "application/json", "{\"ok\":1}");
}

void routeCanvas() {
  lastInteractionMs = millis();
  const bool on = server.hasArg("on") && server.arg("on") == "1";
  if (on) {
    currentView = VIEW_DRAW;
    canvasClear(drawBgColor);
    saveSettings();
  }
  server.send(200, "application/json", "{\"ok\":1}");
}

void routeDrawClear() {
  lastInteractionMs = millis();
  const String bg = server.hasArg("bg") ? server.arg("bg") : "#aa4818";
  drawBgColor = hexToRgb565(bg);
  currentView = VIEW_DRAW;
  canvasClear(drawBgColor);
  server.send(200, "application/json", "{\"ok\":1}");
}

void routeDrawStroke() {
  lastInteractionMs = millis();
  if (!server.hasArg("pts") || !server.hasArg("pen")) {
    server.send(200, "application/json", "{\"ok\":1}"); return;
  }
  const uint16_t color = hexToRgb565(server.arg("pen"));
  drawBgColor = color;  // persist pen colour
  const String   data  = server.arg("pts");
  const uint8_t  sz    = server.hasArg("sz") ? constrain(server.arg("sz").toInt(), 1, 20) : 8;
  currentView = VIEW_DRAW;
  canvasDrawStroke(data, color, sz);
  saveSettings();
  server.send(200, "application/json", "{\"ok\":1}");
}

void routeBacklight() {
  lastInteractionMs = millis();
  if (server.hasArg("bri")) {
    setBrightness(constrain(server.arg("bri").toInt(), 10, 255));
    saveSettings();
  }
  if (server.hasArg("on")) {
    setBacklight(server.arg("on") == "1");
  }
  server.send(200, "application/json", "{\"ok\":1}");
}

void routeState() {
  String j = "{\"view\":"; j += currentView;
  j += ",\"busy\":";   j += busy        ? "true" : "false";
  j += ",\"bl\":";     j += backlightOn ? "true" : "false";
  j += ",\"bri\":";    j += brightness;
  j += ",\"speed\":";  j += animSpeed;
  j += ",\"sta\":";    j += wifiIsStaConnected() ? "true" : "false";
  j += ",\"sta_ip\":\""; j += staIP; j += "\"";
  j += ",\"bg\":\"";  j += rgb565ToHex(animBgColor); j += "\"";
  j += ",\"pen\":\""; j += rgb565ToHex(drawBgColor); j += "\"";
  j += "}";
  server.send(200, "application/json", j);
}

void routeTicker() {
  lastInteractionMs = millis();

  // Stop ticker
  if (server.hasArg("action") && server.arg("action") == "stop") {
    tickerReturnToEyes();
    server.send(200, "application/json", "{\"ok\":1}");
    return;
  }

  // Update bg/pen colours if provided
  if (server.hasArg("bg"))  animBgColor = hexToRgb565(server.arg("bg"));
  if (server.hasArg("pen")) drawBgColor = hexToRgb565(server.arg("pen"));

  // Start ticker
  String text = server.hasArg("text") ? server.arg("text") : "";
  if (text.length() == 0) {
    server.send(400, "application/json", "{\"e\":\"no text\"}"); return;
  }
  uint8_t speed    = server.hasArg("speed") ? constrain(server.arg("speed").toInt(), 1, 5) : 2;
  uint8_t textSize = server.hasArg("size")  ? constrain(server.arg("size").toInt(), 1, 5)  : 3;

  tickerStart(text, speed, textSize);
  server.send(200, "application/json", "{\"ok\":1}");
}

void routeWifiConfig() {
  lastInteractionMs = millis();
  if (server.method() == HTTP_POST || server.hasArg("ssid")) {
    String ssid = server.arg("ssid");
    String pass = server.arg("pass");
    if (ssid.length() == 0) {
      server.send(400, "application/json", "{\"e\":\"no ssid\"}"); return;
    }
    wifiSaveCredentials(ssid, pass);
    server.send(200, "application/json", "{\"ok\":1,\"msg\":\"saved, reboot to connect\"}");
    return;
  }
  // GET — return status
  String j = "{\"connected\":"; j += wifiIsStaConnected() ? "true" : "false";
  j += ",\"ssid\":\""; j += staSSID;
  j += "\",\"ip\":\""; j += staIP;
  j += "\"}";
  server.send(200, "application/json", j);
}

void routeWeatherConfig() {
  lastInteractionMs = millis();
  if (server.hasArg("key") || server.hasArg("lat") || server.hasArg("lon")) {
    if (server.hasArg("key")) wxSetPref("owm_key", server.arg("key"));
    if (server.hasArg("lat")) wxSetPref("owm_lat", server.arg("lat"));
    if (server.hasArg("lon")) wxSetPref("owm_lon", server.arg("lon"));
    server.send(200, "application/json", "{\"ok\":1}");
    return;
  }
  // GET — return current config (mask API key)
  String key = wxGetPref("owm_key");
  String j = "{\"hasKey\":"; j += key.length() > 0 ? "true" : "false";
  j += ",\"lat\":\""; j += wxGetPref("owm_lat");
  j += "\",\"lon\":\""; j += wxGetPref("owm_lon");
  j += "\"}";
  server.send(200, "application/json", j);
}

void routeAPConfig() {
  lastInteractionMs = millis();
  if (server.hasArg("ssid")) {
    String ssid = server.arg("ssid");
    String pass = server.hasArg("pass") ? server.arg("pass") : "";
    if (ssid.length() == 0) {
      server.send(400, "application/json", "{\"e\":\"no ssid\"}"); return;
    }
    wifiSaveAPConfig(ssid, pass);
    server.send(200, "application/json", "{\"ok\":1,\"msg\":\"saved, reboot to apply\"}");
    return;
  }
  String j = "{\"ssid\":\""; j += apSSID;
  j += "\"}";
  server.send(200, "application/json", j);
}

void routeNotFound() { server.send(404, "text/plain", "not found"); }

// ── Route registration ───────────────────────────────────────

void registerRoutes() {
  server.on("/",            HTTP_GET, routeRoot);
  server.on("/cmd",         HTTP_GET, routeCmd);
  server.on("/speed",       HTTP_GET, routeSpeed);
  server.on("/redraw",      HTTP_GET, routeRedraw);
  server.on("/canvas",      HTTP_GET, routeCanvas);
  server.on("/draw/clear",  HTTP_GET, routeDrawClear);
  server.on("/draw/stroke", HTTP_GET, routeDrawStroke);
  server.on("/backlight",   HTTP_GET, routeBacklight);
  server.on("/state",       HTTP_GET, routeState);
  server.on("/ticker",      HTTP_GET, routeTicker);
  server.on("/config/wifi",    HTTP_ANY, routeWifiConfig);
  server.on("/config/ap",      HTTP_ANY, routeAPConfig);
  server.on("/config/weather", HTTP_ANY, routeWeatherConfig);
  server.onNotFound(routeNotFound);
}
