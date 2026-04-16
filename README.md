# Health Monitoring & Fall Detection System (STM32F4)

A real-time embedded health monitoring system built on the STM32F407VGTx (ARM Cortex-M4) that performs heart rate monitoring and fall detection, streams telemetry over Bluetooth, and triggers emergency alerts under critical conditions.

---

## Key Features

- Real-time heart rate (BPM) estimation using IR signal peak detection  
- Fall detection using accelerometer magnitude vector  
- Dual I2C architecture to eliminate bus conflicts  
- Wireless data streaming via Bluetooth (HC-05)  
- Emergency alert system (LED + buzzer + UART message)  
- Register-level sensor interfacing without external libraries  

---

## System Architecture

                ┌──────────────────────────┐
                │   STM32F407VGTx MCU     │
                │  (ARM Cortex-M4 @168MHz)│
                └──────────┬──────────────┘
                           │
     ┌─────────────────────┼─────────────────────┐
     │                     │                     │
  I2C1                  I2C2                 USART2
(PB6/PB7)            (PB10/PB11)           (PA2/PA3)
     │                     │                     │
 MPU6050             MAX30100               HC-05
(Accel/Gyro)      (Heart Rate)          (Bluetooth)
     │                                         │
     └────────────── GPIO (PC13) ───────────────┘
                          │
                      LED / Buzzer

---

## STM32F4 Pin Mapping

| Function       | STM32 Pin | Connected Device |
|----------------|----------|------------------|
| I2C1 SCL       | PB6      | MPU6050          |
| I2C1 SDA       | PB7      | MPU6050          |
| I2C2 SCL       | PB10     | MAX30100         |
| I2C2 SDA       | PB11     | MAX30100         |
| USART2 TX      | PA2      | HC-05 RX         |
| USART2 RX      | PA3      | HC-05 TX         |
| Status LED     | PC13     | Onboard LED      |
| Buzzer         | PB0      | Active buzzer    |

Electrical Notes:
- All sensors operate at 3.3V logic
- Use voltage divider for HC-05 RX
- Use 4.7kΩ pull-up resistors on I2C lines

---

## Sensor Configuration

### MAX30100

| Register | Value | Description |
|----------|-------|-------------|
| 0x06     | 0x40  | Reset       |
| 0x06     | 0x03  | SpO2 Mode   |
| 0x07     | 0x27  | Sample Rate |
| 0x09     | 0x24  | LED Current |

### MPU6050

| Register | Value | Description |
|----------|-------|-------------|
| 0x6B     | 0x00  | Wake-up     |

---

## Detection Logic

Acceleration magnitude:

|A| = sqrt(Ax^2 + Ay^2 + Az^2)

Trigger condition:
- Fall detected if magnitude > 20000

Heart rate:
- BPM = 60000 / (time between peaks)

---

## Emergency Conditions

| Condition       | Threshold        |
|----------------|-----------------|
| Fall            | > 20000         |
| Bradycardia     | BPM < 50        |
| Tachycardia     | BPM > 120       |

---

## Data Output

AX: <val> AY: <val> AZ: <val> | BPM: <val> | IR: <val>
EMERGENCY DETECTED

- Baud rate: 9600
- Interval: 200 ms

---

## Project Structure

health-monitoring-system/
│── main_project/
│   └── main_project.ino
│
│── max30100_test/
│   └── max30100_init.ino
│
│── mpu6050_test/
│   └── mpu6050_init.ino
│
└── README.md

---

## Resume-Ready Description

Embedded Health Monitoring & Fall Detection System | STM32F4

- Designed and implemented a real-time embedded system using STM32F407VGTx (ARM Cortex-M4) for heart rate monitoring and fall detection  
- Interfaced MAX30100 (I2C) for IR-based BPM estimation and MPU6050 for motion sensing using dual I2C buses  
- Developed peak detection algorithm for heart rate calculation and vector-based acceleration model for fall detection  
- Integrated UART-based Bluetooth (HC-05) for real-time telemetry streaming to mobile devices  
- Implemented threshold-based emergency alert system (LED, buzzer, and wireless alert)  
- Configured sensors using low-level register programming without external libraries  

---

## License

MIT License
