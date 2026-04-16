# Health Monitoring System

An embedded health monitoring system on the STM32F407VGTx that reads biometric and motion data, streams it wirelessly over Bluetooth, and triggers a buzzer alert when readings breach safety thresholds.

---

## Hardware

| Component      | Role                                        |
|---------------|---------------------------------------------|
| STM32F407VGTx | ARM Cortex-M4 MCU, 168 MHz                  |
| MAX30100       | Heart rate and SpO2 sensor (I2C)            |
| MPU6050        | 6-axis accelerometer and gyroscope (I2C)    |
| HC-05          | Bluetooth serial module (UART)              |
| Buzzer         | Active buzzer for emergency alert           |

---

## System Architecture

```
STM32F407VGTx
  |-- I2C1  (PB6/PB7)   --> MPU6050   (Accel / Gyro)
  |-- I2C2  (PB10/PB11) --> MAX30100  (Heart Rate / SpO2)
  |-- USART2 (PA2/PA3)  --> HC-05     (Bluetooth TX/RX)
  |-- GPIO  (PB0)       --> Buzzer    (Emergency Alert)
```

Two independent I2C buses prevent address conflicts between sensors.

---

## Pin Configuration

| Signal       | Pin  | Peripheral    |
|-------------|------|---------------|
| I2C1 SDA    | PB7  | MPU6050       |
| I2C1 SCL    | PB6  | MPU6050       |
| I2C2 SDA    | PB11 | MAX30100      |
| I2C2 SCL    | PB10 | MAX30100      |
| USART2 TX   | PA2  | HC-05 RX      |
| USART2 RX   | PA3  | HC-05 TX      |
| Buzzer      | PB0  | Active Buzzer |
| Onboard LED | PC13 | Status LED    |

> MAX30100 and MPU6050 run on 3.3V. The HC-05 RX pin requires a voltage divider when wired to the STM32 TX line.

---

## Alert Thresholds

| Condition      | Parameter                         | Threshold       |
|---------------|-----------------------------------|-----------------|
| Fall detected  | magnitude = sqrt(Ax²+Ay²+Az²)    | > 20000         |
| Bradycardia    | BPM                               | < 50            |
| Tachycardia    | BPM                               | > 120           |

BPM = 0 (no finger placed) is excluded from evaluation. When any condition is triggered, the buzzer activates, the onboard LED goes HIGH, and `EMERGENCY DETECTED` is transmitted over Bluetooth.

---

## Firmware Overview

**Initialisation:** USART2 at 9600 baud, I2C1 wakes MPU6050 (reg 0x6B = 0x00), I2C2 configures MAX30100 (soft reset, SpO2 mode, ADC range, LED amplitude).

**MAX30100 Register Map:**

| Register | Value | Purpose                        |
|---------|-------|--------------------------------|
| 0x06    | 0x40  | Soft reset                     |
| 0x06    | 0x03  | SpO2 mode                      |
| 0x07    | 0x27  | ADC range and sample rate      |
| 0x09    | 0x24  | LED pulse amplitude            |

**Main loop (200 ms cycle):** Read MPU6050 accelerometer → compute magnitude → read MAX30100 IR → derive BPM from inter-beat interval → evaluate thresholds → drive buzzer/LED → transmit data over UART.

---

## Communication Protocol

Data streams over Bluetooth at 9600 baud, newline-terminated:

```
AX: <val> AY: <val> AZ: <val> | BPM: <val> | IR: <val>
EMERGENCY DETECTED   (appended when threshold is breached)
```

---

## Nurse Monitoring via Serial Bluetooth Terminal

A nurse within Bluetooth range (~10 m) can view live readings on an Android phone using **Serial Bluetooth Terminal**.

[Download on Google Play](https://play.google.com/store/search?q=bluetooth%20serial%20terminal&c=apps&hl=en_IN)

**Setup:**
1. Pair phone with **HC-05** (PIN: `1234` or `0000`).
2. Open the app, select HC-05, set baud rate to **9600**, and connect.
3. Readings stream at 200 ms intervals. `EMERGENCY DETECTED` appears alongside the buzzer alert for immediate triage.

**Notes:** HC-05 supports one active connection at a time. The device continues alerting locally via buzzer if the Bluetooth connection drops. The app supports session logging for later review.

---

## Getting Started

**Prerequisites:** Arduino IDE with STM32duino board support, ST-LINK V2 programmer.

```bash
git clone https://github.com/<your-username>/health-monitoring-system.git
```

1. Open `src/main.ino` in Arduino IDE.
2. Board: Generic STM32F4 Series > STM32F407VGTx.
3. Upload method: ST-LINK.
4. Compile and flash.

---

## Project Structure

```
health-monitoring-system/
|-- src/
|   |-- main.ino
|-- docs/
|   |-- circuit_diagram.png
|   |-- block_diagram.png
|-- README.md
```

---

## Future Improvements

- Integrate official MAX30100 library for accurate SpO2 readings
- Use gyroscope data for orientation-based fall detection
- Add FreeRTOS for deterministic sensor scheduling
- Add SIM800L GSM module for SMS alerts beyond Bluetooth range
- Log timestamped data to SD card for clinical review

---

## License

MIT License. See `LICENSE` for details.
