#include "NavigationContainer.h"

NavigationContainer::NavigationContainer() : ViewContainer() {
}

View *NavigationContainer::getRootView() {
  return _rootView;
}

void NavigationContainer::setRootView(View *view, TransitionOptions options) {
  _navigationStack.clear();
  _rootView = view;
  setCurrentView(view, options);
}

bool NavigationContainer::canPop() {
  return !_navigationStack.isEmpty();
}

void NavigationContainer::pushView(View *view, TransitionOptions options) {
  _navigationStack.push(view);
  setCurrentView(view, options);
}

View *NavigationContainer::popView(TransitionOptions options) {
  if (canPop()) {
    return NULL;
  }
  View *view = _navigationStack.pop();
  setCurrentView(view, options);
  return view;
}
