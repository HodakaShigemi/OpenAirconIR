#pragma once

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#include "ir/IrReceiverService.h"

class IrSenderService {
 public:
  IrSenderService();

  void begin();
  bool sendRawSignal(const CapturedIrSignal &signal, Stream &stream);

 private:
  IRsend sender_;
};
