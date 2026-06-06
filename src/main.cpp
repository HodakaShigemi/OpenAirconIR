#include <Arduino.h>

#include "config.h"
#include "ir/IrReceiverService.h"
#include "ir/IrSenderService.h"
#include "state/AirconStateManager.h"

using namespace OpenAirconIRConfig;

IrReceiverService irReceiver;
IrSenderService irSender;
AirconStateManager airconStateManager;

String serialLine;
uint32_t lastBlinkMs = 0;
bool ledState = false;

void printVersion(Stream &stream) {
  stream.print(ProjectName);
  stream.print(F(" firmware "));
  stream.println(FirmwareVersion);
}

void printHelp(Stream &stream) {
  stream.println(F("Available serial commands:"));
  stream.println(F("  help         - Show this command list"));
  stream.println(F("  state        - Print estimated aircon state"));
  stream.println(F("  send-last    - Re-send the last captured raw IR signal"));
  stream.println(F("  has-last     - Print whether a captured IR signal is stored"));
  stream.println(F("  clear-last   - Clear the captured IR signal"));
  stream.println(F("  capture-mode - Explain the future capture workflow"));
  stream.println(F("  version      - Print firmware version"));
}

void printBootConfig(Stream &stream) {
  stream.println();
  printVersion(stream);
  stream.println(F("{"));
  stream.print(F("  \"event\": \"boot\", \"ir_receive_pin\": "));
  stream.print(IrReceivePin);
  stream.print(F(", \"ir_send_pin\": "));
  stream.print(IrSendPin);
  stream.print(F(", \"status_led_pin\": "));
  stream.print(StatusLedPin);
  stream.print(F(", \"serial_baud_rate\": "));
  stream.print(SerialBaudRate);
  stream.print(F(", \"ir_capture_buffer_size\": "));
  stream.print(IrCaptureBufferSize);
  stream.print(F(", \"ir_timeout_ms\": "));
  stream.print(IrTimeoutMs);
  stream.println();
  stream.println(F("}"));
}

void printCaptureModeInfo(Stream &stream) {
  stream.println(F("{\"event\":\"capture_mode\",\"status\":\"planned\","
                   "\"message\":\"Capture mode will group repeated samples and export JSON for remote analysis.\"}"));
}

void handleSerialCommand(const String &command) {
  if (command.length() == 0) {
    return;
  }

  if (command == F("help")) {
    printHelp(Serial);
  } else if (command == F("state")) {
    airconStateManager.printState(Serial);
  } else if (command == F("send-last")) {
    if (irReceiver.hasLastSignal()) {
      irSender.sendRawSignal(irReceiver.lastSignal(), Serial);
    } else {
      Serial.println(F("{\"event\":\"ir_send\",\"ok\":false,\"reason\":\"no_last_signal\"}"));
    }
  } else if (command == F("has-last")) {
    irReceiver.printLastSignalSummary(Serial);
  } else if (command == F("clear-last")) {
    irReceiver.clearLastSignal();
    Serial.println(F("{\"event\":\"ir_last_cleared\",\"ok\":true}"));
  } else if (command == F("capture-mode")) {
    printCaptureModeInfo(Serial);
  } else if (command == F("version")) {
    printVersion(Serial);
  } else {
    Serial.print(F("{\"event\":\"serial_command\",\"ok\":false,\"reason\":\"unknown_command\",\"command\":\""));
    Serial.print(command);
    Serial.println(F("\"}"));
    printHelp(Serial);
  }
}

void pollSerial() {
  while (Serial.available() > 0) {
    const char incoming = static_cast<char>(Serial.read());
    if (incoming == '\r') {
      continue;
    }
    if (incoming == '\n') {
      serialLine.trim();
      handleSerialCommand(serialLine);
      serialLine = "";
      continue;
    }
    serialLine += incoming;
  }
}

void updateStatusLed() {
  const uint32_t now = millis();
  if (now - lastBlinkMs < StatusBlinkIntervalMs) {
    return;
  }

  lastBlinkMs = now;
  ledState = !ledState;
  digitalWrite(StatusLedPin, ledState ? HIGH : LOW);
}

void setup() {
  pinMode(StatusLedPin, OUTPUT);
  digitalWrite(StatusLedPin, LOW);

  Serial.begin(SerialBaudRate);
  delay(200);

  printBootConfig(Serial);
  airconStateManager.begin();
  irReceiver.begin();
  irSender.begin();
  printHelp(Serial);
}

void loop() {
  if (irReceiver.poll(Serial)) {
    airconStateManager.markUpdatedFromIr();
  }

  pollSerial();
  updateStatusLed();
}
