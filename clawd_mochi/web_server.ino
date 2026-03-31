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

  if (termMode) {
    if (c == 'q') { termMode = false; drawCodeView(); }
    server.send(200, "application/json", "{\"ok\":1}"); return;
  }

  server.send(200, "application/json", "{\"ok\":1}");
  switch (c) {
    case 'w': currentView = VIEW_EYES_NORMAL; animNormalEyes(); break;
    case 's': currentView = VIEW_EYES_SQUISH; animSquishEyes(); break;
    case 'd':
      currentView = VIEW_CODE; drawCodeView();
      termMode = true; termClear(); termFullRedraw(); break;
    case 'a':
      currentView = VIEW_EYES_NORMAL;
      animLogoReveal();
      break;
    case 'x':
      currentView = VIEW_WEATHER;
      wxValid = false;  // force refresh
      drawWeatherView();
      break;
  }
}

void routeChar() {
  lastInteractionMs = millis();
  if (!termMode) { server.send(200, "application/json", "{\"ok\":1}"); return; }
  const String val = server.arg("c");
  if (val.length() > 0) termAddChar(val[0]);
  server.send(200, "application/json", "{\"ok\":1}");
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
    drawBgColor = animBgColor;
  }
  switch (currentView) {
    case VIEW_EYES_NORMAL: drawNormalEyes(); break;
    case VIEW_EYES_SQUISH: drawSquishEyes(); break;
    case VIEW_CODE:        drawCodeView();   break;
    case VIEW_DRAW:        gfx->fillScreen(drawBgColor); break;
  }
  server.send(200, "application/json", "{\"ok\":1}");
}

void routeCanvas() {
  lastInteractionMs = millis();
  const bool on = server.hasArg("on") && server.arg("on") == "1";
  if (on) { currentView = VIEW_DRAW; gfx->fillScreen(drawBgColor); }
  server.send(200, "application/json", "{\"ok\":1}");
}

void routeDrawClear() {
  lastInteractionMs = millis();
  const String bg = server.hasArg("bg") ? server.arg("bg") : "#aa4818";
  drawBgColor = hexToRgb565(bg);
  animBgColor = drawBgColor;
  currentView = VIEW_DRAW; termMode = false;
  gfx->fillScreen(drawBgColor);
  server.send(200, "application/json", "{\"ok\":1}");
}

void routeDrawStroke() {
  lastInteractionMs = millis();
  if (!server.hasArg("pts") || !server.hasArg("pen")) {
    server.send(200, "application/json", "{\"ok\":1}"); return;
  }
  const uint16_t color = hexToRgb565(server.arg("pen"));
  const String   data  = server.arg("pts");
  currentView = VIEW_DRAW;

  struct Pt { int16_t x, y; };
  Pt prev = {-1, -1};
  int start = 0;
  while (start < (int)data.length()) {
    int semi = data.indexOf(';', start);
    if (semi == -1) semi = data.length();
    String entry = data.substring(start, semi);
    const int comma = entry.indexOf(',');
    if (comma > 0) {
      const int16_t x = entry.substring(0, comma).toInt();
      const int16_t y = entry.substring(comma + 1).toInt();
      if (prev.x >= 0) {
        gfx->drawLine(prev.x, prev.y, x, y, color);
        gfx->drawLine(prev.x + 1, prev.y, x + 1, y, color);
        gfx->drawLine(prev.x, prev.y + 1, x, y + 1, color);
      } else {
        gfx->fillCircle(x, y, 2, color);
      }
      prev = {x, y};
    }
    start = semi + 1;
  }
  server.send(200, "application/json", "{\"ok\":1}");
}

void routeBacklight() {
  lastInteractionMs = millis();
  setBacklight(server.hasArg("on") && server.arg("on") == "1");
  server.send(200, "application/json", "{\"ok\":1}");
}

void routeState() {
  String j = "{\"view\":"; j += currentView;
  j += ",\"busy\":";   j += busy        ? "true" : "false";
  j += ",\"term\":";   j += termMode    ? "true" : "false";
  j += ",\"bl\":";     j += backlightOn ? "true" : "false";
  j += ",\"speed\":";  j += animSpeed;
  j += ",\"sta\":";    j += wifiIsStaConnected() ? "true" : "false";
  j += ",\"sta_ip\":\""; j += staIP; j += "\"";
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

void routeNotFound() { server.send(404, "text/plain", "not found"); }

// ── Route registration ───────────────────────────────────────

void registerRoutes() {
  server.on("/",            HTTP_GET, routeRoot);
  server.on("/cmd",         HTTP_GET, routeCmd);
  server.on("/char",        HTTP_GET, routeChar);
  server.on("/speed",       HTTP_GET, routeSpeed);
  server.on("/redraw",      HTTP_GET, routeRedraw);
  server.on("/canvas",      HTTP_GET, routeCanvas);
  server.on("/draw/clear",  HTTP_GET, routeDrawClear);
  server.on("/draw/stroke", HTTP_GET, routeDrawStroke);
  server.on("/backlight",   HTTP_GET, routeBacklight);
  server.on("/state",       HTTP_GET, routeState);
  server.on("/ticker",      HTTP_GET, routeTicker);
  server.on("/config/wifi",    HTTP_ANY, routeWifiConfig);
  server.on("/config/weather", HTTP_ANY, routeWeatherConfig);
  server.onNotFound(routeNotFound);
}
