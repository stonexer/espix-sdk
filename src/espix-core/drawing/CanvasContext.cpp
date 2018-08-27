
#include "CanvasContext.h"

#include "Canvas.h"

CanvasContext::CanvasContext() {
}

Canvas *CanvasContext::getCanvas() {
  return _canvas;
}

void CanvasContext::setCanvas(Canvas *canvas) {
  _canvas = canvas;
  setColor(_color);
  setFont(_font);
  setTextAlign(_textAlign);
}

int CanvasContext::getWidth() {
  return _width;
}
void CanvasContext::setWidth(int width) {
  _width = width;
}

int CanvasContext::getHeight() {
  return _height;
}
void CanvasContext::setHeight(int height) {
  _height = height;
}

void CanvasContext::setDirty() {
  getCanvas()->setDirty();
}

void CanvasContext::setColor(Color color) {
  _color = color;
  getCanvas()->setColor(color);
}

int CanvasContext::getOffsetX() {
  return _offsetX;
}
void CanvasContext::setOffsetX(int offsetX) {
  _offsetX = offsetX;
}

int CanvasContext::getOffsetY() {
  return _offsetY;
}
void CanvasContext::setOffsetY(int offsetY) {
  _offsetY = offsetY;
}

void CanvasContext::setTextAlign(TextAlign align) {
  _textAlign = align;
  getCanvas()->setTextAlign(_textAlign);
}

void CanvasContext::setFont(const uint8_t *fontData) {
  _font = fontData;
  getCanvas()->setFont(fontData);
}

void CanvasContext::setFontSize(FontSize size) {
  if (size == FontSize::H1) {
    _font = ArialMT_Plain_24;
  } else if (size == FontSize::H2) {
    _font = ArialMT_Plain_16;
  } else {
    _font = ArialMT_Plain_10;
  }
  getCanvas()->setFont(_font);
}

void CanvasContext::setPixel(int x, int y) {
  getCanvas()->setPixel(_x(x), _y(y));
}

void CanvasContext::drawLine(int x0, int y0, int x1, int y1) {
  getCanvas()->drawLine(_x(x0), _y(y0), _x(x1), _y(y1));
}

void CanvasContext::drawRect(int x, int y, int width, int height) {
  getCanvas()->drawRect(_x(x), _y(y), width, height);
}

void CanvasContext::fillRect(int x, int y, int width, int height) {
  getCanvas()->fillRect(_x(x), _y(y), width, height);
}

void CanvasContext::drawCircle(int x, int y, int radius) {
  getCanvas()->drawCircle(_x(x), _y(y), radius);
}

void CanvasContext::drawCircleQuads(int x, int y, int radius, int quads) {
  getCanvas()->drawCircleQuads(_x(x), _y(y), radius, quads);
}

void CanvasContext::fillCircle(int x, int y, int radius) {
  getCanvas()->fillCircle(_x(x), _y(y), radius);
}

void CanvasContext::drawHorizontalLine(int x, int y, int length) {
  if (length == -1) {
    length = getWidth();
  }
  getCanvas()->drawHorizontalLine(_x(x), _y(y), length);
}

void CanvasContext::drawVerticalLine(int x, int y, int length) {
  if (length == -1) {
    length = getHeight();
  }
  getCanvas()->drawVerticalLine(_x(x), _y(y), length);
}

void CanvasContext::drawXBM(const uint8_t *xbm, int width, int height, int x, int y) {
  getCanvas()->drawXBM(xbm, width, height, _x(x), _y(y));
}

void CanvasContext::drawString(String text, int x, int y) {
  if (x == -1) {
    if (_textAlign == TextAlign::LEFT) {
      x = 0;
    } else if (_textAlign == TextAlign::RIGHT) {
      x = getWidth();
      y = 0;
    } else if (_textAlign == TextAlign::CENTER || _textAlign == TextAlign::CENTER_BOTH) {
      x = getWidth() / 2;
    }
  }
  if (y == -1) {
    if (_textAlign == TextAlign::CENTER_BOTH) {
      y = getHeight() / 2;
    } else {
      y = 0;
    }
  }
  getCanvas()->drawString(text, _x(x), _y(y));
}

void CanvasContext::drawMultilineString(String text, int x, int y, int maxLineWidth) {
  if (x == -1) {
    if (_textAlign == TextAlign::LEFT) {
      x = 0;
    } else if (_textAlign == TextAlign::RIGHT) {
      x = getWidth();
      y = 0;
    } else if (_textAlign == TextAlign::CENTER || _textAlign == TextAlign::CENTER_BOTH) {
      x = getWidth() / 2;
    }
  }
  if (y == -1) {
    if (_textAlign == TextAlign::CENTER_BOTH) {
      y = getHeight() / 2;
    } else {
      y = 0;
    }
  }
  if (maxLineWidth == -1) {
    maxLineWidth = getWidth();
  }
  getCanvas()->drawMultilineString(text, _x(x), _y(y), maxLineWidth);
}

int CanvasContext::getStringWidth(String text) {
  return getCanvas()->getStringWidth(text);
}

void CanvasContext::clear() {
  return getCanvas()->clear();
}

int CanvasContext::_x(int value) {
  return _offsetX + value;
}

int CanvasContext::_y(int value) {
  return _offsetY + value;
}