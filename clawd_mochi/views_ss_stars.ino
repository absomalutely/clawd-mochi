// ═════════════════════════════════════════════════════════════
//  SCREENSAVER — Starfield (3D fly-through)
// ═════════════════════════════════════════════════════════════

#define STAR_COUNT  80
#define STAR_SPEED  4
#define STAR_STEP_MS 30
#define STAR_CX    (DISP_W / 2)
#define STAR_CY    (DISP_H / 2)
#define STAR_DEPTH 256

struct Star {
  int16_t x, y, z;     // 3D position
  int16_t px, py;       // previous screen position
  bool drawn;
};

static Star stars[STAR_COUNT];
static unsigned long starLastMs = 0;

static void starReset(int i) {
  stars[i].x = random(-STAR_CX, STAR_CX);
  stars[i].y = random(-STAR_CY, STAR_CY);
  stars[i].z = STAR_DEPTH;
  stars[i].drawn = false;
}

void starsStart() {
  currentView = VIEW_SS_STARS;
  gfx->fillScreen(C_BLACK);
  for (int i = 0; i < STAR_COUNT; i++) {
    starReset(i);
    stars[i].z = random(1, STAR_DEPTH);
  }
  lastInteractionMs = millis();
}

void loopStars() {
  if (currentView != VIEW_SS_STARS) return;
  if (millis() - starLastMs < STAR_STEP_MS) return;
  starLastMs = millis();

  for (int i = 0; i < STAR_COUNT; i++) {
    // Erase previous position
    if (stars[i].drawn) {
      gfx->fillRect(stars[i].px, stars[i].py, 3, 3, C_BLACK);
    }

    // Move star closer
    stars[i].z -= STAR_SPEED;
    if (stars[i].z <= 0) {
      starReset(i);
      continue;
    }

    // Project 3D → 2D
    int16_t sx = STAR_CX + (stars[i].x * 128) / stars[i].z;
    int16_t sy = STAR_CY + (stars[i].y * 128) / stars[i].z;

    // Off screen? Reset
    if (sx < 0 || sx >= DISP_W || sy < 0 || sy >= DISP_H) {
      starReset(i);
      continue;
    }

    // Brightness based on depth (closer = brighter)
    uint8_t bri = 255 - (stars[i].z * 200 / STAR_DEPTH);
    uint16_t col = gfx->color565(bri, bri, bri);

    // Size based on depth (closer = bigger)
    uint8_t sz = (stars[i].z < 64) ? 3 : (stars[i].z < 128) ? 2 : 1;

    gfx->fillRect(sx, sy, sz, sz, col);

    // Draw streak line for close stars
    if (stars[i].drawn && stars[i].z < 100) {
      gfx->drawLine(stars[i].px, stars[i].py, sx, sy, gfx->color565(bri / 2, bri / 2, bri / 2));
    }

    stars[i].px = sx;
    stars[i].py = sy;
    stars[i].drawn = true;
  }
}
