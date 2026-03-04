#include "ST7789LCDDisplay.h"

#ifndef DISPLAY_ROTATION
#define DISPLAY_ROTATION 3
#endif

#ifndef DISPLAY_SCALE_X
#define DISPLAY_SCALE_X 2.5f // 320 / 128
#endif

#ifndef DISPLAY_SCALE_Y
#define DISPLAY_SCALE_Y 3.75f // 240 / 64
#endif

#ifndef DISPLAY_CLEAR_EVERY_FRAME
#define DISPLAY_CLEAR_EVERY_FRAME 0
#endif

#ifndef DISPLAY_INVERT_COLORS
#define DISPLAY_INVERT_COLORS true
#endif

#define DISPLAY_WIDTH  240
#define DISPLAY_HEIGHT 320

bool ST7789LCDDisplay::i2c_probe(TwoWire &wire, uint8_t addr) {
  return true;
}

bool ST7789LCDDisplay::begin() {
  if (!_isOn) {
    if (_peripher_power) _peripher_power->claim();

    if (PIN_TFT_LEDA_CTL != -1) {
      pinMode(PIN_TFT_LEDA_CTL, OUTPUT);
      digitalWrite(PIN_TFT_LEDA_CTL, HIGH);
    }
    if (PIN_TFT_RST != -1) {
      pinMode(PIN_TFT_RST, OUTPUT);
      digitalWrite(PIN_TFT_RST, LOW);
      delay(10);
      digitalWrite(PIN_TFT_RST, HIGH);
    }

// Im not sure if this is just a t-deck problem or not, if your display is slow try this.
#if defined(LILYGO_TDECK) || defined(HELTEC_LORA_V4_TFT)
    displaySPI.begin(PIN_TFT_SCL, -1, PIN_TFT_SDA, PIN_TFT_CS);
#endif

    display.init(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    display.setRotation(DISPLAY_ROTATION);
    display.setSPISpeed(40e6);
    display.invertDisplay(DISPLAY_INVERT_COLORS);

    int canvas_w = display.getRotation() % 2 == 0 ? DISPLAY_WIDTH : DISPLAY_HEIGHT;
    int canvas_h = display.getRotation() % 2 == 0 ? DISPLAY_HEIGHT : DISPLAY_WIDTH;
    _canvas = new GFXcanvas16(canvas_w, canvas_h);
    _canvas->fillScreen(ST77XX_BLACK);
    _canvas->setTextColor(ST77XX_WHITE);
    _canvas->setTextSize(2 * DISPLAY_SCALE_X);
    _canvas->cp437(true);

    _isOn = true;
  }

  return true;
}

void ST7789LCDDisplay::turnOn() {
  ST7789LCDDisplay::begin();
}

void ST7789LCDDisplay::turnOff() {
  if (_isOn) {
    if (PIN_TFT_LEDA_CTL != -1) {
      digitalWrite(PIN_TFT_LEDA_CTL, HIGH);
    }
    if (PIN_TFT_RST != -1) {
      digitalWrite(PIN_TFT_RST, LOW);
    }
    if (PIN_TFT_LEDA_CTL != -1) {
      digitalWrite(PIN_TFT_LEDA_CTL, LOW);
    }
    _isOn = false;

    if (_peripher_power) _peripher_power->release();
  }
}

void ST7789LCDDisplay::clear() {
  if (_canvas) _canvas->fillScreen(ST77XX_BLACK);
}

void ST7789LCDDisplay::startFrame(Color bkg) {
  (void)bkg;
  if (_canvas) {
    _canvas->fillScreen(ST77XX_BLACK);
    _canvas->setTextColor(ST77XX_WHITE);
    _canvas->setTextSize(1 * DISPLAY_SCALE_X);
    _canvas->cp437(true);
  }
}

void ST7789LCDDisplay::setTextSize(int sz) {
  if (_canvas) _canvas->setTextSize(sz * DISPLAY_SCALE_X);
}

void ST7789LCDDisplay::setColor(Color c) {
  switch (c) {
  case DisplayDriver::DARK:
    _color = ST77XX_BLACK;
    break;
  case DisplayDriver::LIGHT:
    _color = ST77XX_WHITE;
    break;
  case DisplayDriver::GREY:
    _color = 0x7BEF;
    break;
  case DisplayDriver::RED:
    _color = ST77XX_RED;
    break;
  case DisplayDriver::GREEN:
    _color = ST77XX_GREEN;
    break;
  case DisplayDriver::BLUE:
    _color = ST77XX_BLUE;
    break;
  case DisplayDriver::YELLOW:
    _color = ST77XX_YELLOW;
    break;
  case DisplayDriver::ORANGE:
    _color = ST77XX_ORANGE;
    break;
  case DisplayDriver::NEON_CYAN:
    _color = 0xFD40; // Tactical Orange/Amber (~RGB 255,170,0)
    break;
  case DisplayDriver::SLATE_GREY:
    _color = 0x4A49; // Warm Steel Grey (~RGB 75,75,75)
    break;
  case DisplayDriver::DARK_GREEN:
    _color = 0x03E0; // Dark Green (RGB565)
    break;
  case DisplayDriver::CHARCOAL:
    _color = 0x1082; // Deep Tactical Black (~RGB 18,18,18)
    break;
  case DisplayDriver::DARK_GREY:
    _color = 0x3186; // Steel Border Grey (~RGB 50,50,50)
    break;
  default:
    _color = ST77XX_WHITE;
    break;
  }
  if (_canvas) _canvas->setTextColor(_color);
}

void ST7789LCDDisplay::setCursor(int x, int y) {
  if (_canvas) _canvas->setCursor(x * DISPLAY_SCALE_X, y * DISPLAY_SCALE_Y);
}

void ST7789LCDDisplay::print(const char *str) {
  if (_canvas) _canvas->print(str);
}

void ST7789LCDDisplay::fillRect(int x, int y, int w, int h) {
  if (_canvas)
    _canvas->fillRect(x * DISPLAY_SCALE_X, y * DISPLAY_SCALE_Y, w * DISPLAY_SCALE_X, h * DISPLAY_SCALE_Y,
                      _color);
}

void ST7789LCDDisplay::drawRect(int x, int y, int w, int h) {
  if (_canvas)
    _canvas->drawRect(x * DISPLAY_SCALE_X, y * DISPLAY_SCALE_Y, w * DISPLAY_SCALE_X, h * DISPLAY_SCALE_Y,
                      _color);
}

void ST7789LCDDisplay::drawRoundRect(int x, int y, int w, int h, int r) {
  if (_canvas)
    _canvas->drawRoundRect(x * DISPLAY_SCALE_X, y * DISPLAY_SCALE_Y, w * DISPLAY_SCALE_X, h * DISPLAY_SCALE_Y,
                           r * DISPLAY_SCALE_X, _color);
}

void ST7789LCDDisplay::fillRoundRect(int x, int y, int w, int h, int r) {
  if (_canvas)
    _canvas->fillRoundRect(x * DISPLAY_SCALE_X, y * DISPLAY_SCALE_Y, w * DISPLAY_SCALE_X, h * DISPLAY_SCALE_Y,
                           r * DISPLAY_SCALE_X, _color);
}

void ST7789LCDDisplay::drawLine(int x0, int y0, int x1, int y1) {
  if (_canvas)
    _canvas->drawLine(x0 * DISPLAY_SCALE_X, y0 * DISPLAY_SCALE_Y, x1 * DISPLAY_SCALE_X, y1 * DISPLAY_SCALE_Y,
                      _color);
}

void ST7789LCDDisplay::drawFastHLine(int x, int y, int w) {
  if (_canvas) _canvas->drawFastHLine(x * DISPLAY_SCALE_X, y * DISPLAY_SCALE_Y, w * DISPLAY_SCALE_X, _color);
}

void ST7789LCDDisplay::drawFastVLine(int x, int y, int h) {
  if (_canvas) _canvas->drawFastVLine(x * DISPLAY_SCALE_X, y * DISPLAY_SCALE_Y, h * DISPLAY_SCALE_Y, _color);
}

void ST7789LCDDisplay::drawCircle(int x, int y, int r) {
  if (_canvas) _canvas->drawCircle(x * DISPLAY_SCALE_X, y * DISPLAY_SCALE_Y, r * DISPLAY_SCALE_X, _color);
}

void ST7789LCDDisplay::fillCircle(int x, int y, int r) {
  if (_canvas) _canvas->fillCircle(x * DISPLAY_SCALE_X, y * DISPLAY_SCALE_Y, r * DISPLAY_SCALE_X, _color);
}

void ST7789LCDDisplay::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2) {
  if (_canvas)
    _canvas->drawTriangle(x0 * DISPLAY_SCALE_X, y0 * DISPLAY_SCALE_Y, x1 * DISPLAY_SCALE_X,
                          y1 * DISPLAY_SCALE_Y, x2 * DISPLAY_SCALE_X, y2 * DISPLAY_SCALE_Y, _color);
}

