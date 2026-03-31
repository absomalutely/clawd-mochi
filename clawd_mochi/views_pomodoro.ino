// ═════════════════════════════════════════════════════════════
//  VIEWS — Pomodoro Timer
// ═════════════════════════════════════════════════════════════

#define POM_STATE_IDLE  0
#define POM_STATE_WORK  1
#define POM_STATE_BREAK 2

static uint8_t  pomState       = POM_STATE_IDLE;
static uint16_t pomWorkMins    = 25;
static uint16_t pomBreakMins   = 5;
static uint32_t pomDurationMs  = 0;
static uint32_t pomStartMs     = 0;
static uint32_t pomRemainingMs = 0;
static bool     pomFlashing    = false;
static uint8_t  pomFlashCount  = 0;
static unsigned long pomFlashMs = 0;
static uint16_t pomLastSecs = 0xFFFF;

// ── Draw helpers ─────────────────────────────────────────────

void pomDrawTimer() {
  uint16_t secs = pomRemainingMs / 1000;
  uint8_t mm = secs / 60;
  uint8_t ss = secs % 60;

  char buf[6];
  snprintf(buf, sizeof(buf), "%02d:%02d", mm, ss);

  int16_t tw = 150;  // 5 chars * 30px
  int16_t tx = (DISP_W - tw) / 2;
  gfx->fillRect(tx, 60, tw, 40, animBgColor);
  gfx->setTextColor(C_BLACK);
  gfx->setTextSize(5);
  gfx->setCursor(tx, 60);
  gfx->print(buf);
}

void pomDrawProgress() {
  float pct = 1.0;
  if (pomDurationMs > 0) {
    uint32_t elapsed = millis() - pomStartMs;
    pct = 1.0 - (float)elapsed / (float)pomDurationMs;
    if (pct < 0) pct = 0;
  }

  int16_t barY = 130;
  int16_t barH = 14;
  int16_t barX = 20;
  int16_t barW = DISP_W - 40;

  // Background track
  gfx->fillRect(barX, barY, barW, barH, gfx->color565(40, 38, 36));
  // Fill
  int16_t fillW = (int16_t)(barW * pct);
  if (fillW > 0) gfx->fillRect(barX, barY, fillW, barH, C_BLACK);
  // Border
  gfx->drawRect(barX - 1, barY - 1, barW + 2, barH + 2, C_BLACK);
}

void pomDrawLabel() {
  gfx->fillRect(0, 160, DISP_W, 60, animBgColor);

  const char* label;
  if (pomState == POM_STATE_WORK) {
    label = "WORK";
  } else if (pomState == POM_STATE_BREAK) {
    label = "BREAK";
  } else {
    label = "READY";
  }

  gfx->setTextColor(C_BLACK);
  gfx->setTextSize(4);
  int16_t tw = strlen(label) * 24;
  gfx->setCursor((DISP_W - tw) / 2, 170);
  gfx->print(label);
}

void pomDrawFull() {
  pomLastSecs = 0xFFFF;
  gfx->fillScreen(animBgColor);
  pomDrawTimer();
  pomDrawProgress();
  pomDrawLabel();
}

// ── State transitions ────────────────────────────────────────

void pomStartWork() {
  pomState = POM_STATE_WORK;
  pomDurationMs = (uint32_t)pomWorkMins * 60000;
  pomRemainingMs = pomDurationMs;
  pomStartMs = millis();
  pomDrawFull();
}

void pomStartBreak() {
  pomState = POM_STATE_BREAK;
  pomDurationMs = (uint32_t)pomBreakMins * 60000;
  pomRemainingMs = pomDurationMs;
  pomStartMs = millis();
  pomDrawFull();
}

void pomStart() {
  currentView = VIEW_POMODORO;
  pomState = POM_STATE_IDLE;
  pomRemainingMs = (uint32_t)pomWorkMins * 60000;
  pomDurationMs = pomRemainingMs;
  pomDrawFull();
  lastInteractionMs = millis();
}

// ── Web commands ─────────────────────────────────────────────

void pomHandleCmd(const String& action) {
  if (action == "start") {
    pomStartWork();
  } else if (action == "stop") {
    pomState = POM_STATE_IDLE;
    pomRemainingMs = (uint32_t)pomWorkMins * 60000;
    pomDurationMs = pomRemainingMs;
    pomDrawFull();
  } else if (action == "skip") {
    if (pomState == POM_STATE_WORK) pomStartBreak();
    else pomStartWork();
  }
}

// ── Loop ─────────────────────────────────────────────────────

void loopPomodoro() {
  if (currentView != VIEW_POMODORO) return;

  // Handle flash alert
  if (pomFlashing) {
    if (millis() - pomFlashMs > 200) {
      pomFlashMs = millis();
      pomFlashCount++;
      if (pomFlashCount >= 8) {
        pomFlashing = false;
        pomDrawFull();
      } else {
        gfx->fillScreen((pomFlashCount % 2 == 0) ? animBgColor : C_BLACK);
      }
    }
    return;
  }

  if (pomState == POM_STATE_IDLE) return;

  // Update remaining time
  uint32_t elapsed = millis() - pomStartMs;
  if (elapsed >= pomDurationMs) {
    // Phase complete — flash then auto-advance
    if (pomState == POM_STATE_WORK) {
      pomStartBreak();
    } else {
      pomStartWork();
    }
    pomFlashing = true;
    pomFlashCount = 0;
    pomFlashMs = millis();
    return;
  }

  pomRemainingMs = pomDurationMs - elapsed;

  // Only redraw when the displayed second changes
  uint16_t secs = pomRemainingMs / 1000;
  if (secs == pomLastSecs) return;
  pomLastSecs = secs;

  pomDrawTimer();
  pomDrawProgress();
}
