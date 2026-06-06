#pragma once

#include <Arduino.h>
#include "AirconState.h"

class AirconStateManager {
 public:
  void begin();
  const AirconState &currentState() const;
  void reset(StateUpdateSource source = StateUpdateSource::System);
  void markUpdatedFromIr();
  void printState(Stream &stream) const;

 private:
  AirconState state_;
};
