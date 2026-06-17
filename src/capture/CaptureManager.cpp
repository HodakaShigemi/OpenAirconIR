#include "capture/CaptureManager.h"

#include "config.h"

using namespace OpenAirconIRConfig;

namespace {

void printSpaces(Stream &stream, uint8_t count) {
  for (uint8_t i = 0; i < count; ++i) {
    stream.print(' ');
  }
}

}  // namespace

void CaptureManager::begin() {
  records_.reserve(MaxCaptureRecords);
}

void CaptureManager::start(Stream &stream) {
  enabled_ = true;
  stream.print(F("{\"event\":\"capture_start\",\"ok\":true,\"label\":\""));
  printJsonEscaped(stream, currentLabel_);
  stream.print(F("\",\"warning\":"));
  if (currentLabel_.length() == 0) {
    stream.print(F("\"capture_label_not_set\""));
  } else {
    stream.print(F("null"));
  }
  stream.println(F("}"));
}

void CaptureManager::stop(Stream &stream) {
  enabled_ = false;
  stream.println(F("{\"event\":\"capture_stop\",\"ok\":true}"));
}

void CaptureManager::setLabel(const String &label, Stream &stream) {
  bool truncated = false;
  currentLabel_ = sanitizeLabel(label, truncated);
  stream.print(F("{\"event\":\"capture_label\",\"ok\":"));
  stream.print(currentLabel_.length() > 0 ? F("true") : F("false"));
  stream.print(F(",\"label\":\""));
  printJsonEscaped(stream, currentLabel_);
  stream.print(F("\""));
  if (truncated) {
    stream.print(F(",\"warning\":\"label_truncated\""));
  }
  if (currentLabel_.length() == 0) {
    stream.print(F(",\"reason\":\"empty_label\""));
  }
  stream.println(F("}"));
}

void CaptureManager::clear(Stream &stream) {
  records_.clear();
  nextId_ = 1;
  stream.println(F("{\"event\":\"capture_clear\",\"ok\":true}"));
}

bool CaptureManager::recordSignal(const CapturedIrSignal &signal, uint32_t capturedAtMillis, Stream &stream) {
  if (!enabled_) {
    return false;
  }

  if (signal.rawTimingsUs.empty()) {
    stream.println(F("{\"event\":\"capture_record\",\"ok\":false,\"reason\":\"empty_raw_signal\"}"));
    return false;
  }

  if (records_.size() >= MaxCaptureRecords) {
    stream.print(F("{\"event\":\"capture_record\",\"ok\":false,\"reason\":\"capture_storage_full\",\"count\":"));
    stream.print(records_.size());
    stream.print(F(",\"max\":"));
    stream.print(MaxCaptureRecords);
    stream.println(F("}"));
    return false;
  }

  IrCaptureRecord record;
  record.id = nextId_++;
  record.label = currentLabel_;
  record.capturedAtMillis = capturedAtMillis;
  record.protocol = signal.protocol;
  record.bits = signal.bits;
  record.value = signal.value;
  record.rawLength = signal.rawTimingsUs.size();
  record.rawTruncated = signal.rawTimingsUs.size() > MaxRawCaptureLength;

  const size_t storedRawLength = min(signal.rawTimingsUs.size(), MaxRawCaptureLength);
  record.rawUs.reserve(storedRawLength);
  for (size_t i = 0; i < storedRawLength; ++i) {
    record.rawUs.push_back(signal.rawTimingsUs[i]);
  }

  records_.push_back(record);

  stream.print(F("{\"event\":\"capture_record\",\"ok\":true,\"id\":"));
  stream.print(record.id);
  stream.print(F(",\"label\":\""));
  printJsonEscaped(stream, record.label);
  stream.print(F("\",\"protocol\":\""));
  printJsonEscaped(stream, record.protocol);
  stream.print(F("\",\"bits\":"));
  stream.print(record.bits);
  stream.print(F(",\"rawLength\":"));
  stream.print(record.rawLength);
  stream.print(F(",\"storedRawLength\":"));
  stream.print(record.rawUs.size());
  if (record.rawTruncated) {
    stream.print(F(",\"warning\":\"raw_truncated\""));
  }
  stream.println(F("}"));
  return true;
}

