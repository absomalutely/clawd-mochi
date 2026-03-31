// ═════════════════════════════════════════════════════════════
//  WiFi Manager — AP + STA dual mode with NVS credentials
// ═════════════════════════════════════════════════════════════

static const char* AP_SSID = "ClaWD-Mochi";
static const char* AP_PASS = "clawd1234";

static Preferences prefs;
bool staConnected = false;
String staSSID = "";
String staIP   = "";

void wifiTryConnect() {
  prefs.begin("clawd", true);  // read-only
  String ssid = prefs.getString("sta_ssid", "");
  String pass = prefs.getString("sta_pass", "");
  prefs.end();

  if (ssid.length() == 0) return;

  staSSID = ssid;
  Serial.print("Connecting to: "); Serial.println(ssid);
  WiFi.begin(ssid.c_str(), pass.c_str());
  WiFi.setAutoReconnect(true);

  // Non-blocking wait — try for up to 10 seconds
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
    delay(250);
  }

  if (WiFi.status() == WL_CONNECTED) {
    staConnected = true;
    staIP = WiFi.localIP().toString();
    Serial.print("Connected! IP: "); Serial.println(staIP);
  } else {
    Serial.println("STA connection failed — continuing AP-only");
  }
}

void wifiSaveCredentials(const String& ssid, const String& pass) {
  prefs.begin("clawd", false);  // read-write
  prefs.putString("sta_ssid", ssid);
  prefs.putString("sta_pass", pass);
  prefs.end();
}

bool wifiIsStaConnected() {
  staConnected = (WiFi.status() == WL_CONNECTED);
  if (staConnected) staIP = WiFi.localIP().toString();
  return staConnected;
}

void setupWiFi() {
  // Dual mode: AP for controller + STA for internet
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(AP_SSID, AP_PASS);

  // Try connecting to saved home network
  wifiTryConnect();

  // WiFi info screen
  gfx->fillScreen(C_DARKBG);
  gfx->fillRect(0, 0, DISP_W, 4, C_ORANGE);
  gfx->setTextColor(C_WHITE);  gfx->setTextSize(2);
  gfx->setCursor(12, 16);  gfx->print("WiFi: ClaWD-Mochi");
  gfx->setTextColor(C_MUTED);  gfx->setTextSize(1);
  gfx->setCursor(12, 44);  gfx->print("password: clawd1234");
  gfx->setTextColor(C_WHITE);  gfx->setTextSize(2);
  gfx->setCursor(12, 68);  gfx->print("Open browser:");
  gfx->setTextColor(C_ORANGE); gfx->setTextSize(2);
  gfx->setCursor(12, 94);  gfx->print("192.168.4.1");

  // Show STA status
  if (staConnected) {
    gfx->setTextColor(C_GREEN); gfx->setTextSize(1);
    gfx->setCursor(12, 124); gfx->print("Home WiFi: connected");
    gfx->setCursor(12, 138); gfx->print(staIP);
  } else if (staSSID.length() > 0) {
    gfx->setTextColor(C_ORANGE); gfx->setTextSize(1);
    gfx->setCursor(12, 124); gfx->print("Home WiFi: not found");
  } else {
    gfx->setTextColor(C_MUTED); gfx->setTextSize(1);
    gfx->setCursor(12, 124); gfx->print("No home WiFi configured");
  }
}
