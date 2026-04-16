# Changelog

All notable changes to this project are documented here.
Format follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

---

## [1.0.0] - 2025

### Added
- STM32F407VGTx firmware with dual I2C bus initialisation
- MAX30100 heart rate sensor driver with peak-detection BPM algorithm
- MPU6050 accelerometer driver with resultant magnitude fall detection
- HC-05 Bluetooth serial output at 9600 baud
- Active buzzer alert on PB0 for emergency conditions
- Threshold-based emergency logic: fall (magnitude > 20000), bradycardia (BPM < 50), tachycardia (BPM > 120)
- Standalone initialisation and test sketches for MAX30100 and MPU6050