bool CaptureManager::isEnabled() const {
  return enabled_;
}

const String &CaptureManager::currentLabel() const {
  return currentLabel_;
}

size_t CaptureManager::count() const {
  return records_.size();
}

void CaptureManager::printStatus(Stream &stream) const {
  stream.println(F("{"));
  stream.print(F("  \"event\": \"capture_status\", \"enabled\": "));
  stream.print(enabled_ ? F("true") : F("false"));
  stream.print(F(", \"current_label\": \""));
  printJsonEscaped(stream, currentLabel_);
  stream.print(F("\", \"count\": "));
  stream.print(records_.size());
  stream.print(F(", \"max_count\": "));
  stream.print(MaxCaptureRecords);
  stream.print(F(", \"max_raw_length\": "));
  stream.print(MaxRawCaptureLength);
  stream.println();
  stream.println(F("}"));
}

void CaptureManager::printList(Stream &stream) const {
  if (records_.empty()) {
    stream.println(F("(no capture records)"));
    return;
  }

  for (const IrCaptureRecord &record : records_) {
    stream.print(F("#"));
    stream.print(record.id);
    stream.print(F(" label="));
    stream.print(record.label);
    stream.print(F(" protocol="));
    stream.print(record.protocol);
    stream.print(F(" bits="));
    stream.print(record.bits);
    stream.print(F(" rawLength="));
    stream.print(record.rawLength);
    if (record.rawTruncated) {
      stream.print(F(" storedRawLength="));
      stream.print(record.rawUs.size());
      stream.print(F(" raw_truncated=true"));
    }
    stream.println();
  }
}

void CaptureManager::printLast(Stream &stream) const {
  if (records_.empty()) {
    stream.println(F("{\"event\":\"capture_show_last\",\"ok\":false,\"reason\":\"no_capture_records\"}"));
    return;
  }

  printRecordJson(stream, records_.back(), true, 0);
  stream.println();
}

void CaptureManager::exportJsonLines(Stream &stream) const {
  stream.println(F("--- OPENAIRCONIR_CAPTURE_JSONL_BEGIN ---"));
  for (const IrCaptureRecord &record : records_) {
    printRecordJson(stream, record, false, 0);
    stream.println();
  }
  stream.println(F("--- OPENAIRCONIR_CAPTURE_JSONL_END ---"));
}

void CaptureManager::exportJson(Stream &stream) const {
  stream.println(F("--- OPENAIRCONIR_CAPTURE_JSON_BEGIN ---"));
  stream.println(F("{"));
  stream.println(F("  \"project\": \"OpenAirconIR\","));
  stream.println(F("  \"format_version\": \"0.2\","));
  stream.println(F("  \"captures\": ["));
  for (size_t i = 0; i < records_.size(); ++i) {
    printRecordJson(stream, records_[i], true, 4);
    if (i + 1 < records_.size()) {
      stream.print(F(","));
    }
    stream.println();
  }
  stream.println(F("  ]"));
  stream.println(F("}"));
  stream.println(F("--- OPENAIRCONIR_CAPTURE_JSON_END ---"));
}

void CaptureManager::printJsonEscaped(Stream &stream, const String &value) {
  for (size_t i = 0; i < value.length(); ++i) {
    const char c = value.charAt(i);
    switch (c) {
      case '"':
        stream.print(F("\\\""));
        break;
      case '\\':
        stream.print(F("\\\\"));
        break;
      case '\b':
        stream.print(F("\\b"));
        break;
      case '\f':
        stream.print(F("\\f"));
        break;
      case '\n':
        stream.print(F("\\n"));
        break;
      case '\r':
        stream.print(F("\\r"));
        break;
      case '\t':
        stream.print(F("\\t"));
        break;
      default:
        if (static_cast<uint8_t>(c) < 0x20) {
          stream.print(F("\\u00"));
          if (static_cast<uint8_t>(c) < 0x10) {
            stream.print(F("0"));
          }
          stream.print(static_cast<uint8_t>(c), HEX);
        } else {
          stream.print(c);
        }
        break;
    }
  }
}

