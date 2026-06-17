# Capture Workflow

This workflow records labeled air conditioner remote operations and exports them for later analysis.

## Steps

1. Open the PlatformIO serial monitor.

   ```sh
   pio device monitor
   ```

2. Set the first operation label.

   ```text
   capture-label cool_26_auto_on
   ```

3. Start capture mode.

   ```text
   capture-start
   ```

4. Press the matching operation on the physical remote.

5. Set the next operation label.

   ```text
   capture-label cool_27_auto_on
   ```

6. Press the matching operation on the physical remote.

7. Repeat label changes and remote operations for useful comparisons, such as `power_on`, `power_off`, `heat_24_auto_on`, `fan_low`, and `fan_high`.

8. Stop capture mode.

   ```text
   capture-stop
   ```

9. Review the list.

   ```text
   capture-list
   ```

10. Export the capture JSON.

    ```text
    capture-export-json
    ```

11. Paste the exported JSON into `docs/ai_ir_analysis_prompt.md` and ask an AI or human reviewer to infer protocol structure.

## Tips

- Capture pairs that differ by only one setting are the most useful.
- Keep labels short and consistent, for example `cool_26_auto_on` and `cool_27_auto_on`.
- If a raw signal is longer than the configured maximum, OpenAirconIR stores a truncated copy and reports `raw_truncated=true`.
- Use `capture-clear confirm` before starting a fresh remote analysis session.
