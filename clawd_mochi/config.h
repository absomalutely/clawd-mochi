#pragma once

// ── Pins ──────────────────────────────────────────────────────
#define TFT_DC  1
#define TFT_RST 0
#define TFT_BLK 10

// ── PWM Backlight ─────────────────────────────────────────────
#define PWM_FREQ 5000
#define PWM_RES  8

// ── Display ───────────────────────────────────────────────────
#define DISP_W 240
#define DISP_H 240

// ── Eye constants ─────────────────────────────────────────────
#define EYE_W   30
#define EYE_H   60
#define EYE_GAP 120
#define EYE_OX  0
#define EYE_OY  40

// ── Views ─────────────────────────────────────────────────────
#define VIEW_EYES_NORMAL 0
#define VIEW_EYES_SQUISH 1
#define VIEW_DRAW        3
#define VIEW_TICKER      4
#define VIEW_WEATHER     5
#define VIEW_TOKENS      6
#define VIEW_SS_LIFE     10
#define VIEW_SS_MATRIX   11
#define VIEW_SS_FIRE     12
#define VIEW_SS_STARS    13
#define VIEW_SS_PLASMA   14
#define VIEW_POMODORO    20

// ── Logo ──────────────────────────────────────────────────────
#define LOGO_CX 120
#define LOGO_CY 105
