# Hardware Wiring Guide

## LilyGO A7670E Board

The LilyGO A7670E is an all-in-one board that includes:
- ESP32 microcontroller
- A7670E 4G/LTE modem
- GPS module
- LIS3DH accelerometer (on some versions)

Most connections are already made on the board. You only need to add:

## Required Connections

### 1. Power Supply
- **USB-C Port**: 5V power input
- **Battery Connector**: For portable operation (3.7V LiPo battery recommended)

### 2. SIM Card
- Insert SIM card into the SIM card slot
- Make sure the SIM card is activated and has SMS capability
- Check the correct orientation (usually printed on the board)

### 3. GPS Antenna
- Connect the GPS antenna to the IPEX/U.FL connector labeled "GPS"
- For best results, use an active GPS antenna
- Place antenna with clear view of the sky

### 4. LTE Antenna
- Connect the LTE antenna to the main antenna connector
- Usually labeled "MAIN" or "LTE"

## Optional Connections

### 5. Secure Button (Optional)
```
GPIO 35 ----[Button]---- GND
```

- Connect a momentary push button between GPIO 35 and GND
- Internal pull-up resistor is enabled in code
- Press to toggle arm/disarm state

### 6. External LED (Optional)
If you want a more visible status LED:

```
GPIO 12 ----[330Ω]----[LED]---- GND
```

- The board usually has a built-in LED on GPIO 12
- You can add an external LED for better visibility
- Use a current-limiting resistor (330Ω recommended)

## Board Pinout Reference

```
                    LilyGO A7670E
                   ┌─────────────┐
    USB-C Power ───┤             │
                   │   ESP32     │
    Battery     ───┤   A7670E    │
                   │   GPS       │
    SIM Slot    ───┤   LIS3DH    │
                   │             │
    GPIO 35     ───┤             │─── LED (GPIO 12)
    (Button)       │             │
                   │             │
    GPS Antenna ───┤             │─── LTE Antenna
                   │             │
                   └─────────────┘
```

## Power Considerations

### USB Powered
- Connect via USB-C cable
- Provides stable 5V power
- Suitable for testing and development

### Battery Powered
- Use 3.7V LiPo battery (1000-3000mAh recommended)
- Connect to battery connector (usually JST 2.0mm connector)
- Board has built-in charging circuit
- Typical battery life:
  - 1000mAh: 4-6 hours continuous monitoring
  - 2000mAh: 8-12 hours continuous monitoring
  - 3000mAh: 12-18 hours continuous monitoring

### Power Banks
- For longer operation without wiring, use a power bank
- Connect via USB-C cable
- 10,000mAh power bank: ~2-3 days continuous monitoring

## Weatherproofing (For Outdoor Use)

If mounting on your bike:

1. **Enclosure**: Use a waterproof enclosure (IP65 or higher)
2. **Antenna Placement**: GPS antenna needs clear sky view
3. **Mounting**: Use vibration-dampening mounts
4. **Cables**: Use waterproof cable glands for any external connections

## Recommended Accessories

- **GPS Antenna**: Active GPS antenna with IPEX connector
- **LTE Antenna**: 4G/LTE antenna with IPEX connector
- **Battery**: 3.7V LiPo battery (2000mAh or higher)
- **Enclosure**: Waterproof case for outdoor mounting
- **SIM Card**: Prepaid SIM with SMS capability

## Testing the Setup

1. **Power**: LED should turn on when powered
2. **Modem**: Serial monitor should show "Modem initialized successfully"
3. **GPS**: Wait 2-5 minutes outdoors for GPS fix
4. **SMS**: Test by sending `arm` command and moving the device
5. **Button**: If installed, press to toggle arm/disarm state

## Troubleshooting Hardware

### No Power
- Check USB cable and power source
- Check battery connection and voltage
- Check if battery is charged

### GPS Not Working
- Ensure GPS antenna is connected
- Check antenna placement (needs sky view)
- Wait longer for initial GPS fix (up to 5 minutes)
- Try outdoors or near a window

### Modem Not Responding
- Check SIM card is properly inserted
- Verify SIM card is activated
- Check antenna connections
- Try power cycling the board

### Button Not Working
- Verify button is connected to GPIO 35 and GND
- Check button orientation
- Test button with multimeter for continuity

## Safety Notes

⚠️ **Important Safety Information:**

- Never short-circuit the battery
- Use proper voltage ratings (3.7V for battery, 5V for USB)
- Don't expose to extreme temperatures
- Keep away from water unless properly sealed
- Disconnect power before making wiring changes
- Use proper fuses if connecting to bike's electrical system

## Additional Resources

- [LilyGO A7670E GitHub](https://github.com/Xinyuan-LilyGO/LilyGO-T-A7670E)
- [ESP32 Pinout Reference](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)
- [A7670E Datasheet](https://simcom.ee/documents/?dir=SIM7670E)
