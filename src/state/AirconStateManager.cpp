#include "state/AirconStateManager.h"

const char *toString(PowerState state) {
  switch (state) {
    case PowerState::Off:
      return "off";
    case PowerState::On:
      return "on";
    case PowerState::Unknown:
    default:
      return "unknown";
  }
}

const char *toString(AirconMode mode) {
  switch (mode) {
    case AirconMode::Cool:
      return "cool";
    case AirconMode::Heat:
      return "heat";
    case AirconMode::Dry:
      return "dry";
    case AirconMode::Fan:
      return "fan";
    case AirconMode::Auto:
      return "auto";
    case AirconMode::Unknown:
    default:
      return "unknown";
  }
}

const char *toString(FanMode mode) {
  switch (mode) {
    case FanMode::Auto:
      return "auto";
    case FanMode::Low:
      return "low";
    case FanMode::Medium:
      return "medium";
    case FanMode::High:
      return "high";
    case FanMode::Unknown:
    default:
      return "unknown";
  }
}

const char *toString(ToggleState state) {
  switch (state) {
    case ToggleState::Off:
      return "off";
    case ToggleState::On:
      return "on";
    case ToggleState::Unknown:
    default:
      return "unknown";
  }
}

const char *toString(StateUpdateSource source) {
  switch (source) {
    case StateUpdateSource::IrRemote:
      return "ir_remote";
    case StateUpdateSource::SerialCommand:
      return "serial_command";
    case StateUpdateSource::System:
      return "system";
    case StateUpdateSource::Unknown:
    default:
      return "unknown";
  }
}

void AirconStateManager::begin() {
  reset(StateUpdateSource::System);
}

const AirconState &AirconStateManager::currentState() const {
  return state_;
}

void AirconStateManager::reset(StateUpdateSource source) {
  state_ = AirconState{};
  state_.lastUpdatedBy = source;
}

void AirconStateManager::markUpdatedFromIr() {
  state_.lastUpdatedBy = StateUpdateSource::IrRemote;
}

void AirconStateManager::printState(Stream &stream) const {
  stream.println(F("{"));
  stream.print(F("  \"event\": \"aircon_state\", \"power\": \""));
  stream.print(toString(state_.power));
  stream.print(F("\", \"mode\": \""));
  stream.print(toString(state_.mode));
  stream.print(F("\", \"target_temperature_celsius\": "));
  stream.print(state_.targetTemperatureCelsius);
  stream.print(F(", \"fan_mode\": \""));
  stream.print(toString(state_.fanMode));
  stream.print(F("\", \"swing\": \""));
  stream.print(toString(state_.swing));
  stream.print(F("\", \"last_updated_by\": \""));
  stream.print(toString(state_.lastUpdatedBy));
  stream.println(F("\""));
  stream.println(F("}"));
}
