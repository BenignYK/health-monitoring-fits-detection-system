// MPU6050 initialisation code here
#include <Wire.h>

// ================= UART =================
HardwareSerial Serial2(USART2); // use default UART2 pins (PA2 TX, PA3 RX)

// ================= MPU6050 =================
#define MPU6050_ADDR 0x68

int16_t Ax, Ay, Az;

// ================= SETUP =================
void setup() {

  Serial2.begin(9600);   // Bluetooth
  Wire.begin();          // I2C1 (PB6, PB7)

  delay(1000);

  Serial2.println("SYSTEM STARTED");

  // -------- CHECK MPU6050 --------
  Wire.beginTransmission(MPU6050_ADDR);
  if (Wire.endTransmission() == 0) {
    Serial2.println("MPU6050 FOUND ✅");
  } else {
    Serial2.println("MPU6050 NOT FOUND ❌");
  }

  // -------- WAKE UP MPU6050 --------
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B);   // Power register
  Wire.write(0x00);   // Wake up
  Wire.endTransmission();

  delay(1000);
}

// ================= LOOP =================
void loop() {

  readMPU6050();

  Serial2.print("AX: "); Serial2.print(Ax);
  Serial2.print(" AY: "); Serial2.print(Ay);
  Serial2.print(" AZ: "); Serial2.println(Az);

  delay(500);
}

// ================= FUNCTION =================
void readMPU6050() {

  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B);   // Start from ACCEL_XOUT_H
  Wire.endTransmission(false);

  Wire.requestFrom(MPU6050_ADDR, 6);

  if (Wire.available() == 6) {
    Ax = (Wire.read() << 8) | Wire.read();
    Ay = (Wire.read() << 8) | Wire.read();
    Az = (Wire.read() << 8) | Wire.read();
  }
}
