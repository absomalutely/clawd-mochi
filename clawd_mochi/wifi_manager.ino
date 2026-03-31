// ═════════════════════════════════════════════════════════════
//  WiFi Manager
// ═════════════════════════════════════════════════════════════

static const char* AP_SSID = "ClaWD-Mochi";
static const char* AP_PASS = "clawd1234";

void setupWiFi() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);

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
  gfx->setTextColor(C_MUTED);  gfx->setTextSize(1);
  gfx->setCursor(12, 124); gfx->print("press any button to start");
}
