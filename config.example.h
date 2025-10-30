/*
 * Configuration Example
 * Copy the settings you want to customize to src/main.cpp
 */

// ===== REQUIRED CONFIGURATION =====

// Your phone number for SMS alerts (CHANGE THIS!)
#define ALERT_PHONE_NUMBER  "+1234567890"


// ===== OPTIONAL CONFIGURATION =====

// Movement Detection Sensitivity
// Lower values = more sensitive (will trigger on smaller movements)
// Higher values = less sensitive (requires more movement to trigger)
// Typical range: 1.0 (very sensitive) to 5.0 (less sensitive)
#define MOVEMENT_THRESHOLD  2.0  // m/s²

// How often to check for movement when armed (in milliseconds)
// Lower = more responsive but uses more power
// Higher = slower response but saves power
#define CHECK_INTERVAL      1000  // 1 second

// Cooldown period between SMS alerts (in milliseconds)
// Prevents SMS spam if bike keeps moving
// 300000 = 5 minutes
#define ALERT_COOLDOWN      300000  // 5 minutes


// ===== PIN CONFIGURATION =====
// Only change these if you're using custom hardware

// Modem pins (LilyGO A7670E default)
#define MODEM_TX            26
#define MODEM_RX            27
#define MODEM_PWRKEY        4
#define MODEM_DTR           32
#define MODEM_RI            33
#define MODEM_FLIGHT        25
#define MODEM_STATUS        34

// I2C pins for accelerometer
#define I2C_SDA             21
#define I2C_SCL             22

// Other pins
#define LED_PIN             12
#define SECURE_BUTTON_PIN   35


// ===== ADVANCED CONFIGURATION =====

// GPS check interval (how often to update GPS position)
// Default: 5000ms (5 seconds)
// Note: This is in the updateGPS() function

// Accelerometer settings
// Range: LIS3DH_RANGE_2_G, LIS3DH_RANGE_4_G, LIS3DH_RANGE_8_G, LIS3DH_RANGE_16_G
// Data Rate: LIS3DH_DATARATE_1_HZ to LIS3DH_DATARATE_5KHZ_LP
// These are set in setup() function
