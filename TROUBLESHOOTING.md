# Troubleshooting Guide

Common issues and their solutions for the Bike GPS Tracker.

## Table of Contents
- [Hardware Issues](#hardware-issues)
- [GPS Issues](#gps-issues)
- [SMS/Modem Issues](#smsmodem-issues)
- [Movement Detection Issues](#movement-detection-issues)
- [Power Issues](#power-issues)
- [Software Issues](#software-issues)

---

## Hardware Issues

### Device Won't Power On

**Symptoms**: No LED, no response, dead board

**Solutions**:
1. Check USB cable (try a different cable)
2. Try a different USB port or power adapter
3. Check if battery is connected and charged
4. Press the reset button on the board
5. Check for physical damage to the board

### Constant Restart/Boot Loop

**Symptoms**: Device keeps restarting, can't complete setup

**Solutions**:
1. Check power supply voltage (needs stable 5V)
2. Check battery voltage (should be > 3.7V)
3. Disconnect all external components and test
4. Re-upload the firmware
5. Check for short circuits on the board

---

## GPS Issues

### "GPS not fixed" Message

**Symptoms**: Device reports "GPS not fixed" in serial monitor

**Solutions**:
1. **Go outdoors** - GPS needs clear view of sky
2. **Wait 5-10 minutes** - Initial fix can take time (cold start)
3. **Check antenna** - Ensure GPS antenna is properly connected
4. **Antenna placement** - Keep GPS antenna away from metal objects
5. **Try near a window** - If testing indoors
6. **Check antenna type** - Active antennas need power, passive don't

**Verification**:
```
Send command: gps
Expected: Latitude and longitude values (not 0.0, 0.0)
```

### GPS Gives Wrong Location

**Symptoms**: Coordinates are incorrect or jumping around

**Solutions**:
1. Wait for better satellite lock (10+ satellites is good)
2. Move to location with clearer sky view
3. Check if GPS antenna is properly connected
4. Verify antenna is correct type for your board
5. Update firmware if available

### GPS Works Then Stops

**Symptoms**: GPS gets fix but then loses it

**Solutions**:
1. Check antenna connection (may be loose)
2. Check for power issues (brown-out)
3. Verify antenna placement hasn't changed
4. Check for interference from nearby electronics
5. Monitor serial output for error messages

---

## SMS/Modem Issues

### "Failed to initialize modem"

**Symptoms**: Setup fails with modem initialization error

**Solutions**:
1. **Check SIM card** - Ensure it's properly inserted
2. **Power cycle** - Unplug and replug power
3. **Check antennas** - LTE antenna must be connected
4. **Wait longer** - Give it 30 seconds to initialize
5. **Check SIM card orientation** - Follow markings on board

**Debug**:
```cpp
// Add to setup() to see detailed modem responses
SerialAT.println("AT");
delay(1000);
while(SerialAT.available()) {
  Serial.write(SerialAT.read());
}
```

### "Failed to send SMS"

**Symptoms**: Movement detected but no SMS received

**Solutions**:
1. **Check phone number format**:
   - ✅ Correct: `"+15551234567"` (with + and country code)
   - ❌ Wrong: `"5551234567"` (no country code)
   - ❌ Wrong: `"001234567890"` (use + not 00)

2. **Check SIM card**:
   - Has active plan/credit
   - SMS service is enabled
   - Not expired
   - PIN is disabled (or entered in code)

3. **Check network**:
   - SIM has signal (check with `status` command)
   - In service area
   - Not in airplane mode

4. **Check SMS service center**:
   ```cpp
   // Add to setup() to check SMS center
   SerialAT.println("AT+CSCA?");
   delay(1000);
   ```

### SMS Sends But Never Arrives

**Symptoms**: Device says SMS sent, but you don't receive it

**Solutions**:
1. Check phone number is correct (typos!)
2. Try sending to a different number
3. Check your phone's SMS settings
4. Check if phone is blocking unknown numbers
5. SMS may be delayed - wait 5-10 minutes
6. Check SMS center number in SIM settings

### Can't Receive Multiple SMS

**Symptoms**: First SMS arrives, but no more after that

**Solutions**:
1. Check cooldown period (`ALERT_COOLDOWN` in code)
2. Verify SIM card has sufficient credit
3. Check for SMS storage full on SIM
4. Try restarting the device

---

## Movement Detection Issues

### Too Many False Alarms

**Symptoms**: Getting SMS alerts when bike isn't actually moved

**Solutions**:
1. **Increase threshold** in `src/main.cpp`:
   ```cpp
   #define MOVEMENT_THRESHOLD  3.0  // Was 2.0, increase to 3-5
   ```

2. **Ensure stable mounting**:
   - Bike should be on stable ground when arming
   - Not on kickstand in windy conditions
   - Away from vibrations (traffic, washing machines, etc.)

3. **Check accelerometer calibration**:
   ```
   Send command: status
   Check acceleration values - should be stable
   ```

4. **Increase check interval**:
   ```cpp
   #define CHECK_INTERVAL      2000  // Was 1000, slower = fewer checks
   ```

### No Alert When Bike Is Moved

**Symptoms**: Bike moved but no SMS received

**Solutions**:
1. **Check if armed**: Send `status` command
   - Should show "ARMED"
   - LED should be blinking

2. **Decrease threshold** (more sensitive):
   ```cpp
   #define MOVEMENT_THRESHOLD  1.0  // Was 2.0, more sensitive
   ```

3. **Check accelerometer**:
   ```
   Send command: status
   Move device and check again - values should change
   ```

4. **Check cooldown**: May be in cooldown period (5 min default)

5. **Verify SMS system**: Test by moving significantly (like picking up)

### Movement Detection Not Working

**Symptoms**: Status shows no acceleration data

**Solutions**:
1. Check if accelerometer initialized:
   - Look for "Accelerometer initialized" in serial monitor
   - If "Failed to initialize", check I2C connections

2. Check I2C address (some boards use 0x19 instead of 0x18):
   ```cpp
   // In setup(), try:
   if (!lis.begin(0x19)) {  // Was 0x18
   ```

3. Check I2C pins are correct for your board

---

## Power Issues

### Battery Drains Too Fast

**Symptoms**: Battery doesn't last long enough

**Solutions**:
1. **Increase check interval**:
   ```cpp
   #define CHECK_INTERVAL      2000  // Was 1000
   ```

2. **Disable LED** when armed (see CUSTOMIZATION.md)

3. **Use larger battery** (2000mAh or more)

4. **Check for power-hungry components**:
   - GPS checking too frequently
   - Modem staying active unnecessarily

5. **Add sleep mode** (advanced):
   - Implement deep sleep between checks
   - Wake on movement interrupt

### Device Resets When Sending SMS

**Symptoms**: Device restarts when trying to send SMS

**Solutions**:
1. **Power supply issue** - Modem needs current spike for SMS
2. Use better power supply (not computer USB, use wall adapter)
3. Add capacitor to power line (advanced)
4. Check battery capacity (min 1000mAh)
5. Ensure good battery connections

---

## Software Issues

### Serial Monitor Shows Garbage

**Symptoms**: Serial output is unreadable characters

**Solutions**:
1. Check baud rate is set to **115200**
2. Try 9600 baud if 115200 doesn't work
3. Check correct COM port is selected
4. Press reset button on board
5. Close and reopen serial monitor

### Commands Not Working

**Symptoms**: Typing commands has no effect

**Solutions**:
1. Check serial monitor is set to "newline" or "both NL & CR"
2. Type command and press Enter
3. Check baud rate (115200)
4. Verify device is not stuck in boot loop
5. Re-upload firmware

### Upload Fails

**Symptoms**: Can't upload code to board

**Solutions**:
1. Hold BOOT button while uploading (some boards)
2. Try different USB cable (must support data)
3. Install/update USB drivers
4. Select correct board type in PlatformIO
5. Close serial monitor before uploading
6. Try different USB port

### Code Won't Compile

**Symptoms**: Build errors in PlatformIO

**Solutions**:
1. Check you have PlatformIO installed
2. Run `pio lib install` to install dependencies
3. Check for typos if you modified code
4. Restore from backup if you made changes
5. Re-download from GitHub

---

## Diagnostic Commands

Use these serial commands to diagnose issues:

```
status    - Shows armed state, GPS fix, acceleration
gps       - Shows current GPS coordinates
arm       - Arms the tracker (for testing)
disarm    - Disarms the tracker
```

## Getting More Help

### Check Serial Output

The serial monitor provides detailed debug information:
- Modem initialization status
- GPS fix status
- Movement detection values
- SMS sending status

### Enable Debug Mode

Add this to see more details:
```cpp
// At the top of main.cpp
#define DEBUG_MODE
// Then add Serial.println() statements where needed
```

### Report Issues

When reporting issues, include:
1. Serial monitor output (full boot sequence)
2. Hardware setup (board model, antennas, battery)
3. What you've tried from this guide
4. Exact error messages
5. When the problem started

### Community Support

- Open an issue on GitHub
- Include "Troubleshooting: [Your Issue]" in title
- Provide all information listed above

---

## Quick Diagnostic Checklist

Before asking for help, check:

- [ ] Power supply is good (5V, stable)
- [ ] USB cable supports data (not just charging)
- [ ] SIM card is inserted correctly
- [ ] SIM card has credit and active plan
- [ ] Antennas are connected (GPS and LTE)
- [ ] Phone number is in correct format (+countrycode)
- [ ] Device is outdoors or near window (for GPS)
- [ ] Baud rate is 115200
- [ ] Latest code is uploaded
- [ ] Checked serial monitor for errors

---

## Still Having Issues?

If you've tried everything in this guide:

1. **Document the issue**:
   - Take photos of hardware setup
   - Copy serial monitor output
   - Note what you've tried

2. **Open a GitHub issue** with all details

3. **Provide context**:
   - What were you doing when it failed?
   - Did it ever work?
   - What changed before it stopped working?

We're here to help! 🚴‍♂️
