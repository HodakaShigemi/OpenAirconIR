# Wiring

This first prototype is intended for a breadboard ESP32 setup.

## GPIO Map

| Part | Signal | ESP32 GPIO | Notes |
| --- | --- | ---: | --- |
| IR receiver module | OUT | GPIO 15 | Receives demodulated 38 kHz IR signals. |
| IR receiver module | VCC | 3V3 | Use a 3.3 V compatible receiver. |
| IR receiver module | GND | GND | Common ground. |
| IR LED driver | IN | GPIO 4 | Connect to the transistor or MOSFET gate/base path. |
| Status LED | LED | GPIO 2 | Onboard LED on many ESP32 boards. |

## IR Receiver

Use a demodulating IR receiver module such as a 38 kHz receiver. Connect VCC to 3.3 V, GND to GND, and OUT to GPIO 15. If your module has a different pin order, follow the module datasheet rather than the board silkscreen guess.

## IR LED Sender

Do not drive a high-current IR LED directly from an ESP32 GPIO. Use GPIO 4 as a logic signal into a simple transistor or MOSFET driver.

A common NPN transistor circuit:

- ESP32 GPIO 4 to base through a resistor.
- IR LED and current limiting resistor from 3.3 V or 5 V to transistor collector.
- Transistor emitter to GND.
- ESP32 GND and LED power GND connected together.

For longer range, tune the IR LED current within the LED and transistor ratings.

## Pin Changes

All firmware pin constants are in `include/config.h`:

- `OpenAirconIRConfig::IrReceivePin`
- `OpenAirconIRConfig::IrSendPin`
- `OpenAirconIRConfig::StatusLedPin`
