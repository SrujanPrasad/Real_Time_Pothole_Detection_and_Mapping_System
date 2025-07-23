#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

// Initialize ADXL345
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(0x53);

// Tilt threshold for pothole detection
#define TILT_THRESHOLD 1.5  

// Motor Driver Pins (L298N)
#define ENA  25   // Speed control Motor A
#define IN1  26   // Direction control Motor A
#define IN2  27
#define ENB  14   // Speed control Motor B
#define IN3  12   // Direction control Motor B
#define IN4  13

void setup() {
  Serial.begin(115200);
  
  // Start I2C
  Wire.begin(21, 22);  

  // Initialize ADXL345
  if (!accel.begin()) {
    Serial.println("ADXL345 not detected!");
    while (1);
  }

  Serial.println("ADXL345 Initialized!");

  // Setup motor driver pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Start motors
  analogWrite(ENA, 150);  // Set speed (0-255)
  analogWrite(ENB, 150);
  moveForward();
}

void loop() {
  sensors_event_t event;
  accel.getEvent(&event);
  // Calculate tilt using X and Y acceleration
  float tilt = sqrt(sq(event.acceleration.x) + sq(event.acceleration.y));

  Serial.print("Tilt: ");
  Serial.println(tilt);

  if (tilt > TILT_THRESHOLD) {
    Serial.println("Pothole Detected! Stopping car...");
    stopCar();
    delay(1000);  // Stop for 1 second
    moveForward();
  } else {
    Serial.println("No Pothole Detected.");
  }

  delay(500);
}

// Move car forward
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Stop car
void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
