// ═════════════════════════════════════════════════════════════
//  VIEWS — Scrolling Text Ticker
// ═════════════════════════════════════════════════════════════

static String   tickerText     = "";
static int16_t  tickerOffset   = DISP_W;
static uint8_t  tickerSpeed    = 2;
static uint8_t  tickerTextSize = 3;
static bool     tickerActive   = false;
static uint8_t  tickerPrevView = VIEW_EYES_NORMAL;

// Layout — ticker runs across bottom third, eyes sit above
#define TICKER_BAND_H   50
#define TICKER_BAND_Y   (DISP_H - TICKER_BAND_H - 10)
#define TICKER_ACCENT_H 2

// Eyes looking down — positioned in upper portion
#define TICKER_EYE_W    30
#define TICKER_EYE_H    50
#define TICKER_EYE_GAP  120
#define TICKER_EYE_Y    30
#define TICKER_ARROW_H  12

// Canvas sprite for flicker-free ticker band (~23KB)
static Arduino_Canvas *tickerCanvas = nullptr;

void tickerInitCanvas() {
  if (tickerCanvas) return;
  tickerCanvas = new Arduino_Canvas(DISP_W, TICKER_BAND_H, gfx, 0, TICKER_BAND_Y);
  tickerCanvas->begin();
}

void tickerDrawEyes() {
  // Eye positions centred horizontally
  const int16_t lx = (DISP_W - (TICKER_EYE_W * 2 + TICKER_EYE_GAP)) / 2;
  const int16_t rx = lx + TICKER_EYE_W + TICKER_EYE_GAP;

  // Rectangular eyes
  gfx->fillRect(lx, TICKER_EYE_Y, TICKER_EYE_W, TICKER_EYE_H, C_BLACK);
  gfx->fillRect(rx, TICKER_EYE_Y, TICKER_EYE_W, TICKER_EYE_H, C_BLACK);

  // Down-arrow pointers beneath each eye
  const int16_t ay = TICKER_EYE_Y + TICKER_EYE_H + 2;
  // Left eye arrow
  const int16_t lcx = lx + TICKER_EYE_W / 2;
  for (int16_t row = 0; row < TICKER_ARROW_H; row++) {
    int16_t halfW = TICKER_EYE_W / 2 - (row * TICKER_EYE_W / 2 / TICKER_ARROW_H);
    gfx->drawFastHLine(lcx - halfW, ay + row, halfW * 2, C_BLACK);
  }
  // Right eye arrow
  const int16_t rcx = rx + TICKER_EYE_W / 2;
  for (int16_t row = 0; row < TICKER_ARROW_H; row++) {
    int16_t halfW = TICKER_EYE_W / 2 - (row * TICKER_EYE_W / 2 / TICKER_ARROW_H);
    gfx->drawFastHLine(rcx - halfW, ay + row, halfW * 2, C_BLACK);
  }
}

void tickerDrawFrame() {
  // Render to off-screen canvas — no flicker
  int16_t textY = (TICKER_BAND_H - (tickerTextSize * 8)) / 2;

  tickerCanvas->fillScreen(animBgColor);
  tickerCanvas->setTextWrap(false);
  tickerCanvas->setTextColor(drawBgColor);
  tickerCanvas->setTextSize(tickerTextSize);
  tickerCanvas->setCursor(tickerOffset, textY);
  tickerCanvas->print(tickerText);

  // Push finished frame to display in one shot
  tickerCanvas->flush();
}

void tickerDrawChrome() {
  // Fill background
  gfx->fillScreen(animBgColor);

  // Draw eyes looking down
  tickerDrawEyes();
}

void tickerStart(const String& text, uint8_t speed, uint8_t textSize) {
  tickerInitCanvas();
  tickerText     = text;
  tickerSpeed    = speed;
  tickerTextSize = textSize;
  tickerOffset   = DISP_W;
  tickerActive   = true;
  tickerPrevView = currentView;
  currentView    = VIEW_TICKER;
  tickerDrawChrome();
}

void tickerReturnToEyes() {
  tickerActive = false;
  currentView  = tickerPrevView;
  if (currentView == VIEW_EYES_SQUISH) {
    drawSquishEyes();
  } else {
    currentView = VIEW_EYES_NORMAL;
    drawNormalEyes();
  }
  lastInteractionMs = millis();
}

// Called from main loop()
void loopTicker() {
  if (currentView != VIEW_TICKER || !tickerActive) return;

  tickerDrawFrame();

  // Advance scroll
  int16_t charW = tickerTextSize * 6;
  int16_t textPixelWidth = tickerText.length() * charW;
  tickerOffset -= tickerSpeed;

  // Message has fully scrolled off — return to eyes
  if (tickerOffset < -textPixelWidth) {
    tickerReturnToEyes();
    return;
  }

  delay(16);
}
