# 🚗 Smart Parking System using ESP32 and ThingSpeak

## 📌 Project Overview

The Smart Parking System is an IoT-based parking monitoring system designed to detect the availability of parking slots in real time.

The system uses an ESP32 and HC-SR04 ultrasonic sensors to detect whether a vehicle is present in each parking slot. The collected data is processed by the ESP32 and uploaded to ThingSpeak for cloud-based monitoring.

## ✨ Features

- Real-time parking slot detection
- Two parking slots
- HC-SR04 ultrasonic sensors
- ESP32-based control
- Automatic calculation of available and occupied slots
- Cloud monitoring using ThingSpeak
- Distance monitoring of each parking slot

## 🛠️ Hardware Components

- ESP32
- 2 × HC-SR04 Ultrasonic Sensors
- LEDs
- Resistors
- Jumper Wires

## 💻 Software and Platforms

- Arduino/C++
- Cirkit Designer
- ThingSpeak

## 🔌 Pin Connections

| Component | Pin | ESP32 GPIO |
|---|---|---|
| Slot 1 HC-SR04 | TRIG | GPIO 5 |
| Slot 1 HC-SR04 | ECHO | GPIO 18 |
| Slot 2 HC-SR04 | TRIG | GPIO 16 |
| Slot 2 HC-SR04 | ECHO | GPIO 17 |

## ⚙️ Working Principle

Each HC-SR04 ultrasonic sensor measures the distance between the sensor and the vehicle.

The ESP32 compares the measured distance with a predefined threshold of 20 cm.

- Distance ≤ 20 cm → **Slot Occupied**
- Distance > 20 cm → **Slot Available**

The ESP32 then calculates the total number of available and occupied parking slots.

## 📊 Parking Logic

| Slot 1 | Slot 2 | Available Slots | Occupied Slots |
|---|---|---:|---:|
| Available | Available | 2 | 0 |
| Occupied | Available | 1 | 1 |
| Available | Occupied | 1 | 1 |
| Occupied | Occupied | 0 | 2 |

## ☁️ ThingSpeak Cloud Monitoring

The parking information is uploaded to ThingSpeak using the ESP32's Wi-Fi connection.

The system sends four parameters:

| ThingSpeak Field | Data |
|---|---|
| Field 1 | Slot 1 Distance |
| Field 2 | Slot 2 Distance |
| Field 3 | Available Slots |
| Field 4 | Occupied Slots |

## 🔄 System Architecture

```text
HC-SR04 Sensor 1 ──┐
                   │
HC-SR04 Sensor 2 ──┤
                   ↓
                 ESP32
                   ↓
                 Wi-Fi
                   ↓
              ThingSpeak
                   ↓
          Cloud Monitoring
```
## 🔗 Project Simulation

The complete circuit simulation is available on Cirkit Designer.

[Open Smart Parking System Simulation](https://app.cirkitdesigner.com/project/38407a15-72d3-4ca6-96a5-0e4581580a25)



