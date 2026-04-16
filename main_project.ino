#include <Wire.h>
#include <math.h>

// ================= UART =================
HardwareSerial Serial2(USART2);

// ================= I2C =================
TwoWire Wire2(PB11, PB10); // MAX30100

#define MPU_ADDR 0x68
#define MAX_ADDR 0x57

// ================= VARIABLES =================
int16_t Ax, Ay, Az;
int lastIR = 0;
unsigned long lastBeatTime = 0;

#define LED_PIN PC13

// ================= SETUP =================
void setup() {

  pinMode(LED_PIN, OUTPUT);

  Serial2.begin(9600);

  Wire.begin();     // MPU
  Wire2.begin();    // MAX

  delay(1000);

  Serial2.println("SYSTEM STARTED");

  // MPU INIT
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  // MAX INIT
  writeMax(0x06, 0x40);
  delay(100);
  writeMax(0x06, 0x03);
  writeMax(0x07, 0x27);
  writeMax(0x09, 0x24);

  Serial2.println("INIT DONE");
}

// ================= LOOP =================
void loop() {

  // -------- MPU READ --------
  readMPU();
  long magnitude = sqrt((long)Ax*Ax + (long)Ay*Ay + (long)Az*Az);

  // -------- MAX READ --------
  int currentIR = readIR();

  int bpm = 0;

  if (currentIR > lastIR + 300) {

    unsigned long now = millis();

    if (lastBeatTime != 0) {
      bpm = 60000 / (now - lastBeatTime);
    }

    lastBeatTime = now;
  }

  lastIR = currentIR;

  // -------- EMERGENCY LOGIC --------
  bool fall = (magnitude > 20000);
  bool abnormalHR = (bpm < 50 && bpm != 0) || (bpm > 120);

  if (fall || abnormalHR) {
    digitalWrite(LED_PIN, HIGH);
    Serial2.println("⚠️ EMERGENCY DETECTED");
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // -------- PRINT DATA --------
  Serial2.print("AX: "); Serial2.print(Ax);
  Serial2.print(" AY: "); Serial2.print(Ay);
  Serial2.print(" AZ: "); Serial2.print(Az);

  Serial2.print(" | BPM: "); Serial2.print(bpm);

  Serial2.print(" | IR: "); Serial2.println(currentIR);

  delay(200);
}

// ================= FUNCTIONS =================

// MPU
void readMPU() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU_ADDR, 6);

  if (Wire.available() == 6) {
    Ax = (Wire.read()<<8)|Wire.read();
    Ay = (Wire.read()<<8)|Wire.read();
    Az = (Wire.read()<<8)|Wire.read();
  }
}

// MAX
void writeMax(uint8_t reg, uint8_t val) {
  Wire2.beginTransmission(MAX_ADDR);
  Wire2.write(reg);
  Wire2.write(val);
  Wire2.endTransmission();
}

uint16_t readIR() {
  Wire2.beginTransmission(MAX_ADDR);
  Wire2.write(0x05);
  Wire2.endTransmission(false);

  Wire2.requestFrom(MAX_ADDR, 2);

  if (Wire2.available() >= 2) {
    return (Wire2.read()<<8)|Wire2.read();
  }
  return 0;
}
