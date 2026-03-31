// ═════════════════════════════════════════════════════════════
//  VIEWS — Weather Widget
// ═════════════════════════════════════════════════════════════

static float   wxTemp     = 0;
static float   wxTempMin  = 0;
static float   wxTempMax  = 0;
static uint8_t wxHumidity = 0;
static String  wxCondition  = "";
static String  wxDescription = "";
bool    wxValid    = false;
static unsigned long wxLastFetchMs = 0;

#define WX_INTERVAL_MS 600000  // 10 minutes

// ── NVS helpers ──────────────────────────────────────────────

String wxGetPref(const char* key) {
  Preferences p;
  p.begin("clawd", true);
  String val = p.getString(key, "");
  p.end();
  return val;
}

void wxSetPref(const char* key, const String& val) {
  Preferences p;
  p.begin("clawd", false);
  p.putString(key, val);
  p.end();
}

// ── API fetch ────────────────────────────────────────────────

void weatherFetch() {
  if (!wifiIsStaConnected()) return;

  String key = wxGetPref("owm_key");
  String lat = wxGetPref("owm_lat");
  String lon = wxGetPref("owm_lon");
  if (key.length() == 0 || lat.length() == 0) return;

  WiFiClient client;
  HTTPClient http;
  String url = "http://api.openweathermap.org/data/2.5/weather?lat=";
  url += lat; url += "&lon="; url += lon;
  url += "&appid="; url += key; url += "&units=metric";

  Serial.print("Weather fetch: "); Serial.println(url);
  http.begin(client, url);
  int code = http.GET();
  Serial.print("HTTP code: "); Serial.println(code);

  if (code == 200) {
    String payload = http.getString();
    Serial.println("=== RAW WEATHER RESPONSE ===");
    Serial.println(payload);
    Serial.println("=== END RESPONSE ===");
    JsonDocument doc;
    if (!deserializeJson(doc, payload)) {
      wxTemp        = doc["main"]["temp"] | 0.0f;
      wxTempMin     = doc["main"]["temp_min"] | 0.0f;
      wxTempMax     = doc["main"]["temp_max"] | 0.0f;
      wxHumidity    = doc["main"]["humidity"] | 0;
      wxCondition   = doc["weather"][0]["main"].as<String>();
      wxDescription = doc["weather"][0]["description"].as<String>();
      wxValid       = true;
      wxLastFetchMs = millis();
      Serial.println("Weather OK: " + wxCondition + " " + String(wxTemp) + "C");
    }
  }
  http.end();
}

// ── Weather icons (GFX primitives) ───────────────────────────

static uint16_t wxYellow, wxLightBlue, wxGrey, wxLightGrey;

void wxInitColours() {
  wxYellow    = gfx->color565(255, 200, 50);
  wxLightBlue = gfx->color565(100, 160, 255);
  wxGrey      = gfx->color565(140, 140, 140);
  wxLightGrey = gfx->color565(200, 200, 200);
}

void wxIconSun(int16_t cx, int16_t cy) {
  gfx->fillCircle(cx, cy, 28, wxYellow);
  for (int i = 0; i < 8; i++) {
    float a = i * PI / 4.0;
    int16_t x1 = cx + cos(a) * 34;
    int16_t y1 = cy + sin(a) * 34;
    int16_t x2 = cx + cos(a) * 46;
    int16_t y2 = cy + sin(a) * 46;
    gfx->drawLine(x1, y1, x2, y2, wxYellow);
    gfx->drawLine(x1+1, y1, x2+1, y2, wxYellow);
    gfx->drawLine(x1, y1+1, x2, y2+1, wxYellow);
  }
}

void wxIconCloud(int16_t cx, int16_t cy, uint16_t col) {
  gfx->fillCircle(cx - 16, cy + 6, 22, col);
  gfx->fillCircle(cx + 18, cy + 3, 25, col);
  gfx->fillCircle(cx, cy - 10, 19, col);
  gfx->fillRect(cx - 38, cy + 6, 78, 22, col);
}

void wxIconRain(int16_t cx, int16_t cy) {
  wxIconCloud(cx, cy - 12, wxGrey);
  for (int i = 0; i < 5; i++) {
    int16_t x = cx - 24 + i * 12;
    gfx->drawLine(x, cy + 18, x - 5, cy + 34, wxLightBlue);
    gfx->drawLine(x + 1, cy + 18, x - 4, cy + 34, wxLightBlue);
    gfx->drawLine(x + 2, cy + 18, x - 3, cy + 34, wxLightBlue);
  }
}

void wxIconDrizzle(int16_t cx, int16_t cy) {
  wxIconCloud(cx, cy - 12, wxGrey);
  for (int i = 0; i < 4; i++) {
    int16_t x = cx - 18 + i * 12;
    gfx->fillCircle(x, cy + 20, 2, wxLightBlue);
    gfx->fillCircle(x + 5, cy + 30, 2, wxLightBlue);
  }
}

