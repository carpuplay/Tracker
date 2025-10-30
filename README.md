# Bike GPS Tracker with SMS Alerts

A GPS tracker designed for bike security using the LilyGO A7670E modem. The tracker monitors your bike's location and sends SMS alerts when movement is detected after the bike has been secured.

## 📚 Documentation

- **[Quick Start Guide](QUICKSTART.md)** - Get started in 5 minutes
- **[Wiring Guide](WIRING.md)** - Hardware setup and connections
- **[Customization Guide](CUSTOMIZATION.md)** - Customize for your needs
- **[Troubleshooting Guide](TROUBLESHOOTING.md)** - Fix common issues

## Features

- **GPS Tracking**: Real-time location tracking using the A7670E's built-in GPS
- **Movement Detection**: Uses accelerometer to detect when the bike is moved
- **SMS Alerts**: Sends text messages with GPS coordinates when unauthorized movement is detected
- **Secure Mode**: Arm/disarm the tracker via serial commands or physical button
- **Low Power**: Efficient power management for long battery life
- **Alert Cooldown**: Prevents spam by limiting alerts to once every 5 minutes

## Hardware Requirements

- LilyGO A7670E ESP32 development board
- SIM card with SMS capability
- Power source (battery or USB)
- Optional: Physical button for arm/disarm

## Pin Configuration

The tracker uses the following pins on the LilyGO A7670E:

- **Modem Communication**: TX=26, RX=27
- **Modem Control**: PWRKEY=4, DTR=32, RI=33, FLIGHT=25, STATUS=34
- **I2C (Accelerometer)**: SDA=21, SCL=22
- **Status LED**: Pin 12
- **Secure Button**: Pin 35 (optional)

## Installation

### 1. Install PlatformIO

If you haven't already, install [PlatformIO](https://platformio.org/):

- **VS Code**: Install the PlatformIO IDE extension
- **Command Line**: `pip install platformio`

### 2. Clone the Repository

```bash
git clone https://github.com/carpuplay/Tracker.git
cd Tracker
```

### 3. Configure Your Phone Number

Edit `src/main.cpp` and change the alert phone number:

```cpp
#define ALERT_PHONE_NUMBER  "+1234567890"  // Change to your phone number
```

**Important**: Use international format (e.g., +1 for US, +44 for UK, +33 for France)

### 4. Adjust Movement Sensitivity (Optional)

You can adjust the movement detection sensitivity in `src/main.cpp`:

```cpp
#define MOVEMENT_THRESHOLD  2.0  // m/s² - Lower = more sensitive
```

### 5. Build and Upload

```bash
# Build the project
pio run

# Upload to your board
pio run --target upload

# Monitor serial output
pio device monitor
```

Or use the PlatformIO IDE buttons in VS Code.

## Usage

### Initial Setup

1. Insert a SIM card with SMS capability into the A7670E
2. Power on the device
3. Wait for GPS to acquire a fix (LED will blink)
4. Open serial monitor at 115200 baud

### Commands

Control the tracker via serial commands:

- `arm` or `secure` - Arm the tracker (enable movement alerts)
- `disarm` or `unlock` - Disarm the tracker (disable alerts)
- `status` - Show current status (armed/disarmed, GPS fix, location)
- `gps` - Display current GPS coordinates with Google Maps link

### Using the Physical Button

If you've connected a button to pin 35:

- **Press once** - Toggle between armed and disarmed states
- LED will blink continuously when armed
- LED will flash rapidly when SMS is sent

### Normal Operation

1. **Park your bike**
2. **Send `arm` command** or press the secure button
   - The tracker records current GPS location and accelerometer position
   - LED starts blinking slowly
   - Movement detection is active
3. **If the bike is moved**:
   - Tracker detects movement via accelerometer
   - SMS is sent to your configured phone number
   - SMS includes:
     - Alert message
     - Last known (secured) GPS location
     - Current GPS location (if available)
     - Google Maps links for both locations
4. **When you return**:
   - Send `disarm` command or press the button
   - LED stops blinking
   - Movement detection is disabled

## SMS Alert Format

When movement is detected, you'll receive an SMS like this:

```
ALERT: Your bike has been moved!
Last location: https://maps.google.com/?q=48.858844,2.294351
Current: https://maps.google.com/?q=48.859123,2.294567
```

## Troubleshooting

### GPS Not Fixing

- Ensure you're outdoors or near a window
- Wait 2-5 minutes for initial GPS fix
- Check GPS antenna connection

### SMS Not Sending

- Verify SIM card is inserted and has credit
- Check phone number format (must include country code with +)
- Ensure SIM card has SMS capability enabled
- Check modem initialization in serial monitor

### False Movement Alerts

- Increase `MOVEMENT_THRESHOLD` value in `src/main.cpp`
- Ensure bike is on stable ground when arming
- Check that accelerometer is properly calibrated

### Modem Not Initializing

- Check power supply (A7670E needs stable power)
- Verify pin connections
- Try power cycling the device
- Check serial output for error messages

## Power Consumption

- **Armed (monitoring)**: ~150mA average
- **GPS acquisition**: ~200mA peak
- **Sending SMS**: ~300mA peak
- **Deep sleep** (future feature): <5mA

For outdoor deployment, use:
- 18650 battery: ~8-12 hours continuous monitoring
- Power bank (10000mAh): ~2-3 days continuous monitoring

## Future Enhancements

- [ ] Deep sleep mode between checks
- [ ] Web interface for configuration
- [ ] Multiple alert phone numbers
- [ ] Geofencing alerts
- [ ] Low battery SMS warning
- [ ] Historical location tracking
- [ ] Theft recovery mode (continuous GPS updates)

## Configuration Options

You can modify these settings in `src/main.cpp`:

```cpp
#define ALERT_PHONE_NUMBER  "+1234567890"  // Your phone number
#define MOVEMENT_THRESHOLD  2.0            // Movement sensitivity (m/s²)
#define CHECK_INTERVAL      1000           // How often to check (ms)
#define ALERT_COOLDOWN      300000         // Time between alerts (ms)
```

## License

This project is open source and available for personal and commercial use.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues.

## Acknowledgments

- Built with [PlatformIO](https://platformio.org/)
- Uses [TinyGSM](https://github.com/vshymanskyy/TinyGSM) library
- Uses [Adafruit LIS3DH](https://github.com/adafruit/Adafruit_LIS3DH) library
