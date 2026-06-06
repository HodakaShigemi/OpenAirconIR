#include "ir/IrReceiverService.h"

#include "config.h"

using namespace OpenAirconIRConfig;

IrReceiverService::IrReceiverService()
    : receiver_(IrReceivePin, IrCaptureBufferSize, IrTimeoutMs, true) {}

void IrReceiverService::begin() {
  receiver_.enableIRIn();
}

bool IrReceiverService::poll(Stream &stream) {
  if (!receiver_.decode(&results_)) {
    return false;
  }

  storeSignal();
  printReceivedSignal(stream);
  receiver_.resume();
  return true;
}

bool IrReceiverService::hasLastSignal() const {
  return hasLastSignal_;
}

const CapturedIrSignal &IrReceiverService::lastSignal() const {
  return lastSignal_;
}

void IrReceiverService::clearLastSignal() {
  lastSignal_ = CapturedIrSignal{};
  hasLastSignal_ = false;
}

void IrReceiverService::printLastSignalSummary(Stream &stream) const {
  stream.print(F("{\"event\":\"ir_last_status\",\"has_last\":"));
  stream.print(hasLastSignal_ ? F("true") : F("false"));
  if (hasLastSignal_) {
    stream.print(F(",\"protocol\":\""));
    stream.print(lastSignal_.protocol);
    stream.print(F("\",\"bits\":"));
    stream.print(lastSignal_.bits);
    stream.print(F(",\"raw_count\":"));
    stream.print(lastSignal_.rawTimingsUs.size());
  }
  stream.println(F("}"));
}

void IrReceiverService::storeSignal() {
  lastSignal_.protocol = typeToString(results_.decode_type, results_.repeat);
  lastSignal_.bits = results_.bits;
  lastSignal_.value = uint64ToString(results_.value, 16);
  lastSignal_.rawTimingsUs.clear();

  const uint16_t correctedLength = getCorrectedRawLength(&results_);
  uint16_t *raw = resultToRawArray(&results_);
  if (raw != nullptr) {
    const uint16_t count = min<uint16_t>(correctedLength, MaxStoredRawTimings);
    lastSignal_.rawTimingsUs.reserve(count);
    for (uint16_t i = 0; i < count; ++i) {
      lastSignal_.rawTimingsUs.push_back(raw[i]);
    }
    delete[] raw;
  }

  hasLastSignal_ = !lastSignal_.rawTimingsUs.empty();
}

void IrReceiverService::printReceivedSignal(Stream &stream) const {
  stream.println(F("{"));
  stream.print(F("  \"event\": \"ir_received\", \"protocol\": \""));
  stream.print(lastSignal_.protocol);
  stream.print(F("\", \"bits\": "));
  stream.print(lastSignal_.bits);
  stream.print(F(", \"value\": \"0x"));
  stream.print(lastSignal_.value);
  stream.print(F("\", \"raw_count\": "));
  stream.print(lastSignal_.rawTimingsUs.size());
  stream.print(F(", \"raw_us\": ["));

  for (size_t i = 0; i < lastSignal_.rawTimingsUs.size(); ++i) {
    if (i > 0) {
      stream.print(F(", "));
    }
    stream.print(lastSignal_.rawTimingsUs[i]);
  }

  stream.println(F("]"));
  stream.println(F("}"));
}
