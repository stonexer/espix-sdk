#include "ViewContainer.h"

ViewContainer::ViewContainer() {
}

View *ViewContainer::getCurrentView() {
  return _currentView;
}

bool ViewContainer::isTransitioning() {
  return _viewTransition.isRunning();
}

void ViewContainer::setCurrentView(View *view, TransitionOptions transitionOptions) {
  if (_currentView) {
    _unmountingView = _currentView;
    _currentView->willUnmount();
  }
  if (transitionOptions.direction != TransitionDirection::NONE) {
    int startValue = 0;
    switch (transitionOptions.direction) {
    case TransitionDirection::LEFT:
    case TransitionDirection::RIGHT:
      startValue = (int)transitionOptions.direction * getWidth();
      _mountView(view, startValue, 0);
      break;
    case TransitionDirection::UP:
    case TransitionDirection::DOWN:
      startValue = (int)transitionOptions.direction / 2 * getHeight();
      _mountView(view, 0, startValue);
      break;
    default:
      break;
    }
    _viewTransition.start(startValue, 0, transitionOptions);
  } else {
    _unmountView();
    _mountView(view);
  }
}

bool ViewContainer::shouldUpdate() {
  if (isDirty()) {
    return true;
  }
  if (_currentView) {
    return isTransitioning() || _currentView->shouldUpdate();
  }
  return false;
}

void ViewContainer::update() {
  if (_currentView) {
    _updateTransition();
    _currentView->tryUpdate();
  }
}

void ViewContainer::render(CanvasContext *context) {
  if (_unmountingView) {
    _unmountingView->redraw();
  }
  if (_currentView) {
    _currentView->redraw();
  }
}

void ViewContainer::handleKeyPress(KeyCode keyCode) {
  if (_currentView) {
    _currentView->handleKeyPress(keyCode);
  }
}

void ViewContainer::handleScroll(int delta) {
  if (_currentView) {
    _currentView->handleScroll(delta);
  }
}

void ViewContainer::_mountView(View *view, int offsetX, int offsetY) {
  _currentView = view;
  _currentView->setParentView(this);
  _currentView->willMount();
  _currentView->setBounds(getBounds());
  _currentView->redraw(true);
  _currentView->didMount();
}

void ViewContainer::_unmountView() {
  if (_unmountingView) {
    _unmountingView = NULL;
  }
}

void ViewContainer::_updateTransition() {
  if (_viewTransition.isRunning()) {
    auto direction = _viewTransition.getOptions().direction;
    _viewOffset = _viewTransition.getValue();

    switch (direction) {
    case TransitionDirection::LEFT:
    case TransitionDirection::RIGHT:
      _unmountingViewOffset = _viewOffset - (int)direction * getWidth();
      _unmountingView->moveTo(_unmountingViewOffset, 0);
      _currentView->moveTo(_viewOffset, 0);
      break;
    case TransitionDirection::UP:
    case TransitionDirection::DOWN:
      _unmountingViewOffset = _viewOffset - (int)direction / 2 * getHeight();
      _unmountingView->moveTo(0, _unmountingViewOffset);
      _currentView->moveTo(0, _viewOffset);
      break;
    default:
      break;
    }

    if (_viewTransition.isTimeout()) {
      _unmountView();
      _viewTransition.stop();
    }
  }
}
