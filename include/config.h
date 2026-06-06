#pragma once

#include <Arduino.h>

namespace OpenAirconIRConfig {

constexpr const char *ProjectName = "OpenAirconIR";
constexpr const char *FirmwareVersion = "0.1.0";

constexpr uint8_t IrReceivePin = 15;
constexpr uint8_t IrSendPin = 4;
constexpr uint8_t StatusLedPin = 2;

constexpr uint32_t SerialBaudRate = 115200;

constexpr uint16_t IrCaptureBufferSize = 1024;
constexpr uint8_t IrTimeoutMs = 50;
constexpr uint16_t IrCarrierFrequencyKhz = 38;
constexpr size_t MaxStoredRawTimings = 1024;
constexpr uint32_t StatusBlinkIntervalMs = 1000;

}  // namespace OpenAirconIRConfig
