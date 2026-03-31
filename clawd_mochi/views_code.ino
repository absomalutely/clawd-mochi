// ═════════════════════════════════════════════════════════════
//  VIEWS — Claude Code splash
// ═════════════════════════════════════════════════════════════

void drawCodeView() {
  termMode = false;
  gfx->fillScreen(C_DARKBG);
  gfx->fillRect(0, 0,          DISP_W, 4, C_ORANGE);
  gfx->fillRect(0, DISP_H - 4, DISP_W, 4, C_ORANGE);
  gfx->setTextColor(C_ORANGE); gfx->setTextSize(4);
  gfx->setCursor((DISP_W - 144) / 2, DISP_H / 2 - 52); gfx->print("Claude");
  gfx->setTextColor(C_WHITE);  gfx->setTextSize(4);
  gfx->setCursor((DISP_W - 96) / 2,  DISP_H / 2 + 8);  gfx->print("Code");
  gfx->fillRect((DISP_W - 96) / 2, DISP_H / 2 + 52, 96, 3, C_ORANGE);
}
