#pragma once

#include <Arduino.h>
#include <vector>

#include "IrCaptureRecord.h"
#include "ir/IrReceiverService.h"

class CaptureManager {
 public:
  void begin();
  void start(Stream &stream);
  void stop(Stream &stream);
  void setLabel(const String &label, Stream &stream);
  void clear(Stream &stream);
  bool recordSignal(const CapturedIrSignal &signal, uint32_t capturedAtMillis, Stream &stream);

  bool isEnabled() const;
  const String &currentLabel() const;
  size_t count() const;

  void printStatus(Stream &stream) const;
  void printList(Stream &stream) const;
  void printLast(Stream &stream) const;
  void exportJsonLines(Stream &stream) const;
  void exportJson(Stream &stream) const;

 private:
  bool enabled_ = false;
  String currentLabel_;
  std::vector<IrCaptureRecord> records_;
  uint32_t nextId_ = 1;

  static void printJsonEscaped(Stream &stream, const String &value);
  static void printJsonStringField(Stream &stream, const __FlashStringHelper *name, const String &value);
  static String sanitizeLabel(const String &label, bool &truncated);
  void printRecordJson(Stream &stream, const IrCaptureRecord &record, bool pretty, uint8_t indent) const;
  void printRawArray(Stream &stream, const IrCaptureRecord &record, bool pretty, uint8_t indent) const;
};
