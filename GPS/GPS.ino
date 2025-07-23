#include <TinyGPS++.h>
#include <HardwareSerial.h>

// GPS object
TinyGPSPlus gps;

// UART2 for GPS
HardwareSerial gpsSerial(2);  // Use UART2 on ESP32

// Define pins for GPS
#define RXD2 16  // GPS TX -> ESP32 GPIO16 (RX)
#define TXD2 17  // GPS RX -> ESP32 GPIO17 (TX)

void setup() {
  Serial.begin(115200);
  while (!Serial);  // Wait for Serial Monitor to open
  delay(1000);
  Serial.println("Initializing GPS Module...");

  // Start UART2 for GPS communication
  gpsSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  // Read and parse GPS data
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);

    // Optional: Print raw GPS data for debugging
    Serial.write(c);
  }

  // Print location + IST time if updated
  if (gps.location.isUpdated() && gps.time.isUpdated()) {
    double latitude = gps.location.lat();
    double longitude = gps.location.lng();

    // Get UTC Time
    int hourUTC = gps.time.hour();
    int minuteUTC = gps.time.minute();
    int secondUTC = gps.time.second();

    // Convert to IST (UTC + 5:30)
    int hourIST = (hourUTC + 5) % 24;
    int minuteIST = minuteUTC + 30;
    if (minuteIST >= 60) {
      minuteIST -= 60;
      hourIST = (hourIST + 1) % 24;
    }

    // Display data
    Serial.println("\n--- Location and Time (IST) ---");
    Serial.print("Latitude: ");
    Serial.println(latitude, 6);
    Serial.print("Longitude: ");
    Serial.println(longitude, 6);
    Serial.print("Time (IST): ");
    Serial.printf("%02d:%02d:%02d\n", hourIST, minuteIST, secondUTC);
    Serial.println("-------------------------------");
  }

  // Print a waiting message if no GPS fix yet
  if (!gps.location.isValid()) {
    Serial.println("Waiting for GPS fix...");
    delay(2000);
  }
}
