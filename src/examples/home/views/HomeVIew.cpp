#include "HomeView.h"

#include "../../services/ServiceClient.h"
#include "../../weather/assets/meteocons-font.h"

#include "MainMenuView.h"

HomeView::HomeView() : View() {
}

HomeView *HomeView::getInstance() {
  static HomeView instance;
  return &instance;
}

void HomeView::willMount() {
  Application.hideStatusBar();
}

void HomeView::willUnmount() {
  Application.showStatusBar();
  _sideViewIndex = 0;
  _millisSinceLastSideViewIndexChanged = millis();
}

bool HomeView::shouldUpdate() {
  if (isDirty()) {
    return true;
  }
  if (millis() - getLastUpdate() > 1000) {
    return true;
  }
  if (_millisSinceLastSideViewIndexChanged == 0 || millis() - _millisSinceLastSideViewIndexChanged > 8 * 1000) {
    _millisSinceLastSideViewIndexChanged = millis();
    if (_sideViewIndex == 0) {
      _sideViewIndex = 1;
    } else {
      _sideViewIndex = 0;
    }
  }
  return false;
}

void HomeView::update() {
  if (TimeClient.isReady()) {
    _timeString = TimeClient.now().toString("%H:%M");
  }
}

void HomeView::didSelect() {
  Application.pushView(MainMenuView::getInstance());
}

void HomeView::render(CanvasContext *context) {
  _drawDateTime(context);
  _drawWeather(context);
  _drawStocks(context);
  if (ServiceClient.isLoading()) {
    context->fillCircle(getClientWidth() / 2, 2, 2);
  }
}

void HomeView::_drawDateTime(CanvasContext *context) {
  const int PADDING_RIGHT = 2;

  context->setTextAlign(TextAlign::RIGHT);

  // Date
  context->setFontSize(FontSize::NORMAL);
  String date = TimeClient.now().toString("%a %d");
  date.toUpperCase();
  context->drawString(date, getClientWidth() - PADDING_RIGHT, 0);

  // Time
  context->setFontSize(FontSize::H1);
  context->drawString(_timeString, getClientWidth() - PADDING_RIGHT, 20);
}

void HomeView::_drawWeather(CanvasContext *context) {
  WeatherForecast now = ServiceClient.getWeatherNow();
  WeatherForecast forecast = ServiceClient.getWeatherForecast(0);

  if (!forecast.day.equals("")) {
    if (_sideViewIndex == 0) {
      const int PADDING_LEFT = 6;

      context->setTextAlign(TextAlign::CENTER);
      context->setFontSize(FontSize::NORMAL);
      String text = String(forecast.lowTemp) + " / " + forecast.highTemp;
      context->drawString(text, PADDING_LEFT + 20, 0);

      context->setTextAlign(TextAlign::LEFT);
      context->setFont(Meteocons_Plain_42);
      context->drawString(now.dayCondCode, PADDING_LEFT, 11);
    } else {
      context->setTextAlign(TextAlign::CENTER);
      context->setFontSize(FontSize::NORMAL);
      String text = now.dayCond;
      text += "  " + String(forecast.lowTemp) + " / " + String(forecast.highTemp);
      context->drawString(text, getClientWidth() / 2, 52);
    }
  }
}

void HomeView::_drawStocks(CanvasContext *context) {
  Stock stock = ServiceClient.getStock(0);
  if (!stock.symbol.equals("")) {
    if (_sideViewIndex == 1) {
      const uint8_t PADDING_LEFT = 8;
      context->setTextAlign(TextAlign::CENTER);
      context->setFontSize(FontSize::NORMAL);
      context->drawString(String(abs(stock.changePercent)) + "%", PADDING_LEFT + 22, 0);

      context->setFontSize(FontSize::H2);
      context->setTextAlign(TextAlign::RIGHT);
      context->drawString(String((int)stock.price), PADDING_LEFT + 34, 27);
      context->setTextAlign(TextAlign::LEFT);
      context->setFontSize(FontSize::NORMAL);
      context->drawString("." + String((int)(stock.price * 100 - ((int)stock.price * 100))),
                          PADDING_LEFT + 34, 32);

      // Draw Triangle
      if (stock.changePercent > 0) {
        context->drawLine(3, 34, 0, 41);
        context->drawLine(3, 34, 6, 41);
        context->drawLine(0, 41, 6, 41);
        context->drawLine(3, 34, 3, 41);
        context->drawLine(3, 34, 2, 41);
        context->drawLine(3, 34, 4, 41);
      } else if (stock.changePercent < 0) {
        context->drawLine(3, 41, 2, 34);
        context->drawLine(3, 41, 3, 34);
        context->drawLine(3, 41, 4, 34);
        context->drawLine(3, 41, 0, 34);
        context->drawLine(3, 41, 6, 34);
        context->drawLine(0, 34, 6, 34);
      }
    } else {
      context->setTextAlign(TextAlign::CENTER);
      context->setFontSize(FontSize::NORMAL);
      String text = stock.symbol + "  " + stock.price + "  " + stock.changePercent + "%";
      context->drawString(text, getClientWidth() / 2, 52);
    }
  }
}
