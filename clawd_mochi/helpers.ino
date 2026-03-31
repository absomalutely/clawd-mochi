// ═════════════════════════════════════════════════════════════
//  HELPERS
// ═════════════════════════════════════════════════════════════

int speedMs(int ms) {
  if (animSpeed == 3) return ms / 2;
  if (animSpeed == 1) return ms * 2;
  return ms;
}

uint16_t hexToRgb565(String hex) {
  hex.replace("#", "");
  if (hex.length() != 6) return C_WHITE;
  long v = strtol(hex.c_str(), nullptr, 16);
  return gfx->color565((v >> 16) & 0xFF, (v >> 8) & 0xFF, v & 0xFF);
}

String rgb565ToHex(uint16_t c) {
  uint8_t r = ((c >> 11) & 0x1F) << 3;
  uint8_t g = ((c >> 5)  & 0x3F) << 2;
  uint8_t b = (c & 0x1F) << 3;
  char buf[8];
  snprintf(buf, sizeof(buf), "#%02x%02x%02x", r, g, b);
  return String(buf);
}

void setBacklight(bool on) {
  backlightOn = on;
  ledcWrite(TFT_BLK, on ? 255 : 0);
}

void initColours() {
  C_ORANGE = gfx->color565(218, 17,  0);
  C_DARKBG = gfx->color565(10,  12,  16);
  C_MUTED  = gfx->color565(90,  88,  86);
  C_GREEN  = gfx->color565(80,  220, 130);
  animBgColor = C_ORANGE;
  drawBgColor = C_ORANGE;
}
