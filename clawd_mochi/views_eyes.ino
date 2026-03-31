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

// ── Idle animation stub (filled in by feature/idle-animations) ──
void loopIdle() {
  // No-op until idle animations feature is implemented
}
