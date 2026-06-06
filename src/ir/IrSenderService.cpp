#include "ir/IrSenderService.h"

#include "config.h"

using namespace OpenAirconIRConfig;

IrSenderService::IrSenderService() : sender_(IrSendPin) {}

void IrSenderService::begin() {
  sender_.begin();
}

bool IrSenderService::sendRawSignal(const CapturedIrSignal &signal, Stream &stream) {
  if (signal.rawTimingsUs.empty()) {
    stream.println(F("{\"event\":\"ir_send\",\"ok\":false,\"reason\":\"no_signal\"}"));
    return false;
  }

  sender_.sendRaw(signal.rawTimingsUs.data(), signal.rawTimingsUs.size(), IrCarrierFrequencyKhz);
  stream.print(F("{\"event\":\"ir_send\",\"ok\":true,\"protocol\":\""));
  stream.print(signal.protocol);
  stream.print(F("\",\"raw_count\":"));
  stream.print(signal.rawTimingsUs.size());
  stream.println(F("}"));
  return true;
}
