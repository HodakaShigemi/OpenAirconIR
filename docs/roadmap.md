# Roadmap

## v0.1: IR Receive And Send PoC

- Receive IR signals on ESP32.
- Log protocol, bit count, value, and raw timings.
- Store the last raw signal in memory.
- Re-send the stored signal from a serial command.
- Keep a future-ready air conditioner state model.

## v0.2: Capture JSON Output

- Implemented labeled capture mode controlled from serial commands.
- Store bounded capture records in memory with protocol, bit count, value, raw timing length, raw timings, and future metadata fields.
- Export captures as JSON Lines or a JSON document between clear begin/end markers.
- Add workflow documentation and an AI analysis prompt template.

## v0.3: Unknown Remote Analysis Support

- Strengthen unknown remote analysis helpers.
- Add a protocol definition YAML format for learned remotes.
- Build comparison and code generation tools that can turn capture exports into OpenAirconIR protocol support.
- Define handoff steps for coding agents to infer protocol structure from labeled captures.

## v0.4: Web UI

- Add Wi-Fi setup.
- Add a local UI for capture, replay, and state display.
- Keep serial workflows available for debugging.

## v0.5: PCB And Enclosure

- Publish PCB design data.
- Add a reliable IR LED driver circuit.
- Publish 3D-printable enclosure files.
- Write assembly instructions.

## v0.6: Matter Experiment

- Prototype Matter endpoint mapping for air conditioner control.
- Decide how learned IR commands map to Matter operations.
- Keep this experimental until stability is proven.

## v1.0: Public OSS Kit

- Firmware, PCB, enclosure, assembly guide, setup guide, and remote-analysis guide.
- Stable release process and contribution guide.
- Tested reference hardware build.
