# Serial Commands

Open the PlatformIO serial monitor at `115200` baud:

```sh
pio device monitor
```

Send one command per line.

## Commands

| Command | Use |
| --- | --- |
| `help` | Prints all supported commands. |
| `state` | Prints the estimated air conditioner state model. |
| `send-last` | Re-sends the last captured raw IR signal. |
| `has-last` | Prints whether a captured signal exists and shows a short summary. |
| `clear-last` | Clears the stored signal from memory. |
| `capture-mode` | Alias-style status helper for capture mode. |
| `capture-start` | Starts capture mode. Warns if no current label is set. |
| `capture-stop` | Stops capture mode. |
| `capture-label <label>` | Sets the label for subsequent captures. Labels use the first whitespace-separated token and are truncated to the configured maximum length. |
| `capture-list` | Prints a compact list of stored capture records. |
| `capture-show-last` | Prints the latest capture record with raw timings. |
| `capture-export-jsonl` | Exports all capture records as JSON Lines between begin/end markers. |
| `capture-export-json` | Exports all capture records as one JSON document between begin/end markers. |
| `capture-clear confirm` | Clears all capture records after confirmation. |
| `capture-status` | Prints whether capture mode is enabled, the current label, record count, maximum record count, and maximum raw length. |
| `version` | Prints the project name and firmware version. |

## Example Logs

Boot:

```json
{
  "event": "boot",
  "ir_receive_pin": 15,
  "ir_send_pin": 4,
  "status_led_pin": 2,
  "serial_baud_rate": 115200
}
```

IR received:

```json
{
  "event": "ir_received",
  "protocol": "UNKNOWN",
  "bits": 32,
  "value": "0x12345678",
  "raw_count": 67,
  "raw_us": [9000, 4500, 560, 560]
}
```

Check last signal:

```json
{"event":"ir_last_status","has_last":true,"protocol":"UNKNOWN","bits":32,"raw_count":67}
```

Re-send last signal:

```json
{"event":"ir_send","ok":true,"protocol":"UNKNOWN","raw_count":67}
```

State:

```json
{
  "event": "aircon_state",
  "power": "unknown",
  "mode": "unknown",
  "target_temperature_celsius": 24,
  "fan_mode": "unknown",
  "swing": "unknown",
  "last_updated_by": "system"
}
```

Start capture mode:

```json
{"event":"capture_start","ok":true,"label":"cool_26_auto_on","warning":null}
```

Capture list:

```text
#1 label=cool_26_auto_on protocol=DAIKIN bits=280 rawLength=583
#2 label=cool_27_auto_on protocol=DAIKIN bits=280 rawLength=583
```

JSON Lines export:

```text
--- OPENAIRCONIR_CAPTURE_JSONL_BEGIN ---
{"id":1,"label":"cool_26_auto_on","capturedAtMillis":12345,"protocol":"DAIKIN","bits":280,"value":"...","rawLength":583,"rawTruncated":true,"notes":"","manufacturer":"","remoteModel":"","airconModel":"","raw_us":[3500,1750,450,1300]}
--- OPENAIRCONIR_CAPTURE_JSONL_END ---
```

JSON export:

```text
--- OPENAIRCONIR_CAPTURE_JSON_BEGIN ---
{
  "project": "OpenAirconIR",
  "format_version": "0.2",
  "captures": [
    {
      "id": 1,
      "label": "cool_26_auto_on",
      "capturedAtMillis": 12345,
      "protocol": "DAIKIN",
      "bits": 280,
      "value": "...",
      "rawLength": 583,
      "rawTruncated": true,
      "notes": "",
      "manufacturer": "",
      "remoteModel": "",
      "airconModel": "",
      "raw_us": [3500, 1750, 450, 1300]
    }
  ]
}
--- OPENAIRCONIR_CAPTURE_JSON_END ---
```
