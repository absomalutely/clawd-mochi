# CLAWD MOCHI — Hardware Reference

> Extracted from `clawd_mochi.ino` (commit `37e3e5c`).
> This file is the single source of truth for pin assignments and display
> configuration when building new sketches.

---

## Board

| Detail   | Value                |
|----------|----------------------|
| MCU      | ESP32-C3 Super Mini  |
| Display  | ST7789 1.54" IPS     |
| Resolution | 240 x 240 px       |

---

## Pin Map

| Function         | GPIO | Notes                      |
|------------------|------|----------------------------|
| SPI MOSI (SDA)   | 5    | Hardware SPI data           |
| SPI SCK  (SCL)   | 4    | Hardware SPI clock          |
| DC (Data/Command) | 1   | Display data/command select |
| RST (Reset)      | 0    | Display reset               |
| BLK (Backlight)  | 10   | PWM-controlled backlight    |

**Power:** VCC → 3V3, GND → GND

**Unused SPI lines:** CS = -1 (not connected), MISO = -1 (not connected)

---

## Display Initialisation (Arduino_GFX)

```cpp
#include <Arduino_GFX_Library.h>

// Bus
Arduino_DataBus *bus = new Arduino_HWSPI(
  1,   // DC   — GPIO 1
  -1,  // CS   — not used
  4,   // SCLK — GPIO 4
  5,   // MOSI — GPIO 5
  -1   // MISO — not used
);

// Display
Arduino_GFX *gfx = new Arduino_ST7789(
  bus,
  0,     // RST  — GPIO 0
  0,     // rotation (0 = portrait)
  true,  // IPS panel
  240,   // width
  240    // height
);
```

---

## PWM Backlight

```cpp
#define TFT_BLK  10
#define PWM_FREQ 5000   // Hz
#define PWM_RES  8      // 8-bit (0–255)
```

Setup in `setup()`:

```cpp
ledcAttach(TFT_BLK, PWM_FREQ, PWM_RES);
ledcWrite(TFT_BLK, 255);   // full brightness
```

Toggle helper:

```cpp
void setBacklight(bool on) {
  ledcWrite(TFT_BLK, on ? 255 : 0);
}
```

---

## WiFi (Access Point)

| Setting   | Value              |
|-----------|--------------------|
| SSID      | `ClaWD-Mochi`     |
| Password  | `clawd1234`        |
| IP        | `192.168.4.1`      |
| Port      | 80 (HTTP)          |

---

## Colour Palette

| Name     | RGB              | Notes                        |
|----------|------------------|------------------------------|
| C_ORANGE | (218, 17, 0)     | Primary brand / accent       |
| C_DARKBG | (10, 12, 16)     | Dark background              |
| C_MUTED  | (90, 88, 86)     | Muted text                   |
| C_GREEN  | (80, 220, 130)   | Terminal prompt / cursor      |
| C_WHITE  | 0xFFFF           | —                            |
| C_BLACK  | 0x0000           | —                            |

Colours are initialised after `gfx->begin()` via `gfx->color565(r, g, b)`.

---

## Eye Geometry

```cpp
#define EYE_W   30    // eye width  (px)
#define EYE_H   60    // eye height (px)
#define EYE_GAP 120   // gap between eyes (px)
#define EYE_OX  0     // horizontal offset
#define EYE_OY  40    // vertical offset (eyes sit above centre)
```

---

## Terminal Layout

```cpp
#define TERM_COLS   15   // characters per line
#define TERM_ROWS    8   // visible lines
#define TERM_CHAR_W 12   // character width  (px)
#define TERM_CHAR_H 20   // character height (px)
#define TERM_PAD_X   8   // left padding     (px)
#define TERM_PAD_Y  18   // top padding / header height (px)
#define PREFIX_PX   54   // "clawd:~$ " prefix width (px)
```

---

## Display Views

| ID | Name             | Description                            |
|----|------------------|----------------------------------------|
| 0  | VIEW_EYES_NORMAL | Rectangular eyes — wiggle + blink loop |
| 1  | VIEW_EYES_SQUISH | Chevron eyes — open / close loop       |
| 2  | VIEW_CODE        | "Claude Code" splash + terminal mode   |
| 3  | VIEW_DRAW        | Web-driven pixel canvas                |

---

## Serial

Baud rate: **115200**

---

## Quick-Start Template

Minimal sketch to get the display running with the same hardware:

```cpp
#include <Arduino_GFX_Library.h>

#define TFT_DC  1
#define TFT_RST 0
#define TFT_BLK 10

Arduino_DataBus *bus = new Arduino_HWSPI(TFT_DC, -1, 4, 5, -1);
Arduino_GFX *gfx = new Arduino_ST7789(bus, TFT_RST, 0, true, 240, 240);

void setup() {
  Serial.begin(115200);

  // Backlight
  ledcAttach(TFT_BLK, 5000, 8);
  ledcWrite(TFT_BLK, 255);

  // Display
  gfx->begin();
  gfx->fillScreen(BLACK);
  gfx->setTextColor(WHITE);
  gfx->setTextSize(2);
  gfx->setCursor(10, 10);
  gfx->print("Hello Clawd!");
}

void loop() {}
```
