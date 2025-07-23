#include <Wire.h>

// MPU6050 I2C address
const int MPU6050_ADDR = 0x68;

// Accelerometer registers
const int ACCEL_XOUT_H = 0x3B;
const int PWR_MGMT_1 = 0x6B;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize I2C communication
  Wire.begin();

  // Wake up MPU6050 (set power management register to 0)
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission();

  Serial.println("MPU6050 initialized successfully");
}

void loop() {
  int16_t accelX, accelY, accelZ;

  // Read accelerometer data
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(ACCEL_XOUT_H); // Starting register for accelerometer data
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 6, true); // Request 6 bytes

  accelX = (Wire.read() << 8 | Wire.read()); // X-axis
  accelY = (Wire.read() << 8 | Wire.read()); // Y-axis
  accelZ = (Wire.read() << 8 | Wire.read()); // Z-axis

  // Display accelerometer data on Serial Monitor
  Serial.print("Accel X: "); Serial.print(accelX);
  Serial.print(" | Accel Y: "); Serial.print(accelY);
  Serial.print(" | Accel Z: "); Serial.println(accelZ);

  delay(500); // Adjust delay as needed
}
