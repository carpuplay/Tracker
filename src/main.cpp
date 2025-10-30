/*
 * Bike GPS Tracker with SMS Alert
 * Hardware: LilyGO A7670E
 * 
 * Features:
 * - GPS tracking
 * - Movement detection using accelerometer
 * - SMS alerts when bike is moved while secured
 * - Secure/Arm mode via serial command or button
 */

#define TINY_GSM_MODEM_SIM7600
#define TINY_GSM_RX_BUFFER 1024

#include <TinyGsmClient.h>
#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

// Pin definitions for LilyGO A7670E
#define MODEM_TX            26
#define MODEM_RX            27
#define MODEM_PWRKEY        4
#define MODEM_DTR           32
#define MODEM_RI            33
#define MODEM_FLIGHT        25
#define MODEM_STATUS        34

#define I2C_SDA             21
#define I2C_SCL             22

#define LED_PIN             12
#define SECURE_BUTTON_PIN   35  // Button to arm/disarm tracker

// Configuration
#define ALERT_PHONE_NUMBER  "+1234567890"  // Change this to your phone number
#define MOVEMENT_THRESHOLD  2.0  // m/s² - sensitivity for movement detection
#define CHECK_INTERVAL      1000 // ms - how often to check for movement
#define ALERT_COOLDOWN      300000 // ms - 5 minutes between alerts

// Global objects
HardwareSerial SerialAT(1);
TinyGsm modem(SerialAT);
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

// State variables
bool isSecured = false;
bool gpsFixed = false;
float securedLat = 0.0;
float securedLon = 0.0;
float securedAccelX = 0.0;
float securedAccelY = 0.0;
float securedAccelZ = 0.0;
unsigned long lastAlertTime = 0;
unsigned long lastCheckTime = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n=== Bike GPS Tracker Starting ===");
  
  // Setup LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Setup secure button
  pinMode(SECURE_BUTTON_PIN, INPUT_PULLUP);
  
  // Setup modem pins
  pinMode(MODEM_PWRKEY, OUTPUT);
  pinMode(MODEM_FLIGHT, OUTPUT);
  digitalWrite(MODEM_PWRKEY, LOW);
  digitalWrite(MODEM_FLIGHT, HIGH);
  
  // Initialize I2C for accelerometer
  Wire.begin(I2C_SDA, I2C_SCL);
  
  // Initialize accelerometer
  if (!lis.begin(0x18)) {
    Serial.println("Failed to initialize accelerometer!");
    Serial.println("Continuing without motion detection...");
  } else {
    Serial.println("Accelerometer initialized");
    lis.setRange(LIS3DH_RANGE_4_G);
    lis.setDataRate(LIS3DH_DATARATE_50_HZ);
  }
  
  // Initialize modem
  Serial.println("Initializing modem...");
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  
  // Power on modem
  powerOnModem();
  
  // Initialize modem
  if (!modem.init()) {
    Serial.println("Failed to initialize modem!");
    while(1) {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      delay(100);
    }
  }
  
  Serial.println("Modem initialized successfully");
  
  // Get modem info
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem Info: ");
  Serial.println(modemInfo);
  
  // Enable GPS
  Serial.println("Enabling GPS...");
  enableGPS();
  
  Serial.println("\n=== Setup Complete ===");
  Serial.println("Commands:");
  Serial.println("  'arm' or 'secure' - Secure the bike (enable alerts)");
  Serial.println("  'disarm' or 'unlock' - Disarm the tracker");
  Serial.println("  'status' - Show current status");
  Serial.println("  'gps' - Show GPS location");
  
  // Blink LED to show ready
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

void loop() {
  // Check for serial commands
  checkSerialCommands();
  
  // Check secure button
  static bool lastButtonState = HIGH;
  bool buttonState = digitalRead(SECURE_BUTTON_PIN);
  if (buttonState == LOW && lastButtonState == HIGH) {
    delay(50); // Debounce
    if (digitalRead(SECURE_BUTTON_PIN) == LOW) {
      toggleSecureMode();
    }
  }
  lastButtonState = buttonState;
  
  // Update GPS location
  updateGPS();
  
  // If secured, check for movement
  if (isSecured && (millis() - lastCheckTime >= CHECK_INTERVAL)) {
    lastCheckTime = millis();
    checkForMovement();
  }
  
  // Blink LED if secured
  if (isSecured) {
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink > 1000) {
      lastBlink = millis();
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
  }
  
  delay(100);
}

void powerOnModem() {
  Serial.println("Powering on modem...");
  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(1000);
  digitalWrite(MODEM_PWRKEY, LOW);
  delay(3000);
}

void enableGPS() {
  // Send AT commands to enable GPS
  SerialAT.println("AT+CGPS=1");
  delay(1000);
  
  // Wait for response
  String response = "";
  unsigned long timeout = millis() + 2000;
  while (millis() < timeout) {
    if (SerialAT.available()) {
      response += (char)SerialAT.read();
    }
  }
  
  Serial.print("GPS Enable Response: ");
  Serial.println(response);
}

