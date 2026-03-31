// ═════════════════════════════════════════════════════════════
//  WiFi Manager — AP + STA dual mode with NVS credentials
// ═════════════════════════════════════════════════════════════

static const char* AP_SSID_DEFAULT = "ClaWD-Mochi";
static const char* AP_PASS_DEFAULT = "clawd1234";

static Preferences prefs;
String apSSID = "";
String apPass = "";
bool staConnected = false;
String staSSID = "";
String staIP   = "";

void wifiLoadAPConfig() {
  prefs.begin("clawd", true);
  apSSID = prefs.getString("ap_ssid", AP_SSID_DEFAULT);
  apPass = prefs.getString("ap_pass", AP_PASS_DEFAULT);
  prefs.end();
}

void wifiSaveAPConfig(const String& ssid, const String& pass) {
  prefs.begin("clawd", false);
  prefs.putString("ap_ssid", ssid);
  prefs.putString("ap_pass", pass);
  prefs.end();
  apSSID = ssid;
  apPass = pass;
}

void wifiTryConnect() {
  prefs.begin("clawd", true);
  String ssid = prefs.getString("sta_ssid", "");
  String pass = prefs.getString("sta_pass", "");
  prefs.end();

  if (ssid.length() == 0) return;

  staSSID = ssid;
  Serial.print("Connecting to: "); Serial.println(ssid);
  WiFi.begin(ssid.c_str(), pass.c_str());
  WiFi.setAutoReconnect(true);

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
  prefs.begin("clawd", false);
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
  wifiLoadAPConfig();

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(apSSID.c_str(), apPass.c_str());

  wifiTryConnect();

  // WiFi info screen
  gfx->fillScreen(C_DARKBG);
  gfx->fillRect(0, 0, DISP_W, 4, C_ORANGE);
  gfx->setTextColor(C_WHITE);  gfx->setTextSize(2);
  gfx->setCursor(12, 16);  gfx->print("WiFi: "); gfx->print(apSSID);
  gfx->setTextColor(C_MUTED);  gfx->setTextSize(1);
  gfx->setCursor(12, 44);  gfx->print("password: "); gfx->print(apPass);
  gfx->setTextColor(C_WHITE);  gfx->setTextSize(2);
  gfx->setCursor(12, 68);  gfx->print("Open browser:");
  gfx->setTextColor(C_ORANGE); gfx->setTextSize(2);
  gfx->setCursor(12, 94);  gfx->print("192.168.4.1");

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
