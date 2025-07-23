#include <Wire.h>               // Include wire library for I2C communication
#include <Adafruit_GFX.h>        // Include Adafruit graphics library
#include <Adafruit_SSD1306.h>    // Include the OLED display library
#include <Ultrasonic.h>          // Include ultrasonic sensor library

// Pin Definitions
#define TRIG_PIN 4               // Ultrasonic sensor trig pin
#define ECHO_PIN 5               // Ultrasonic sensor echo pin

// Create Ultrasonic sensor object
Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);

// OLED Display Settings
#define SCREEN_WIDTH 128         // OLED display width
#define SCREEN_HEIGHT 64         // OLED display height
#define OLED_RESET -1            // Reset pin for OLED (use -1 if no reset pin)
#define SCREEN_ADDRESS 0x3C      // Default I2C address for SSD1306 OLED

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // I2C address is 0x3C

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Initialize the OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {  // Use 0x3C as I2C address
    Serial.println(F("SSD1306 allocation failed"));
    while (true);  // Loop forever if initialization fails
  }
  display.display();
  delay(2000);  // Wait for 2 seconds for the screen to initialize
  
  // Clear the display
  display.clearDisplay();
  display.setTextSize(1);      // Set text size to normal
  display.setTextColor(SSD1306_WHITE);  // Set text color to white
  display.setCursor(0, 0);     // Set the cursor to top-left
}

void loop() {
  // Measure distance with the ultrasonic sensor
  long distance = ultrasonic.read();  // Distance in cm
  
  // Display the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Clear the previous content on the display
  display.clearDisplay();
  
  // Display the distance on OLED
   display.setCursor(0, 0);  // Start at top-left corner
    display.print("Distance: ");
  display.print(distance);
  display.setCursor(64,32);  // Start at top-left corner
  display.print("Pothole Depth: ");
  
  // Classify pothole depth
  if (distance <= 5) {
    display.print("Shallow (<5 cm)");
  } 
  else if (distance > 5 && distance <= 15) {
    display.print("Moderate (5-15 cm)");
  }
  else {
    display.print("Severe (>15 cm)");
  }

  // Update the display
  display.display();
  
  // Wait for 1 second before next reading
  delay(1000);
}
