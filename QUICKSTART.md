# Quick Start Guide

Get your bike tracker up and running in 5 minutes!

## Prerequisites

- [ ] LilyGO A7670E board
- [ ] SIM card with SMS capability
- [ ] GPS antenna (usually included)
- [ ] LTE antenna (usually included)
- [ ] USB-C cable for programming
- [ ] PlatformIO installed ([Install Guide](https://platformio.org/install))

## Step 1: Hardware Setup (2 minutes)

1. **Insert SIM card** into the SIM card slot
2. **Connect GPS antenna** to the GPS connector
3. **Connect LTE antenna** to the main/LTE connector
4. **Connect USB-C cable** to your computer

## Step 2: Configure Phone Number (1 minute)

1. Open `src/main.cpp`
2. Find this line (around line 36):
   ```cpp
   #define ALERT_PHONE_NUMBER  "+1234567890"
   ```
3. Replace with your phone number in international format:
   - US: `"+15551234567"`
   - UK: `"+447123456789"`
   - France: `"+33612345678"`

## Step 3: Upload Code (2 minutes)

```bash
# In the project directory
pio run --target upload
```

Or click the "Upload" button in PlatformIO IDE.

## Step 4: Test (1 minute)

1. Open Serial Monitor (115200 baud)
2. Wait for "Setup Complete"
3. Type `arm` and press Enter
4. Move the device
5. Check your phone for SMS!

## Daily Use

### Securing Your Bike

1. Park your bike
2. Serial command: `arm` (or press button if installed)
3. Wait for LED to blink (means it's armed)
4. Walk away!

### Unlocking Your Bike

1. Return to your bike
2. Serial command: `disarm` (or press button)
3. LED stops blinking
4. Ride away!

## Serial Commands

Open serial monitor at 115200 baud and type:

| Command | Action |
|---------|--------|
| `arm` | Arm the tracker |
| `disarm` | Disarm the tracker |
| `status` | Show current status |
| `gps` | Show GPS location |

## LED Indicators

- **Slow blink (1 Hz)**: Armed and monitoring
- **Fast blink (5 Hz)**: Sending SMS
- **3 flashes on startup**: System ready
- **Solid on then off**: Command received

## Troubleshooting

### "GPS not fixed"
- Go outdoors or near a window
- Wait 2-5 minutes
- GPS antenna must have clear sky view

### "Failed to send SMS"
- Check SIM card is inserted correctly
- Verify SIM has credit
- Check phone number format (needs + and country code)
- Ensure SIM card supports SMS

### No response from device
- Check USB connection
- Try different USB port
- Press reset button on board
- Re-upload code

## What's Next?

- Read [README.md](README.md) for detailed documentation
- Check [WIRING.md](WIRING.md) for hardware setup
- Adjust sensitivity in `src/main.cpp` if needed
- Add a button for easier arm/disarm

## Need Help?

- Check the [README.md](README.md) troubleshooting section
- Review the [WIRING.md](WIRING.md) hardware guide
- Open an issue on GitHub

---

**Ready to ride safely! 🚴‍♂️🔒**