void ST7789LCDDisplay::fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2) {
  if (_canvas)
    _canvas->fillTriangle(x0 * DISPLAY_SCALE_X, y0 * DISPLAY_SCALE_Y, x1 * DISPLAY_SCALE_X,
                          y1 * DISPLAY_SCALE_Y, x2 * DISPLAY_SCALE_X, y2 * DISPLAY_SCALE_Y, _color);
}

void ST7789LCDDisplay::drawXbm(int x, int y, const uint8_t *bits, int w, int h) {
  uint8_t byteWidth = (w + 7) / 8;

  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      uint8_t byte = bits[j * byteWidth + i / 8];
      bool pixelOn = byte & (0x80 >> (i & 7));

      if (pixelOn) {
        for (int dy = 0; dy < DISPLAY_SCALE_X; dy++) {
          for (int dx = 0; dx < DISPLAY_SCALE_X; dx++) {
            if (_canvas)
              _canvas->drawPixel(x * DISPLAY_SCALE_X + i * DISPLAY_SCALE_X + dx,
                                 y * DISPLAY_SCALE_Y + j * DISPLAY_SCALE_X + dy, _color);
          }
        }
      }
    }
  }
}

void ST7789LCDDisplay::drawRGBBitmap(int x, int y, const uint16_t *bitmap, int w, int h) {
  if (!_canvas) return;
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      uint16_t color = bitmap[j * w + i];
      // Don't draw 0x0000 (black) as a way to have background transparency
      if (color != 0x0000) {
        for (int dy = 0; dy < DISPLAY_SCALE_X; dy++) {
          for (int dx = 0; dx < DISPLAY_SCALE_X; dx++) {
            _canvas->drawPixel(x * DISPLAY_SCALE_X + i * DISPLAY_SCALE_X + dx,
                               y * DISPLAY_SCALE_Y + j * DISPLAY_SCALE_X + dy, color);
          }
        }
      }
    }
  }
}

