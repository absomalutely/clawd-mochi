// ═════════════════════════════════════════════════════════════
//  VIEWS — Canvas / Drawing
// ═════════════════════════════════════════════════════════════

// Draw a thick line to the display
void canvasDrawStroke(const String& data, uint16_t color, uint8_t sz) {
  const uint8_t half = sz / 2;

  struct Pt { int16_t x, y; };
  Pt prev = {-1, -1};
  int start = 0;
  while (start < (int)data.length()) {
    int semi = data.indexOf(';', start);
    if (semi == -1) semi = data.length();
    String entry = data.substring(start, semi);
    const int comma = entry.indexOf(',');
    if (comma > 0) {
      const int16_t x = entry.substring(0, comma).toInt();
      const int16_t y = entry.substring(comma + 1).toInt();
      if (prev.x >= 0) {
        for (int8_t d = -(int8_t)half; d <= (int8_t)half; d++) {
          gfx->drawLine(prev.x, prev.y + d, x, y + d, color);
          gfx->drawLine(prev.x + d, prev.y, x + d, y, color);
        }
      } else {
        gfx->fillCircle(x, y, half, color);
      }
      prev = {x, y};
    }
    start = semi + 1;
  }
}

// Clear the display
void canvasClear(uint16_t bg) {
  gfx->fillScreen(bg);
}
