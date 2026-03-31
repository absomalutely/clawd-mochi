#pragma once
#include <Arduino.h>
#include <WebServer.h>

// ── Colours (initialised after gfx->begin()) ──────────────────
extern uint16_t C_ORANGE, C_DARKBG, C_MUTED, C_GREEN;
#define C_WHITE 0xFFFF
#define C_BLACK 0x0000

// ── State ─────────────────────────────────────────────────────
extern uint8_t  currentView;
extern bool     busy;
extern bool     backlightOn;
extern uint8_t  animSpeed;
extern uint16_t animBgColor;
extern uint16_t drawBgColor;
extern bool     termMode;
extern unsigned long lastInteractionMs;

// ── Terminal state ────────────────────────────────────────────
extern String  termLines[];
extern uint8_t termRow;
extern uint8_t termCol;

// ── WiFi state ────────────────────────────────────────────────
extern bool   staConnected;
extern String staSSID;
extern String staIP;

// ── Weather state ─────────────────────────────────────────────
extern bool wxValid;

// ── WebServer ─────────────────────────────────────────────────
extern WebServer server;
