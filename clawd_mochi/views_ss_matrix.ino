// ═════════════════════════════════════════════════════════════
//  SCREENSAVER — Matrix Digital Rain
// ═════════════════════════════════════════════════════════════

#define MTX_COLS     20        // characters across (240 / 12)
#define MTX_ROWS     20        // characters down (240 / 12)
#define MTX_CHAR_W   12
#define MTX_CHAR_H   12
#define MTX_STEP_MS  60

static int8_t  mtxHead[MTX_COLS];      // current head row per column
static int8_t  mtxLen[MTX_COLS];       // trail length per column
static int8_t  mtxSpeed[MTX_COLS];     // frames between advances per column
static uint8_t mtxTick[MTX_COLS];      // frame counter per column
static unsigned long mtxLastMs = 0;

static uint16_t mtxGreen, mtxBrightGreen, mtxDimGreen;

void matrixInitColours() {
  mtxGreen       = gfx->color565(0, 180, 70);
  mtxBrightGreen = gfx->color565(180, 255, 180);
  mtxDimGreen    = gfx->color565(0, 80, 30);
}

static char mtxRandChar() {
  // Mix of katakana-ish chars and digits
  uint8_t r = random(62);
  if (r < 10) return '0' + r;
  if (r < 36) return 'A' + (r - 10);
  return '!' + (r - 36);  // symbols
}

void matrixResetCol(uint8_t c) {
  mtxHead[c]  = -(int8_t)random(MTX_ROWS);  // stagger start
  mtxLen[c]   = 4 + random(12);
  mtxSpeed[c] = 1 + random(3);
  mtxTick[c]  = 0;
}

void matrixStart() {
  currentView = VIEW_SS_MATRIX;
  matrixInitColours();
  gfx->fillScreen(C_BLACK);
  for (uint8_t c = 0; c < MTX_COLS; c++) matrixResetCol(c);
  lastInteractionMs = millis();
}

void loopMatrix() {
  if (currentView != VIEW_SS_MATRIX) return;
  if (millis() - mtxLastMs < MTX_STEP_MS) return;
  mtxLastMs = millis();

  gfx->setTextSize(1);

  for (uint8_t c = 0; c < MTX_COLS; c++) {
    mtxTick[c]++;
    if (mtxTick[c] < mtxSpeed[c]) continue;
    mtxTick[c] = 0;

    int16_t px = c * MTX_CHAR_W;

    // Draw head character (bright)
    if (mtxHead[c] >= 0 && mtxHead[c] < MTX_ROWS) {
      int16_t py = mtxHead[c] * MTX_CHAR_H;
      gfx->fillRect(px, py, MTX_CHAR_W, MTX_CHAR_H, C_BLACK);
      gfx->setTextColor(mtxBrightGreen);
      gfx->setCursor(px + 2, py + 2);
      gfx->print(mtxRandChar());
    }

    // Dim the previous head (now part of trail)
    int8_t prev = mtxHead[c] - 1;
    if (prev >= 0 && prev < MTX_ROWS) {
      int16_t py = prev * MTX_CHAR_H;
      gfx->fillRect(px, py, MTX_CHAR_W, MTX_CHAR_H, C_BLACK);
      gfx->setTextColor(mtxGreen);
      gfx->setCursor(px + 2, py + 2);
      gfx->print(mtxRandChar());
    }

    // Erase tail
    int8_t tail = mtxHead[c] - mtxLen[c];
    if (tail >= 0 && tail < MTX_ROWS) {
      int16_t py = tail * MTX_CHAR_H;
      gfx->fillRect(px, py, MTX_CHAR_W, MTX_CHAR_H, C_BLACK);
    }

    // Occasionally flicker a random trail character
    if (random(8) == 0) {
      int8_t flickR = mtxHead[c] - 1 - random(mtxLen[c]);
      if (flickR >= 0 && flickR < MTX_ROWS) {
        int16_t py = flickR * MTX_CHAR_H;
        gfx->fillRect(px, py, MTX_CHAR_W, MTX_CHAR_H, C_BLACK);
        gfx->setTextColor(random(2) ? mtxDimGreen : mtxGreen);
        gfx->setCursor(px + 2, py + 2);
        gfx->print(mtxRandChar());
      }
    }

    mtxHead[c]++;
    if (mtxHead[c] - mtxLen[c] >= MTX_ROWS) {
      matrixResetCol(c);
    }
  }
}
