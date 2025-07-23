#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// I2C Addresses

#define ACCEL_I2C_ADDR 0x53  
#define OLED_I2C_ADDR  0x3C  

// OLED Display Config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Accelerometer Config (Explicit I2C Address)
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(ACCEL_I2C_ADDR);

// Tilt threshold for pothole detection
#define TILT_THRESHOLD 2.0  

// Motor Driver Pins (L298N)
#define ENA  25
#define IN1  26
#define IN2  27
#define ENB  14
#define IN3  12
#define IN4  13

// Ultrasonic Sensor Pins
#define TRIG 5
#define ECHO 18

// Buzzer & LED Pins
#define BUZZER 4
#define RED_LED 2
#define GREEN_LED 15

void setup() {
  Serial.begin(115200);
  
  // Start I2C with explicit pins
  Wire.begin(21, 22);  

  // Initialize OLED with explicit address
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
    Serial.println("OLED not found!");
    while (1);
  }
  display.clearDisplay();
  display.display();

  // Initialize ADXL345 with explicit address
  if (!accel.begin(ACCEL_I2C_ADDR)) {
    Serial.println("ADXL345 not detected!");
    while (1);
  }

  // Setup motor driver
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Setup Ultrasonic Sensor
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // Setup Buzzer & LEDs
  pinMode(BUZZER, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
  moveForward();
}

void loop() {
  sensors_event_t event;
  accel.getEvent(&event);
  float tilt = sqrt(sq(event.acceleration.x) + sq(event.acceleration.y));

  Serial.print("Tilt: ");
  Serial.println(tilt);

  float depth = measureDepth();
  Serial.print("Pothole Depth: ");
  Serial.print(depth);
  Serial.println(" cm");

  if (tilt > TILT_THRESHOLD) {
    Serial.println("Pothole Detected! Stopping car...");
    stopCar();
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER, HIGH);
    updateOLED(tilt, depth, "POTHOLE DETECTED!");
    delay(1000);
    moveForward();
  } else {
    Serial.println("No Pothole Detected.");
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BUZZER, LOW);
    updateOLED(tilt, depth, "No Pothole");
  }
  
  delay(500);
}

// Move car forward
void moveForward() {
  digitalWrite(IN1, LOW );
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Stop car
void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Measure pothole depth using ultrasonic sensor
float measureDepth() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  float distance = (duration * 0.0343) / 2;  // Convert to cm
  return distance;
}

// Update OLED display
void updateOLED(float tilt, float depth, String status) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0, 0);
  display.print("Tilt: ");
  display.print(tilt);
  display.println(" m/s2");
  
  display.setCursor(0, 15);
  display.print("Depth: ");
  display.print(depth);
  display.println(" cm");

  display.setCursor(0, 30);
  display.print("Status: ");
  display.println(status);
  
  display.display();
}
