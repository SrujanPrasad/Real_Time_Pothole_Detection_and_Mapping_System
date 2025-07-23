#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed further
  }

  display.clearDisplay();   // Clear the screen
  display.setTextSize(1);   // Set text size
  display.setTextColor(SSD1306_WHITE); // Set text color to white
  display.setCursor(64, 32); // Set the cursor to the top-left corner
  display.println(F("Hello World")); // Print the message
  
  display.display(); // Update the display
  delay(2000); // Wait for 2 seconds
}

void loop() {
  // You can add other code here for looping functionality if needed
}
