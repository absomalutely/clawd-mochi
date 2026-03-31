// ═════════════════════════════════════════════════════════════
//  SCREENSAVER — Plasma (sine-wave colour patterns)
// ═════════════════════════════════════════════════════════════

#define PLASMA_W     60
#define PLASMA_H     60
#define PLASMA_SCALE 4   // 60 * 4 = 240
#define PLASMA_STEP_MS 40

static uint16_t plasmaPal[256];
static uint16_t plasmaTime = 0;
static unsigned long plasmaLastMs = 0;

// Precomputed sine table (0-255 → 0-255 sine wave)
static uint8_t sinTab[256];

void plasmaInitSinTable() {
  for (int i = 0; i < 256; i++) {
    sinTab[i] = (uint8_t)(128.0 + 127.0 * sin(i * 2.0 * PI / 256.0));
  }
}

void plasmaInitPalette() {
  for (int i = 0; i < 256; i++) {
    uint8_t r = (uint8_t)(128.0 + 127.0 * sin(i * 2.0 * PI / 256.0));
    uint8_t g = (uint8_t)(128.0 + 127.0 * sin(i * 2.0 * PI / 256.0 + 2.094));
    uint8_t b = (uint8_t)(128.0 + 127.0 * sin(i * 2.0 * PI / 256.0 + 4.189));
    plasmaPal[i] = gfx->color565(r, g, b);
  }
}

void plasmaStart() {
  currentView = VIEW_SS_PLASMA;
  plasmaInitSinTable();
  plasmaInitPalette();
  plasmaTime = 0;
  gfx->fillScreen(C_BLACK);
  lastInteractionMs = millis();
}

void loopPlasma() {
  if (currentView != VIEW_SS_PLASMA) return;
  if (millis() - plasmaLastMs < PLASMA_STEP_MS) return;
  plasmaLastMs = millis();

  uint16_t t = plasmaTime;

  for (int y = 0; y < PLASMA_H; y++) {
    for (int x = 0; x < PLASMA_W; x++) {
      // Combine several sine waves for organic movement
      uint8_t v = sinTab[(x * 8 + t) & 0xFF];
      v += sinTab[(y * 6 + t * 2) & 0xFF];
      v += sinTab[((x + y) * 4 + t) & 0xFF];
      v += sinTab[((x * x + y * y) / 4 + t * 3) & 0xFF];

      gfx->fillRect(x * PLASMA_SCALE, y * PLASMA_SCALE,
                     PLASMA_SCALE, PLASMA_SCALE,
                     plasmaPal[v]);
    }
  }

  plasmaTime += 3;
}
