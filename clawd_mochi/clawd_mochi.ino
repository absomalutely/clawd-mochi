/*
 * ╔══════════════════════════════════════════════════════════════╗
 *   CLAWD MOCHI — ESP32-C3 Super Mini + ST7789 1.54" 240×240
 *
 *   Wiring:
 *     SDA → GPIO 5   (hardware SPI MOSI)
 *     SCL → GPIO 4   (hardware SPI SCK)
 *     RST → GPIO 0
 *     DC  → GPIO 1
 *     BLK → GPIO 10  (PWM backlight)
 *     VCC → 3V3
 *     GND → GND
 *
 *   WiFi: "ClaWD-Mochi"  pw: clawd1234  → http://192.168.4.1
 * ╚══════════════════════════════════════════════════════════════╝
 */

#include <Arduino_GFX_Library.h>
#include <SPI.h>
#include <math.h>
#include <WiFi.h>
#include <WebServer.h>

#include "config.h"
#include "display.h"
#include "state.h"
#include "logo_data.h"
#include "web_ui.h"

// ── Forward declarations (functions with default params) ──────
void drawNormalEyes(int16_t ox = 0, bool blink = false);
void drawSquishEyes(bool closed = false);

// ── Global objects ────────────────────────────────────────────
Arduino_DataBus *bus = new Arduino_HWSPI(
  TFT_DC,  // DC
  -1,      // CS
  4,       // SCLK
  5,       // MOSI
  -1       // MISO
);

Arduino_GFX *gfx = new Arduino_ST7789(
  bus,
  TFT_RST,  // RST
  0,        // rotation
  true,     // IPS
  240,      // width
  240       // height
);

WebServer server(80);

// ── Global state ──────────────────────────────────────────────
uint16_t C_ORANGE, C_DARKBG, C_MUTED, C_GREEN;
uint8_t  currentView  = VIEW_EYES_NORMAL;
bool     busy         = false;
bool     backlightOn  = true;
uint8_t  animSpeed    = 1;
uint16_t animBgColor  = 0;
uint16_t drawBgColor  = 0;
bool     termMode     = false;
unsigned long lastInteractionMs = 0;

// ── Terminal state ────────────────────────────────────────────
String  termLines[TERM_ROWS];
uint8_t termRow = 0;
uint8_t termCol = 0;

// ═════════════════════════════════════════════════════════════
//  SETUP
// ═════════════════════════════════════════════════════════════

void setup() {
  Serial.begin(115200);

  // PWM backlight init
  ledcAttach(TFT_BLK, PWM_FREQ, PWM_RES);
  setBacklight(true);

  // Init display
  gfx->begin();
  gfx->setRotation(1);
  initColours();

  // Boot splash + logo reveal
  bootSplash();

  // Start WiFi
  setupWiFi();

  // Register web routes and start server
  registerRoutes();
  server.begin();

  // Draw initial view so screen is ready before idle animations
  drawNormalEyes();
  lastInteractionMs = millis();
}

// ═════════════════════════════════════════════════════════════
//  LOOP
// ═════════════════════════════════════════════════════════════

void loop() {
  server.handleClient();
  loopIdle();
  loopTicker();
}
