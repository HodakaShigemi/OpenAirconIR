#include <Arduino.h>

#include "config.h"
#include "capture/CaptureManager.h"
#include "ir/IrReceiverService.h"
#include "ir/IrSenderService.h"
#include "state/AirconStateManager.h"

using namespace OpenAirconIRConfig;

IrReceiverService irReceiver;
IrSenderService irSender;
AirconStateManager airconStateManager;
CaptureManager captureManager;

String serialLine;
uint32_t lastBlinkMs = 0;
bool ledState = false;

void printVersion(Stream &stream) {
  stream.print(ProjectName);
  stream.print(F(" firmware "));
  stream.println(FirmwareVersion);
  stream.println(F("Features: IR receive, IR send-last, serial commands, capture mode, JSON export"));
}

void printHelp(Stream &stream) {
  stream.println(F("Available serial commands:"));
  stream.println(F("  help         - Show this command list"));
  stream.println(F("  state        - Print estimated aircon state"));
  stream.println(F("  send-last    - Re-send the last captured raw IR signal"));
  stream.println(F("  has-last     - Print whether a captured IR signal is stored"));
  stream.println(F("  clear-last   - Clear the captured IR signal"));
  stream.println(F("  capture-mode - Print capture mode status"));
  stream.println(F("  capture-start - Start capture mode"));
  stream.println(F("  capture-stop - Stop capture mode"));
  stream.println(F("  capture-label <label> - Set label for captured IR records"));
  stream.println(F("  capture-list - List captured records"));
  stream.println(F("  capture-show-last - Show latest capture record"));
  stream.println(F("  capture-export-jsonl - Export captures as JSON Lines"));
  stream.println(F("  capture-export-json - Export captures as JSON"));
  stream.println(F("  capture-clear confirm - Clear all capture records"));
  stream.println(F("  capture-status - Print capture mode status"));
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
  stream.print(F(", \"max_capture_records\": "));
  stream.print(MaxCaptureRecords);
  stream.print(F(", \"max_raw_capture_length\": "));
  stream.print(MaxRawCaptureLength);
  stream.println();
  stream.println(F("}"));
}

void printCaptureModeInfo(Stream &stream) {
  captureManager.printStatus(stream);
}

void handleSerialCommand(const String &command) {
  if (command.length() == 0) {
    return;
  }

  const int firstSpace = command.indexOf(' ');
  const String commandName = firstSpace >= 0 ? command.substring(0, firstSpace) : command;
  String argument = firstSpace >= 0 ? command.substring(firstSpace + 1) : String("");
  argument.trim();

  if (commandName == F("help")) {
    printHelp(Serial);
  } else if (commandName == F("state")) {
    airconStateManager.printState(Serial);
  } else if (commandName == F("send-last")) {
    if (irReceiver.hasLastSignal()) {
      irSender.sendRawSignal(irReceiver.lastSignal(), Serial);
    } else {
      Serial.println(F("{\"event\":\"ir_send\",\"ok\":false,\"reason\":\"no_last_signal\"}"));
    }
  } else if (commandName == F("has-last")) {
    irReceiver.printLastSignalSummary(Serial);
  } else if (commandName == F("clear-last")) {
    irReceiver.clearLastSignal();
    Serial.println(F("{\"event\":\"ir_last_cleared\",\"ok\":true}"));
  } else if (commandName == F("capture-mode")) {
    printCaptureModeInfo(Serial);
  } else if (commandName == F("capture-start")) {
    captureManager.start(Serial);
  } else if (commandName == F("capture-stop")) {
    captureManager.stop(Serial);
  } else if (commandName == F("capture-label")) {
    captureManager.setLabel(argument, Serial);
  } else if (commandName == F("capture-list")) {
    captureManager.printList(Serial);
  } else if (commandName == F("capture-show-last")) {
    captureManager.printLast(Serial);
  } else if (commandName == F("capture-export-jsonl")) {
    captureManager.exportJsonLines(Serial);
  } else if (commandName == F("capture-export-json")) {
    captureManager.exportJson(Serial);
  } else if (commandName == F("capture-clear")) {
    if (argument == F("confirm")) {
      captureManager.clear(Serial);
    } else {
      Serial.println(F("{\"event\":\"capture_clear\",\"ok\":false,\"reason\":\"confirmation_required\",\"usage\":\"capture-clear confirm\"}"));
    }
  } else if (commandName == F("capture-status")) {
    captureManager.printStatus(Serial);
  } else if (commandName == F("version")) {
    printVersion(Serial);
  } else {
    Serial.print(F("{\"event\":\"serial_command\",\"ok\":false,\"reason\":\"unknown_command\",\"command\":\""));
    Serial.print(commandName);
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
  captureManager.begin();
  irReceiver.begin();
  irSender.begin();
  printHelp(Serial);
}

void loop() {
  if (irReceiver.poll(Serial)) {
    airconStateManager.markUpdatedFromIr();
    captureManager.recordSignal(irReceiver.lastSignal(), millis(), Serial);
  }

  pollSerial();
  updateStatusLed();
}