void CaptureManager::printJsonStringField(Stream &stream, const __FlashStringHelper *name, const String &value) {
  stream.print(F("\""));
  stream.print(name);
  stream.print(F("\":\""));
  printJsonEscaped(stream, value);
  stream.print(F("\""));
}

String CaptureManager::sanitizeLabel(const String &label, bool &truncated) {
  String clean = label;
  clean.trim();

  const int firstSpace = clean.indexOf(' ');
  if (firstSpace >= 0) {
    clean = clean.substring(0, firstSpace);
  }

  truncated = clean.length() > MaxCaptureLabelLength;
  if (truncated) {
    clean = clean.substring(0, MaxCaptureLabelLength);
  }

  return clean;
}

void CaptureManager::printRecordJson(Stream &stream, const IrCaptureRecord &record, bool pretty, uint8_t indent) const {
  const char *separator = pretty ? ",\n" : ",";

  if (pretty) {
    printSpaces(stream, indent);
  }
  stream.print(F("{"));
  if (pretty) {
    stream.println();
    printSpaces(stream, indent + 2);
  }
  stream.print(F("\"id\":"));
  stream.print(record.id);
  stream.print(separator);

  if (pretty) {
    printSpaces(stream, indent + 2);
  }
  printJsonStringField(stream, F("label"), record.label);
  stream.print(separator);

  if (pretty) {
    printSpaces(stream, indent + 2);
  }
  stream.print(F("\"capturedAtMillis\":"));
  stream.print(record.capturedAtMillis);
  stream.print(separator);

  if (pretty) {
    printSpaces(stream, indent + 2);
  }
  printJsonStringField(stream, F("protocol"), record.protocol);
  stream.print(separator);

  if (pretty) {
    printSpaces(stream, indent + 2);
  }
  stream.print(F("\"bits\":"));
  stream.print(record.bits);
  stream.print(separator);

  if (pretty) {
    printSpaces(stream, indent + 2);
  }
  printJsonStringField(stream, F("value"), record.value);
  stream.print(separator);

  if (pretty) {
    printSpaces(stream, indent + 2);
  }
  stream.print(F("\"rawLength\":"));
  stream.print(record.rawLength);
  stream.print(separator);

  if (pretty) {
    printSpaces(stream, indent + 2);
  }
  stream.print(F("\"rawTruncated\":"));
  stream.print(record.rawTruncated ? F("true") : F("false"));
  stream.print(separator);

  if (pretty) {
    printSpaces(stream, indent + 2);
  }
  printJsonStringField(stream, F("notes"), record.notes);
  stream.print(separator);

  if (pretty) {
    printSpaces(stream, indent + 2);
  }
  printJsonStringField(stream, F("manufacturer"), record.manufacturer);
  stream.print(separator);

  if (pretty) {
    printSpaces(stream, indent + 2);
  }
  printJsonStringField(stream, F("remoteModel"), record.remoteModel);
  stream.print(separator);

  if (pretty) {
    printSpaces(stream, indent + 2);
  }
  printJsonStringField(stream, F("airconModel"), record.airconModel);
  stream.print(separator);

  if (pretty) {
    printSpaces(stream, indent + 2);
  }
  stream.print(F("\"raw_us\":"));
  printRawArray(stream, record, pretty, indent + 2);

  if (pretty) {
    stream.println();
    printSpaces(stream, indent);
  }
  stream.print(F("}"));
}

void CaptureManager::printRawArray(Stream &stream, const IrCaptureRecord &record, bool pretty, uint8_t indent) const {
  stream.print(F("["));
  for (size_t i = 0; i < record.rawUs.size(); ++i) {
    if (i > 0) {
      stream.print(F(","));
      if (pretty) {
        stream.print(F(" "));
      }
    }
    stream.print(record.rawUs[i]);
  }
  stream.print(F("]"));
}
