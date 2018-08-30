#include <Arduino.h>

#include "../../espix-core.h"

#include "../clock/views/ClockView.h"
#include "../weather/views/WeatherTodayView.h"

class RootView : public CarouselContainer {
public:
  RootView() : CarouselContainer() {
    addSubviews({new TimeIndexView(), new WeatherIndexView()});
  }

  void willMount() {
    Application.hideStatusView();
    if (getCurrentView() == NULL) {
      showFirstSubview();
    }
  }

  void willUnmount() {
    Application.showStatusView();
  }
};