#pragma once

#include <Arduino.h>

// Represents a state of animation
struct AnimationState {
  unsigned long lastUpdate = 0;
  unsigned long updateStart = 0;
  int timeBudget = 0;
  int elapsedSinceLastUpdate = 0;
};
