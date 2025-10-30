# Customization Examples

Here are common customizations you might want to make to your bike tracker.

## 1. Change Alert Phone Number

**Location**: `src/main.cpp`, line 36

**Default**:
```cpp
#define ALERT_PHONE_NUMBER  "+1234567890"
```

**Your customization**:
```cpp
#define ALERT_PHONE_NUMBER  "+33612345678"  // Your actual number
```

**Important**: Always include the country code with `+`

---

## 2. Adjust Movement Sensitivity

**Location**: `src/main.cpp`, line 37

**For more sensitive detection** (detects smaller movements):
```cpp
#define MOVEMENT_THRESHOLD  1.0  // Very sensitive
```

**For less sensitive detection** (only detects larger movements):
```cpp
#define MOVEMENT_THRESHOLD  5.0  // Less sensitive
```

**Default** (balanced):
```cpp
#define MOVEMENT_THRESHOLD  2.0  // Good for most bikes
```

**Use cases**:
- Mountain bike on rough terrain: 3.0-5.0
- Road bike on smooth pavement: 1.5-2.5
- Bike in windy area: 2.5-3.5

---

## 3. Change Check Interval

**Location**: `src/main.cpp`, line 38

**For faster response** (uses more power):
```cpp
#define CHECK_INTERVAL      500  // Check every 0.5 seconds
```

**For battery saving** (slower response):
```cpp
#define CHECK_INTERVAL      2000  // Check every 2 seconds
```

**Default** (balanced):
```cpp
#define CHECK_INTERVAL      1000  // Check every 1 second
```

---

## 4. Change Alert Cooldown

**Location**: `src/main.cpp`, line 39

**For more frequent alerts**:
```cpp
#define ALERT_COOLDOWN      60000  // 1 minute between alerts
```

**For fewer alerts** (save SMS costs):
```cpp
#define ALERT_COOLDOWN      600000  // 10 minutes between alerts
```

**Default** (balanced):
```cpp
#define ALERT_COOLDOWN      300000  // 5 minutes between alerts
```

---

## 5. Multiple Alert Numbers

**Location**: `src/main.cpp`, modify the `sendAlert()` function

**Current** (around line 238):
```cpp
void sendAlert() {
  // ... existing code ...
  
  if (modem.sendSMS(ALERT_PHONE_NUMBER, message)) {
    Serial.println("SMS sent successfully!");
  }
}
```

**Modified** (send to multiple numbers):
```cpp
void sendAlert() {
  // ... existing code ...
  
  String numbers[] = {"+1234567890", "+1987654321", "+1122334455"};
  int numCount = 3;
  
  for(int i = 0; i < numCount; i++) {
    if (modem.sendSMS(numbers[i], message)) {
      Serial.print("SMS sent to: ");
      Serial.println(numbers[i]);
    }
    delay(1000);  // Wait between SMS
  }
}
```

---

## 6. Add Low Battery Warning

**Location**: `src/main.cpp`, add to `loop()` function

Add this after the movement check:

```cpp
// In loop() function, add:
void loop() {
  // ... existing code ...
  
  // Check battery voltage periodically
  static unsigned long lastBatteryCheck = 0;
  if (millis() - lastBatteryCheck > 600000) {  // Every 10 minutes
    lastBatteryCheck = millis();
    
    // Read battery voltage (adjust pin if needed)
    int batteryVoltage = analogRead(35);  // Use appropriate ADC pin
    if (batteryVoltage < 3300) {  // Below 3.3V
      String message = "WARNING: Tracker battery low!";
      modem.sendSMS(ALERT_PHONE_NUMBER, message);
    }
  }
  
  // ... rest of existing code ...
}
```

---

## 7. Custom SMS Message Format

**Location**: `src/main.cpp`, in `sendAlert()` function (around line 245)

**Current**:
```cpp
String message = "ALERT: Your bike has been moved!\n";
```

**Custom examples**:

**Minimal**:
```cpp
String message = "🚨 BIKE MOVED!\n";
```

**Detailed**:
```cpp
String message = "🚨 SECURITY ALERT 🚨\n";
message += "Your bike has been moved!\n";
message += "Time: " + String(millis() / 1000) + "s\n";
```

**Multiple languages**:
```cpp
String message = "ALERTE: Votre vélo a été déplacé!\n";  // French
// or
String message = "ALARMA: Tu bicicleta ha sido movida!\n";  // Spanish
```

---

## 8. Change LED Pin

**Location**: `src/main.cpp`, line 33

If your board uses a different LED pin:

```cpp
#define LED_PIN             2   // Common for many ESP32 boards
// or
#define LED_PIN             13  // Another common LED pin
```

---

## 9. Disable LED (Save Power)

**Location**: `src/main.cpp`, comment out LED operations

In `loop()` function, comment out the LED blinking:

```cpp
// Blink LED if secured
if (isSecured) {
  /*
  static unsigned long lastBlink = 0;
  if (millis() - lastBlink > 1000) {
    lastBlink = millis();
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
  */
}
```

---

## 10. Add Geofence Alert

**Location**: `src/main.cpp`, add new function

Add this function after `checkForMovement()`:

```cpp
void checkGeofence() {
  if (!isSecured || !gpsFixed) return;
  
  float currentLat, currentLon;
  if (!modem.getGPS(&currentLat, &currentLon)) return;
  
  // Calculate distance from secured location (simplified)
  float latDiff = abs(currentLat - securedLat);
  float lonDiff = abs(currentLon - securedLon);
  float distance = sqrt(latDiff*latDiff + lonDiff*lonDiff) * 111000; // rough meters
  
  if (distance > 100) {  // 100 meters geofence
    String message = "ALERT: Bike moved " + String(distance, 0) + "m away!";
    if (modem.sendSMS(ALERT_PHONE_NUMBER, message)) {
      Serial.println("Geofence alert sent!");
    }
  }
}
```

Then call it in `loop()`:
```cpp
void loop() {
  // ... existing code ...
  
  if (isSecured && (millis() - lastCheckTime >= CHECK_INTERVAL)) {
    lastCheckTime = millis();
    checkForMovement();
    checkGeofence();  // Add this line
  }
  
  // ... rest of code ...
}
```

---

## Testing Your Customizations

After making changes:

1. **Save the file**
2. **Rebuild**: `pio run`
3. **Upload**: `pio run --target upload`
4. **Test**: Open serial monitor and test your changes

---

## Tips for Safe Customization

✅ **DO**:
- Make one change at a time
- Test after each change
- Keep backup of working code
- Comment your changes

❌ **DON'T**:
- Change pin definitions unless you know your hardware
- Remove safety checks (cooldowns, thresholds)
- Make SMS messages too long (160 char limit for SMS)
- Set check intervals too low (<500ms, wastes power)

---

## Need More Help?

- Check the [README.md](README.md) for full documentation
- Review the code comments in `src/main.cpp`
- Open an issue on GitHub with your question
