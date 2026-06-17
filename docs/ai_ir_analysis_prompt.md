# AI IR Analysis Prompt

Use this template when asking an AI coding agent to analyze an OpenAirconIR capture export.

````text
You are analyzing infrared captures from an air conditioner remote for the OpenAirconIR ESP32 firmware project.

The data below is exported from OpenAirconIR v0.2 capture mode. Each capture has a user-provided operation label, the protocol name detected by IRremoteESP8266 when available, bit count, decoded value when available, and raw timing data in microseconds.

Please analyze the captures with this process:

1. First inspect the raw timing structure. Identify lead-in marks/spaces, repeated frame structure, bit mark timing, logical 0/1 space timing, gaps, repeats, checksums, and whether the data appears pulse-distance, pulse-width, Manchester, or another encoding.
2. Convert the raw timings into likely bit strings or byte arrays. Explain your assumptions and show enough intermediate data to verify them.
3. Compare labels against each other to infer which bit positions or bytes correspond to power, mode, target temperature, fan speed, swing, and other settings.
4. If IRremoteESP8266 reported a known protocol name, check whether that protocol already has usable decode/send support and suggest how OpenAirconIR should call or wrap it.
5. If the protocol is unknown or only partially decoded, propose a protocol definition YAML shape that captures timings, frame layout, fields, enumerations, checksums, and examples.
6. Finish with an implementation plan for adding this remote to OpenAirconIR, including data structures, tests or sample fixtures, and any risks or captures that are still missing.

Important constraints:

- Do not assume a single capture is enough to identify all fields.
- Prefer comparing minimally different labels such as cool_26_auto_on vs cool_27_auto_on.
- Treat raw_us arrays as the source of truth if decoded value or protocol name looks incomplete.
- Keep the final recommendations practical for an Arduino/ESP32 PlatformIO project.

OpenAirconIR capture JSON:

```json
PASTE capture-export-json OUTPUT HERE
```
````
