// ═════════════════════════════════════════════════════════════
//  VIEWS — Eyes (Normal + Squish)
// ═════════════════════════════════════════════════════════════

inline int16_t eyeLX(int16_t ox) {
  return (DISP_W - (EYE_W * 2 + EYE_GAP)) / 2 + EYE_OX + ox;
}
inline int16_t eyeRX(int16_t ox) { return eyeLX(ox) + EYE_W + EYE_GAP; }
inline int16_t eyeY()            { return (DISP_H - EYE_H) / 2 - EYE_OY; }
inline int16_t eyeCY()           { return eyeY() + EYE_H / 2; }

void drawNormalEyes(int16_t ox, bool blink) {
  gfx->fillScreen(animBgColor);
  const int16_t lx = eyeLX(ox), rx = eyeRX(ox), ey = eyeY();
  if (!blink) {
    gfx->fillRect(lx, ey, EYE_W, EYE_H, C_BLACK);
    gfx->fillRect(rx, ey, EYE_W, EYE_H, C_BLACK);
  } else {
    gfx->fillRect(lx, ey + EYE_H / 2 - 3, EYE_W, 6, C_BLACK);
    gfx->fillRect(rx, ey + EYE_H / 2 - 3, EYE_W, 6, C_BLACK);
  }
}

// Draw normal eyes with custom height (for sleepy/surprised)
void drawNormalEyesH(int16_t ox, int16_t h) {
  gfx->fillScreen(animBgColor);
  const int16_t lx = eyeLX(ox), rx = eyeRX(ox);
  const int16_t cy = eyeY() + EYE_H / 2;
  const int16_t ey = cy - h / 2;
  gfx->fillRect(lx, ey, EYE_W, h, C_BLACK);
  gfx->fillRect(rx, ey, EYE_W, h, C_BLACK);
}

void drawChevron(int16_t cx, int16_t cy, int16_t arm, int16_t reach,
                 uint8_t thk, bool rightFacing, uint16_t col) {
  for (int8_t t = -(int8_t)thk; t <= (int8_t)thk; t++) {
    if (rightFacing) {
      gfx->drawLine(cx - reach/2, cy - arm + t, cx + reach/2, cy + t,       col);
      gfx->drawLine(cx + reach/2, cy + t,        cx - reach/2, cy + arm + t, col);
    } else {
      gfx->drawLine(cx + reach/2, cy - arm + t, cx - reach/2, cy + t,       col);
      gfx->drawLine(cx - reach/2, cy + t,        cx + reach/2, cy + arm + t, col);
    }
  }
}

void drawSquishEyes(bool closed) {
  gfx->fillScreen(animBgColor);
  const int16_t lx = eyeLX(0), rx = eyeRX(0), cy = eyeCY();
  const int16_t arm   = EYE_H / 2;
  const int16_t reach = EYE_W / 2;
  const int16_t lcx   = lx + EYE_W / 2;
  const int16_t rcx   = rx + EYE_W / 2;
  if (!closed) {
    drawChevron(lcx, cy, arm, reach, 10, true,  C_BLACK);
    drawChevron(rcx, cy, arm, reach, 10, false, C_BLACK);
  } else {
    gfx->fillRect(lx, cy - 5, EYE_W, 10, C_BLACK);
    gfx->fillRect(rx, cy - 5, EYE_W, 10, C_BLACK);
  }
}

// ═════════════════════════════════════════════════════════════
//  PARTIAL REDRAW HELPERS (flicker-free idle animations)
// ═════════════════════════════════════════════════════════════

// Bounding box that covers all possible eye positions
// eyeLX ranges from 10 (ox=-20) to 50 (ox=+20)
// eyeRX+EYE_W ranges from 190 (ox=-20) to 230 (ox=+20)
// eyeY = 50, max height 75 → centre=80, top=43, bottom=118
#define EYE_BBOX_X  8
#define EYE_BBOX_Y  40
#define EYE_BBOX_W  224
#define EYE_BBOX_H  82

// Clear only the eye region, then draw normal eyes
static void drawNormalEyesPartial(int16_t ox, bool blink) {
  gfx->fillRect(EYE_BBOX_X, EYE_BBOX_Y, EYE_BBOX_W, EYE_BBOX_H, animBgColor);
  const int16_t lx = eyeLX(ox), rx = eyeRX(ox), ey = eyeY();
  if (!blink) {
    gfx->fillRect(lx, ey, EYE_W, EYE_H, C_BLACK);
    gfx->fillRect(rx, ey, EYE_W, EYE_H, C_BLACK);
  } else {
    gfx->fillRect(lx, ey + EYE_H / 2 - 3, EYE_W, 6, C_BLACK);
    gfx->fillRect(rx, ey + EYE_H / 2 - 3, EYE_W, 6, C_BLACK);
  }
}

