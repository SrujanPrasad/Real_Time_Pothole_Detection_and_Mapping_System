#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

// Initialize ADXL345
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(0x53);

#define TILT_THRESHOLD 1.5  // Define tilt detection threshold

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Start I2C
  Wire.begin(21, 22);  

  // Check if ADXL345 is detected
  if (!accel.begin()) {
    Serial.println("ADXL345 not detected!");
    while (1);
  }

  Serial.println("ADXL345 Initialized!");
}

void loop() {
  sensors_event_t event;
  accel.getEvent(&event);

  // Calculate tilt using X and Y acceleration
  float tilt = sqrt(sq(event.acceleration.x) + sq(event.acceleration.y));

  Serial.print("Tilt: ");
  Serial.println(tilt);

  if (tilt > TILT_THRESHOLD) {
    Serial.println("Pothole Detected!");
  } else {
    Serial.println("No Pothole Detected.");
  }

  delay(500);
}
