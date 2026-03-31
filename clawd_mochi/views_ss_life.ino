// ═════════════════════════════════════════════════════════════
//  SCREENSAVER — Conway's Game of Life
// ═════════════════════════════════════════════════════════════

#define LIFE_COLS 60
#define LIFE_ROWS 60
#define LIFE_CELL 4  // pixels per cell (60 * 4 = 240)

static uint8_t lifeGrid[LIFE_ROWS][(LIFE_COLS + 7) / 8];  // bitpacked: ~450 bytes
static uint8_t lifeNext[LIFE_ROWS][(LIFE_COLS + 7) / 8];
static uint16_t lifeGeneration = 0;
static unsigned long lifeLastStepMs = 0;

#define LIFE_STEP_MS 100
#define LIFE_RESTART_GENS 500  // auto-restart after N generations

static inline bool lifeGet(const uint8_t grid[][8], int r, int c) {
  if (r < 0 || r >= LIFE_ROWS || c < 0 || c >= LIFE_COLS) return false;
  return (grid[r][c / 8] >> (c % 8)) & 1;
}

static inline void lifeSet(uint8_t grid[][8], int r, int c, bool v) {
  if (v) grid[r][c / 8] |=  (1 << (c % 8));
  else   grid[r][c / 8] &= ~(1 << (c % 8));
}

void lifeRandomise() {
  for (int r = 0; r < LIFE_ROWS; r++)
    for (int b = 0; b < (LIFE_COLS + 7) / 8; b++)
      lifeGrid[r][b] = random(256);
  lifeGeneration = 0;
  gfx->fillScreen(animBgColor);
}

void lifeStep() {
  memset(lifeNext, 0, sizeof(lifeNext));
  bool changed = false;

  for (int r = 0; r < LIFE_ROWS; r++) {
    for (int c = 0; c < LIFE_COLS; c++) {
      int n = 0;
      for (int dr = -1; dr <= 1; dr++)
        for (int dc = -1; dc <= 1; dc++)
          if (dr || dc)
            n += lifeGet(lifeGrid, r + dr, c + dc) ? 1 : 0;

      bool alive = lifeGet(lifeGrid, r, c);
      bool next  = alive ? (n == 2 || n == 3) : (n == 3);

      lifeSet(lifeNext, r, c, next);

      if (next != alive) {
        changed = true;
        int16_t px = c * LIFE_CELL;
        int16_t py = r * LIFE_CELL;
        gfx->fillRect(px, py, LIFE_CELL, LIFE_CELL, next ? drawBgColor : animBgColor);
      }
    }
  }

  memcpy(lifeGrid, lifeNext, sizeof(lifeGrid));
  lifeGeneration++;

  // Restart if stagnant or too many generations
  if (!changed || lifeGeneration >= LIFE_RESTART_GENS) {
    lifeRandomise();
  }
}

void lifeDrawFull() {
  gfx->fillScreen(animBgColor);
  for (int r = 0; r < LIFE_ROWS; r++) {
    for (int c = 0; c < LIFE_COLS; c++) {
      if (lifeGet(lifeGrid, r, c)) {
        gfx->fillRect(c * LIFE_CELL, r * LIFE_CELL, LIFE_CELL, LIFE_CELL, drawBgColor);
      }
    }
  }
}

void lifeStart() {
  currentView = VIEW_SS_LIFE;
  lifeRandomise();
  lifeDrawFull();
  lastInteractionMs = millis();
}

void loopLife() {
  if (currentView != VIEW_SS_LIFE) return;
  if (millis() - lifeLastStepMs < LIFE_STEP_MS) return;
  lifeLastStepMs = millis();
  lifeStep();
}
