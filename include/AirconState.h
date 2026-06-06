#pragma once

#include <Arduino.h>

enum class PowerState {
  Unknown,
  Off,
  On,
};

enum class AirconMode {
  Unknown,
  Cool,
  Heat,
  Dry,
  Fan,
  Auto,
};

enum class FanMode {
  Unknown,
  Auto,
  Low,
  Medium,
  High,
};

enum class ToggleState {
  Unknown,
  Off,
  On,
};

enum class StateUpdateSource {
  Unknown,
  IrRemote,
  SerialCommand,
  System,
};

struct AirconState {
  PowerState power = PowerState::Unknown;
  AirconMode mode = AirconMode::Unknown;
  int targetTemperatureCelsius = 24;
  FanMode fanMode = FanMode::Unknown;
  ToggleState swing = ToggleState::Unknown;
  StateUpdateSource lastUpdatedBy = StateUpdateSource::Unknown;
};

const char *toString(PowerState state);
const char *toString(AirconMode mode);
const char *toString(FanMode mode);
const char *toString(ToggleState state);
const char *toString(StateUpdateSource source);
