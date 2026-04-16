// MAX30100 initialisation code here
#include <Wire.h>

// ================= UART =================
HardwareSerial Serial2(USART2);

// ================= I2C2 =================
TwoWire Wire2(PB11, PB10);

#define MAX30100_ADDR 0x57

// ================= BPM VARIABLES =================
int lastValue = 0;
unsigned long lastBeatTime = 0;

// ================= SETUP =================
void setup() {

  Serial2.begin(9600);
  Wire2.begin();

  delay(1000);

  Serial2.println("START");

  // Check sensor
  Wire2.beginTransmission(MAX30100_ADDR);
  if (Wire2.endTransmission() == 0) {
    Serial2.println("MAX FOUND");
  } else {
    Serial2.println("MAX NOT FOUND");
    while (1);
  }

  // ---------- CONFIG ----------
  
  writeRegister(0x06, 0x40);  // Reset
  delay(100);

  writeRegister(0x06, 0x03);  // SPO2 mode
  writeRegister(0x07, 0x27);  // SPO2 config
  writeRegister(0x09, 0x24);  // LED current

  Serial2.println("INIT DONE");
}

// ================= LOOP =================
void loop() {

  int current = readIR();

  // Print raw signal
  Serial2.print("IR: ");
  Serial2.print(current);

  // ---------- PEAK DETECTION ----------
  if (current > lastValue + 300) {

    unsigned long now = millis();

    if (lastBeatTime != 0) {
      int bpm = 60000 / (now - lastBeatTime);

      // Filter unrealistic values
      if (bpm > 40 && bpm < 180) {
        Serial2.print("  BPM: ");
        Serial2.print(bpm);
      }
    }

    lastBeatTime = now;
  }

  Serial2.println();

  lastValue = current;

  delay(50);
}

// ================= FUNCTIONS =================

void writeRegister(uint8_t reg, uint8_t value) {
  Wire2.beginTransmission(MAX30100_ADDR);
  Wire2.write(reg);
  Wire2.write(value);
  Wire2.endTransmission();
}

uint16_t readIR() {

  Wire2.beginTransmission(MAX30100_ADDR);
  Wire2.write(0x05);
  Wire2.endTransmission(false);

  Wire2.requestFrom(MAX30100_ADDR, 2);

  if (Wire2.available() >= 2) {
    return (Wire2.read() << 8) | Wire2.read();
  }

  return 0;
}