void ST7789LCDDisplay::drawRGBBitmapScaled(int x, int y, const uint16_t *bitmap, int w, int h, float scale) {
  if (!_canvas) return;
  int tw = (int)(w * scale);
  int th = (int)(h * scale);
  for (int j = 0; j < th; j++) {
    int sy = j / scale;
    if (sy >= h) break;
    for (int i = 0; i < tw; i++) {
      int sx = i / scale;
      if (sx >= w) break;
      uint16_t color = bitmap[sy * w + sx];
      if (color != 0x0000) {
        // Apply both the requested scale and the driver's internal scaling
        for (int dy = 0; dy < DISPLAY_SCALE_X; dy++) {
          for (int dx = 0; dx < DISPLAY_SCALE_X; dx++) {
            _canvas->drawPixel(x * DISPLAY_SCALE_X + i * DISPLAY_SCALE_X + dx,
                               y * DISPLAY_SCALE_Y + j * DISPLAY_SCALE_X + dy, color);
          }
        }
      }
    }
  }
}

uint16_t ST7789LCDDisplay::getTextWidth(const char *str) {
  int16_t x1, y1;
  uint16_t w, h;
  if (_canvas) {
    _canvas->getTextBounds(str, 0, 0, &x1, &y1, &w, &h);
    return w / DISPLAY_SCALE_X;
  }
  return 0;
}

void ST7789LCDDisplay::endFrame() {
  if (_canvas) {
    display.drawRGBBitmap(0, 0, _canvas->getBuffer(), _canvas->width(), _canvas->height());
  }
}