// Clear only the eye region, then draw with custom height
static void drawNormalEyesHPartial(int16_t ox, int16_t h) {
  gfx->fillRect(EYE_BBOX_X, EYE_BBOX_Y, EYE_BBOX_W, EYE_BBOX_H, animBgColor);
  const int16_t lx = eyeLX(ox), rx = eyeRX(ox);
  const int16_t cy = eyeY() + EYE_H / 2;
  const int16_t ey = cy - h / 2;
  gfx->fillRect(lx, ey, EYE_W, h, C_BLACK);
  gfx->fillRect(rx, ey, EYE_W, h, C_BLACK);
}

// Clear only the squish eye region, then draw
static void drawSquishEyesPartial(bool closed) {
  gfx->fillRect(EYE_BBOX_X, EYE_BBOX_Y, EYE_BBOX_W, EYE_BBOX_H, animBgColor);
  const int16_t lx = eyeLX(0), rx = eyeRX(0), cy = eyeCY();
  const int16_t arm   = EYE_H / 2;
  const int16_t reach = EYE_W / 2;
  const int16_t lcx   = lx + EYE_W / 2;
  const int16_t rcx   = rx + EYE_W / 2;
  if (!closed) {
    drawChevron(lcx, cy, arm, reach, 10, true,  C_BLACK);
    drawChevron(rcx, cy, arm, reach, 10, false, C_BLACK);
  } else {
    gfx->fillRect(lx, cy - 5, EYE_W, 10, C_BLACK);
    gfx->fillRect(rx, cy - 5, EYE_W, 10, C_BLACK);
  }
}

// ═════════════════════════════════════════════════════════════
//  IDLE ANIMATIONS — Normal Eyes
// ═════════════════════════════════════════════════════════════

static unsigned long nextIdleMs = 0;
static const uint16_t IDLE_THRESHOLD_MS = 5000;

// Single blink
static void idleBlink() {
  drawNormalEyesPartial(0, true);  delay(120);
  drawNormalEyesPartial(0, false);
}

// Double blink
static void idleDoubleBlink() {
  drawNormalEyesPartial(0, true);  delay(80);
  drawNormalEyesPartial(0, false); delay(60);
  drawNormalEyesPartial(0, true);  delay(80);
  drawNormalEyesPartial(0, false);
}

// Look left then back
static void idleLookLeft() {
  drawNormalEyesPartial(-20, false); delay(800);
  drawNormalEyesPartial(0, false);
}

// Look right then back
static void idleLookRight() {
  drawNormalEyesPartial(20, false); delay(800);
  drawNormalEyesPartial(0, false);
}

// Eyes droop closed then snap open
static void idleSleepy() {
  for (int16_t h = EYE_H; h >= 10; h -= 5) { drawNormalEyesHPartial(0, h); delay(80); }
  delay(1000);
  drawNormalEyesPartial(0, false);
}

// Eyes widen briefly
static void idleSurprised() {
  drawNormalEyesHPartial(0, 75); delay(400);
  drawNormalEyesPartial(0, false);
}

// ═════════════════════════════════════════════════════════════
//  IDLE ANIMATIONS — Squish Eyes
// ═════════════════════════════════════════════════════════════

// Quick close + open
static void idleSquishBlink() {
  drawSquishEyesPartial(true);  delay(150);
  drawSquishEyesPartial(false);
}

// Rapid flutter
static void idleSquishFlutter() {
  for (uint8_t i = 0; i < 3; i++) {
    drawSquishEyesPartial(true);  delay(60);
    drawSquishEyesPartial(false); delay(60);
  }
}

// Hold closed (sleeping)
static void idleSquishSleep() {
  drawSquishEyesPartial(true);  delay(2000);
  drawSquishEyesPartial(false);
}

// ═════════════════════════════════════════════════════════════
//  IDLE LOOP
// ═════════════════════════════════════════════════════════════

void loopIdle() {
  // Only run for eye views, and not when busy
  if (busy) return;
  if (currentView != VIEW_EYES_NORMAL && currentView != VIEW_EYES_SQUISH) return;

  // Wait for idle threshold
  if (millis() - lastInteractionMs < IDLE_THRESHOLD_MS) return;

  // Wait for next scheduled idle animation
  if (millis() < nextIdleMs) return;

  busy = true;

  if (currentView == VIEW_EYES_NORMAL) {
    // Weighted random: blink 45%, look 30%, sleepy 15%, surprised 10%
    uint8_t r = random(100);
    if      (r < 25) idleBlink();
    else if (r < 45) idleDoubleBlink();
    else if (r < 60) idleLookLeft();
    else if (r < 75) idleLookRight();
    else if (r < 90) idleSleepy();
    else             idleSurprised();
  } else {
    // Squish eyes: blink 50%, flutter 30%, sleep 20%
    uint8_t r = random(100);
    if      (r < 50) idleSquishBlink();
    else if (r < 80) idleSquishFlutter();
    else             idleSquishSleep();
  }

  busy = false;

  // Random cooldown before next idle: 3–15 seconds
  nextIdleMs = millis() + 3000 + random(12000);
}
