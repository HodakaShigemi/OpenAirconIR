# OpenAirconIR

OpenAirconIR is an open-source ESP32 firmware project for receiving, logging, re-sending, and analyzing air conditioner IR remote signals. The long-term goal is a Matter-capable air conditioner IR bridge kit, but the current firmware is intentionally small: it can be tested from the PlatformIO serial monitor without Wi-Fi, Matter, MQTT, or a Web UI.

## What v0.2 Firmware Can Do

- Receive IR remote signals on an ESP32.
- Print received protocol, bit count, value, and raw timing data to serial logs.
- Keep the last received raw IR signal in memory.
- Re-send the last received raw IR signal with the `send-last` serial command.
- Record labeled IR captures in capture mode.
- Export capture records as JSON Lines or a JSON array for later human or AI-assisted analysis.
- Keep an estimated air conditioner state model for future protocol parsing.
- Provide a directory layout that can later grow Matter, Web UI, and capture-analysis features.

## Hardware

- ESP32 development board, such as `esp32dev`.
- IR receiver module, 38 kHz type recommended.
- IR LED.
- NPN transistor or MOSFET driver for the IR LED.
- Current limiting resistors.
- Breadboard and jumper wires.

## Default Wiring

| Function | ESP32 GPIO | Notes |
| --- | ---: | --- |
| IR receiver output | GPIO 15 | Change `OpenAirconIRConfig::IrReceivePin` in `include/config.h` if needed. |
| IR LED driver input | GPIO 4 | Drive an IR LED through a transistor or MOSFET, not directly from the GPIO. |
| Status LED | GPIO 2 | Many ESP32 dev boards have an onboard LED here. |
| 3.3 V | 3V3 | Power the IR receiver module. |
| GND | GND | Common ground for ESP32, receiver, and sender circuit. |

See `docs/wiring.md` for more detail.

## Build And Upload

```sh
pio run
pio run --target upload
pio device monitor
```

The serial monitor speed is `115200`.

## Serial Commands

Type a command followed by Enter in the PlatformIO serial monitor.

| Command | Description |
| --- | --- |
| `help` | Show command list. |
| `state` | Print the estimated air conditioner state. |
| `send-last` | Re-send the last captured raw IR signal. |
| `has-last` | Print whether a captured signal is stored. |
| `clear-last` | Clear the stored signal. |
| `capture-label <label>` | Set the label for following capture records. |
| `capture-start` | Start capture mode. |
| `capture-stop` | Stop capture mode. |
| `capture-list` | List capture records. |
| `capture-show-last` | Show the latest capture record. |
| `capture-export-jsonl` | Export records as JSON Lines. |
| `capture-export-json` | Export records as a JSON document. |
| `capture-clear confirm` | Clear all capture records. |
| `capture-status` | Show capture mode state and limits. |
| `version` | Print firmware version. |

Received IR logs are JSON-like so they can later be pasted into analysis tools:

```json
{
  "event": "ir_received",
  "protocol": "DAIKIN",
  "bits": 280,
  "value": "0x...",
  "raw_count": 123,
  "raw_us": [3500, 1750, 450, 1300]
}
```

## Capture Workflow

Capture mode lets you attach operation labels such as `power_on`, `cool_26_auto_on`, or `fan_high` to raw IR signals. A typical session:

```text
capture-label cool_26_auto_on
capture-start
```

Press the matching button on the air conditioner remote, then change labels as needed:

```text
capture-label cool_27_auto_on
capture-label heat_24_auto_on
capture-stop
capture-list
capture-export-json
```

The JSON export is intended to be pasted into analysis tools or an AI coding agent. The first v0.2 goal is not to fully decode every air conditioner protocol on-device; it is to preserve enough labeled evidence that humans and AI can compare captures, infer mode/temperature/fan bits, and propose protocol support for OpenAirconIR. See `docs/capture_workflow.md` and `docs/ai_ir_analysis_prompt.md`.

## Current Limits

- No Matter support yet.
- No Web UI, Wi-Fi setup, MQTT, or cloud integration.
- No manufacturer-specific air conditioner command generation yet.
- State is only a future-ready data model. IR signals are logged and stored, but not fully decoded into air conditioner state.
- Raw signal replay depends on receiver quality, IR LED drive strength, carrier frequency, and the target air conditioner protocol.

## Roadmap

- v0.1: IR receive/send proof of concept.
- v0.2: Labeled capture mode and JSON export.
- v0.3: Stronger unknown remote analysis support, protocol definition YAML, and code generation tooling.
- v0.4: Web UI.
- v0.5: PCB and enclosure.
- v0.6: Matter experiment.
- v1.0: Public OSS kit release.

More detail is in `docs/roadmap.md`.
