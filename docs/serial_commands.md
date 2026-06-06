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
| `capture-mode` | Prints a placeholder for the future capture workflow. |
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
