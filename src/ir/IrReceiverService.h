#pragma once

#include <Arduino.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>
#include <vector>

struct CapturedIrSignal {
  String protocol = "UNKNOWN";
  uint16_t bits = 0;
  String value = "0";
  std::vector<uint16_t> rawTimingsUs;
};

class IrReceiverService {
 public:
  IrReceiverService();

  void begin();
  bool poll(Stream &stream);
  bool hasLastSignal() const;
  const CapturedIrSignal &lastSignal() const;
  void clearLastSignal();
  void printLastSignalSummary(Stream &stream) const;

 private:
  IRrecv receiver_;
  decode_results results_;
  CapturedIrSignal lastSignal_;
  bool hasLastSignal_ = false;

  void storeSignal();
  void printReceivedSignal(Stream &stream) const;
};
