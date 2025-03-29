# 🔒 ESP32 WiFi Threat Detector

> ⚠️ **Disclaimer**: This project is intended for **educational and ethical use only**. Do not use this code for any unauthorized network monitoring or malicious activities. The developer assumes no responsibility for misuse.

## 📡 Overview

This project transforms an **ESP32 microcontroller** into a lightweight **WiFi threat detection system**. By leveraging the ESP32's **promiscuous mode**, the device can passively sniff WiFi management frames in its vicinity and detect common wireless attacks in real-time.

Detected threats include:
- 🚫 **Deauthentication Attacks**
- 🔌 **Disassociation Attacks**
- 📡 **Probe Request Floods**
- 🛰️ **Fake Access Point / Beacon Floods**

When a threat is detected, the ESP32 sends an alert over **Bluetooth Serial** to any connected device, allowing for immediate, real-time notifications.

## 🧠 Features

- Sniffs WiFi packets without joining a network
- Identifies attack patterns using IEEE 802.11 frame types
- Sends Bluetooth alerts for live monitoring
- Lightweight and low-power implementation

## 🛠️ Requirements

- ESP32 development board
- Arduino IDE or PlatformIO
- Libraries:
  - `WiFi.h`
  - `esp_wifi.h`
  - `BluetoothSerial.h`

## 🚀 How It Works

The ESP32 is configured to enter **promiscuous mode**, enabling it to capture all WiFi packets in range. It parses **management frames**, checks the **frame subtype**, and identifies suspicious behavior based on frequency and type. If a threat pattern is matched, a warning is sent via Bluetooth.

## 📦 Getting Started

1. Clone this repo and open the `.ino` file in the Arduino IDE.
2. Select your ESP32 board under Tools → Board.
3. Select Tools → Partition Scheme: → No OTA(2MB APP/2MD SPIFFS)
4. Upload the code to your ESP32.
5. Pair your phone over Bluetooth.
6. Monitor Bluetooth serial output for alerts using (e.g. Bluetooth serial Terminal <avaliable on android>).

## ✅ Example Alerts

Deauth Attack Detected! 
Probe Request Flood!

