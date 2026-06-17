#pragma once

#include <Arduino.h>
#include <vector>

struct IrCaptureRecord {
  uint32_t id = 0;
  String label;
  uint32_t capturedAtMillis = 0;
  String protocol = "UNKNOWN";
  uint16_t bits = 0;
  String value = "0";
  size_t rawLength = 0;
  std::vector<uint16_t> rawUs;
  String notes;
  String manufacturer;
  String remoteModel;
  String airconModel;
  bool rawTruncated = false;
};