void wxIconThunder(int16_t cx, int16_t cy) {
  wxIconCloud(cx, cy - 14, gfx->color565(80, 80, 90));
  // Bigger lightning bolt
  gfx->fillTriangle(cx - 2, cy + 8, cx + 8, cy + 8, cx, cy + 22, wxYellow);
  gfx->fillTriangle(cx - 8, cy + 20, cx + 2, cy + 20, cx - 4, cy + 36, wxYellow);
}

void wxIconSnow(int16_t cx, int16_t cy) {
  wxIconCloud(cx, cy - 12, wxLightGrey);
  for (int i = 0; i < 5; i++) {
    int16_t x = cx - 24 + i * 12;
    int16_t y = cy + 18 + (i % 2) * 10;
    gfx->fillCircle(x, y, 3, C_WHITE);
  }
}

void wxIconMist(int16_t cx, int16_t cy) {
  for (int i = 0; i < 5; i++) {
    uint16_t c = (i % 2 == 0) ? wxGrey : wxLightGrey;
    int16_t w = 70 - i * 6;
    gfx->drawFastHLine(cx - w / 2, cy - 16 + i * 10, w, c);
    gfx->drawFastHLine(cx - w / 2, cy - 15 + i * 10, w, c);
    gfx->drawFastHLine(cx - w / 2, cy - 14 + i * 10, w, c);
  }
}

void wxDrawIcon(int16_t cx, int16_t cy) {
  if      (wxCondition == "Clear")        wxIconSun(cx, cy);
  else if (wxCondition == "Clouds")       wxIconCloud(cx, cy, wxLightGrey);
  else if (wxCondition == "Rain")         wxIconRain(cx, cy);
  else if (wxCondition == "Drizzle")      wxIconDrizzle(cx, cy);
  else if (wxCondition == "Thunderstorm") wxIconThunder(cx, cy);
  else if (wxCondition == "Snow")         wxIconSnow(cx, cy);
  else                                    wxIconMist(cx, cy);
}

// ── Thermometer icon ─────────────────────────────────────────

void wxDrawThermometer(int16_t cx, int16_t cy) {
  // Bulb at bottom
  gfx->fillCircle(cx, cy + 18, 8, gfx->color565(220, 50, 50));
  // Stem
  gfx->fillRect(cx - 3, cy - 12, 6, 30, gfx->color565(220, 50, 50));
  // Outline
  gfx->drawRect(cx - 5, cy - 14, 10, 30, C_BLACK);
  gfx->drawCircle(cx, cy + 18, 10, C_BLACK);
  // Tick marks
  for (int i = 0; i < 3; i++) {
    gfx->drawFastHLine(cx + 5, cy - 10 + i * 8, 4, C_BLACK);
  }
}

// ── Draw weather view ────────────────────────────────────────

void drawWeatherView() {
  gfx->fillScreen(animBgColor);

  if (!wxValid) {
    gfx->setTextColor(C_BLACK); gfx->setTextSize(3);
    if (!wifiIsStaConnected()) {
      gfx->setCursor(30, 90); gfx->print("No WiFi");
      gfx->setTextSize(2);
      gfx->setCursor(12, 140); gfx->print("Configure in");
      gfx->setCursor(12, 165); gfx->print("web controller");
    } else {
      String key = wxGetPref("owm_key");
      if (key.length() == 0) {
        gfx->setCursor(24, 90); gfx->print("Setup");
        gfx->setCursor(24, 125); gfx->print("needed");
        gfx->setTextSize(2);
        gfx->setCursor(12, 170); gfx->print("Set API key");
      } else {
        gfx->setCursor(12, 100); gfx->print("Fetching...");
      }
    }
    return;
  }

  wxInitColours();

  // Big weather icon — centred, upper half
  wxDrawIcon(120, 60);

  // Description text below icon
  gfx->setTextColor(C_BLACK); gfx->setTextSize(2);
  int16_t descW = wxDescription.length() * 12;
  gfx->setCursor(120 - descW / 2, 100);
  gfx->print(wxDescription);

  // Thermometer icon + temperature — big and bold, lower half
  wxDrawThermometer(40, 155);

  gfx->setTextColor(C_BLACK); gfx->setTextSize(5);
  char tempBuf[10];
  dtostrf(wxTemp, 0, 1, tempBuf);
  String tempStr = String(tempBuf) + "C";
  gfx->setCursor(65, 140);
  gfx->print(tempStr);
}

// ── Loop (auto-refresh) ─────────────────────────────────────

void loopWeather() {
  if (currentView != VIEW_WEATHER) return;

  if (!wxValid || (millis() - wxLastFetchMs > WX_INTERVAL_MS)) {
    weatherFetch();
    drawWeatherView();
  }
}
