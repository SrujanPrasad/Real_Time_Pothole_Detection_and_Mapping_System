#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

// WiFi Credentials
const char* ssid = "Wifi_SSID";
const char* password = "Wifi_Password";

// ThingSpeak
const char* server = "http://api.thingspeak.com/update";
const char* apiKey = "314BD8FFNGXEYJ3L";

// I2C Addresses
#define ACCEL_I2C_ADDR 0x53
#define OLED_I2C_ADDR  0x3C

// OLED Setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Accelerometer
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(ACCEL_I2C_ADDR);
#define TILT_THRESHOLD 2.0

// Motor Driver Pins
#define ENA  25
#define IN1  26
#define IN2  27
#define ENB  14
#define IN3  12
#define IN4  13

// Ultrasonic Sensor
#define TRIG 5
#define ECHO 18

// Buzzer and LEDs
#define BUZZER 4
#define RED_LED 2
#define GREEN_LED 15

// GPS on UART2
#define RXD2 16
#define TXD2 17
TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  // OLED Init
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
    Serial.println("OLED not found!");
    while (1);
  }
  display.clearDisplay();
  display.display();

  // ADXL Init
  if (!accel.begin(ACCEL_I2C_ADDR)) {
    Serial.println("ADXL345 not found!");
    while (1);
  }

  // Motor setup
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  analogWrite(ENA, 150); analogWrite(ENB, 150);
  moveForward();

  // Ultrasonic & Indicators
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // GPS Init
  gpsSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

void loop() {
  // Read GPS data
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  sensors_event_t event;
  accel.getEvent(&event);
  float tilt = sqrt(sq(event.acceleration.x) + sq(event.acceleration.y));
  float depth = measureDepth();

  float latitude = gps.location.isValid() ? gps.location.lat() : 0.0;
  float longitude = gps.location.isValid() ? gps.location.lng() : 0.0;

  Serial.println("--- Data ---");
  Serial.print("Tilt: "); Serial.println(tilt);
  Serial.print("Depth: "); Serial.print(depth); Serial.println(" cm");
  Serial.print("Lat: "); Serial.println(latitude, 6);
  Serial.print("Lon: "); Serial.println(longitude, 6);

  if (tilt > TILT_THRESHOLD) {
    Serial.println("Pothole Detected!");
    stopCar();
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER, HIGH);
    updateOLED(tilt, depth, "POTHOLE DETECTED!", latitude, longitude);
    sendToThingSpeak(tilt, depth, latitude, longitude);
    delay(1000);
    moveForward();
  } else {
    Serial.println("No Pothole Detected.");
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BUZZER, LOW);
    updateOLED(tilt, depth, "No Pothole", latitude, longitude);
    sendToThingSpeak(tilt, depth, latitude, longitude);
  }

  delay(500);
}

void moveForward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void stopCar() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

float measureDepth() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH);
  return (duration * 0.0343) / 2;
}

void updateOLED(float tilt, float depth, String status, float lat, float lon) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.print("Tilt: "); display.print(tilt); display.println(" m/s2");

  display.setCursor(0, 12);
  display.print("Depth: "); display.print(depth); display.println(" cm");

  display.setCursor(0, 24);
  display.print("Status: "); display.println(status);

  display.setCursor(0, 36);
  display.print("Lat: "); display.println(lat, 2);

  display.setCursor(0, 48);
  display.print("Lon: "); display.println(lon, 2);

  display.display();
}

void sendToThingSpeak(float tilt, float depth, float lat, float lon) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(server) + "?api_key=" + apiKey +
                 "&field1=" + String(tilt) +
                 "&field2=" + String(depth) +
                 "&field3=" + String(lat, 6) +
                 "&field4=" + String(lon, 6);

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.print("ThingSpeak Response: ");
      Serial.println(httpCode);
    } else {
      Serial.print("ThingSpeak Error: ");
      Serial.println(http.errorToString(httpCode));
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected!");
  }
}