void updateGPS() {
  static unsigned long lastGPSCheck = 0;
  if (millis() - lastGPSCheck < 5000) {
    return; // Check every 5 seconds
  }
  lastGPSCheck = millis();
  
  float lat, lon;
  if (modem.getGPS(&lat, &lon)) {
    gpsFixed = true;
    if (!isSecured) {
      // Update current location only when not secured
      securedLat = lat;
      securedLon = lon;
    }
  } else {
    gpsFixed = false;
  }
}

void checkForMovement() {
  // Check accelerometer
  sensors_event_t event;
  lis.getEvent(&event);
  
  float deltaX = abs(event.acceleration.x - securedAccelX);
  float deltaY = abs(event.acceleration.y - securedAccelY);
  float deltaZ = abs(event.acceleration.z - securedAccelZ);
  
  float totalMovement = sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);
  
  // Check if movement exceeds threshold
  if (totalMovement > MOVEMENT_THRESHOLD) {
    // Check cooldown
    if (millis() - lastAlertTime > ALERT_COOLDOWN) {
      Serial.println("MOVEMENT DETECTED!");
      Serial.print("Delta: ");
      Serial.println(totalMovement);
      sendAlert();
      lastAlertTime = millis();
    }
  }
}

void sendAlert() {
  Serial.println("Sending SMS alert...");
  
  // Get current GPS location
  float currentLat, currentLon;
  bool hasCurrentGPS = modem.getGPS(&currentLat, &currentLon);
  
  // Build message
  String message = "ALERT: Your bike has been moved!\n";
  
  if (gpsFixed && hasCurrentGPS) {
    message += "Last location: https://maps.google.com/?q=";
    message += String(securedLat, 6);
    message += ",";
    message += String(securedLon, 6);
    message += "\nCurrent: https://maps.google.com/?q=";
    message += String(currentLat, 6);
    message += ",";
    message += String(currentLon, 6);
  } else {
    message += "GPS signal not available";
  }
  
  // Send SMS
  if (modem.sendSMS(ALERT_PHONE_NUMBER, message)) {
    Serial.println("SMS sent successfully!");
    // Flash LED rapidly
    for(int i = 0; i < 10; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(50);
      digitalWrite(LED_PIN, LOW);
      delay(50);
    }
  } else {
    Serial.println("Failed to send SMS!");
  }
}

void toggleSecureMode() {
  if (isSecured) {
    disarmTracker();
  } else {
    armTracker();
  }
}

void armTracker() {
  Serial.println("\n=== ARMING TRACKER ===");
  
  // Get current accelerometer reading
  sensors_event_t event;
  lis.getEvent(&event);
  securedAccelX = event.acceleration.x;
  securedAccelY = event.acceleration.y;
  securedAccelZ = event.acceleration.z;
  
  // Get current GPS location
  float lat, lon;
  if (modem.getGPS(&lat, &lon)) {
    securedLat = lat;
    securedLon = lon;
    gpsFixed = true;
    Serial.print("Secured at: ");
    Serial.print(securedLat, 6);
    Serial.print(", ");
    Serial.println(securedLon, 6);
  } else {
    Serial.println("Warning: GPS not fixed!");
  }
  
  isSecured = true;
  Serial.println("Tracker ARMED - Movement detection active");
  
  // Flash LED to confirm
  for(int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}

void disarmTracker() {
  Serial.println("\n=== DISARMING TRACKER ===");
  isSecured = false;
  digitalWrite(LED_PIN, LOW);
  Serial.println("Tracker DISARMED");
  
  // Flash LED to confirm
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(300);
    digitalWrite(LED_PIN, LOW);
    delay(300);
  }
}

void checkSerialCommands() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toLowerCase();
    
    if (command == "arm" || command == "secure") {
      armTracker();
    } 
    else if (command == "disarm" || command == "unlock") {
      disarmTracker();
    }
    else if (command == "status") {
      printStatus();
    }
    else if (command == "gps") {
      printGPS();
    }
    else {
      Serial.println("Unknown command. Available: arm, disarm, status, gps");
    }
  }
}

void printStatus() {
  Serial.println("\n=== STATUS ===");
  Serial.print("Tracker State: ");
  Serial.println(isSecured ? "ARMED" : "DISARMED");
  Serial.print("GPS Fixed: ");
  Serial.println(gpsFixed ? "Yes" : "No");
  
  if (gpsFixed) {
    Serial.print("Location: ");
    Serial.print(securedLat, 6);
    Serial.print(", ");
    Serial.println(securedLon, 6);
  }
  
  sensors_event_t event;
  if (lis.getEvent(&event)) {
    Serial.print("Acceleration: X=");
    Serial.print(event.acceleration.x);
    Serial.print(" Y=");
    Serial.print(event.acceleration.y);
    Serial.print(" Z=");
    Serial.println(event.acceleration.z);
  }
  
  Serial.println("===============");
}

void printGPS() {
  Serial.println("\n=== GPS INFO ===");
  
  float lat, lon;
  if (modem.getGPS(&lat, &lon)) {
    Serial.print("Latitude: ");
    Serial.println(lat, 6);
    Serial.print("Longitude: ");
    Serial.println(lon, 6);
    Serial.print("Google Maps: https://maps.google.com/?q=");
    Serial.print(lat, 6);
    Serial.print(",");
    Serial.println(lon, 6);
  } else {
    Serial.println("GPS not fixed");
  }
  
  Serial.println("================");
}
