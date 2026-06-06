# Roadmap

## v0.1: IR Receive And Send PoC

- Receive IR signals on ESP32.
- Log protocol, bit count, value, and raw timings.
- Store the last raw signal in memory.
- Re-send the stored signal from a serial command.
- Keep a future-ready air conditioner state model.

## v0.2: Capture JSON Output

- Emit stricter JSON records.
- Add capture session IDs and repeated sample counts.
- Save logs in a format that can be pasted into analysis tools.

## v0.3: Unknown Remote Analysis Support

- Document a repeatable workflow for recording unknown remotes.
- Add comparison helpers for multiple captures.
- Define handoff steps for coding agents to infer protocol structure.

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
