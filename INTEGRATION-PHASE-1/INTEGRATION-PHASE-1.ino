#include <Wire.h>
#include <Adafruit_MPU6050.h>

// MPU6050 Config
Adafruit_MPU6050 mpu;

// Motor Pins
#define ENA 25   // Motor A speed control
#define IN1 26   // Motor A direction control
#define IN2 27
#define ENB 14   // Motor B speed control
#define IN3 12   // Motor B direction control
#define IN4 13   // Motor B direction control

// Constants
#define TILT_THRESHOLD 1.5  // Tilt threshold (m/s²)

// Initialization
void setup() {
  Serial.begin(115200);

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Could not find MPU6050 sensor!");
    while (1);
  }

  // Initialize motor pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Start moving the car forward initially
  moveForward();

  Serial.println("System Ready");
  delay(1000);
}

// Main Loop
void loop() {
  // Read acceleration from MPU6050
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Calculate total acceleration
  float acceleration = sqrt(sq(a.acceleration.x) + sq(a.acceleration.y) + sq(a.acceleration.z));

  // Detect Pothole based on tilt (acceleration) threshold
  if (acceleration > TILT_THRESHOLD) {
    Serial.println("Pothole Detected (Tilt)!");
    stopCar();  // Stop the car if pothole is detected
  } else {
    Serial.println("No Pothole Detected");
    moveForward();  // Move forward if no pothole is detected
  }

  delay(500);
}

// Function to move the car forward
void moveForward() {
  analogWrite(ENA, 120);   // Full speed for Motor A
  analogWrite(ENB, 120);   // Full speed for Motor B

  digitalWrite(IN1, HIGH); // Motor A moves forward
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH); // Motor B moves forward
  digitalWrite(IN4, LOW);
}

// Function to stop the car
void stopCar() {
  analogWrite(ENA, 0);  // Stop Motor A
  analogWrite(ENB, 0);  // Stop Motor B

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
