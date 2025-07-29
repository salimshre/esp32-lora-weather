# esp32-lora-weather
LoRa-Based Wireless Weather Monitoring System Using ESP32

# LoRa-Based Wireless Weather Station 🌦️

A simple and efficient weather monitoring system using two ESP32-WROOM-32 boards and LoRa communication. This project sends temperature and humidity data wirelessly from a sensor node to a receiver node using LoRa modules.

---

## 🔌 Driver Links

To upload code to the ESP32 via USB (CP2102):

- [CP210x Windows Drivers – Random Nerd Tutorials](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)
- [Official CP210x Drivers – Silicon Labs](https://www.silabs.com/software-and-tools/usb-to-uart-bridge-vcp-drivers)

---

## 📦 List of Components

- Two **ESP32-WROOM-32** boards (Transmitter + Receiver)
- **DHT sensor** (DHT11 or DHT22)
- Two **LoRa modules** (e.g., SX1278)
- Breadboard and jumper wires
- USB cables for programming

---

## 📺 Recommended Video Tutorials

- [ESP32 with LoRa Communication](https://www.youtube.com/watch?v=fOK3zN7rK6k)
- [ESP32 with DHT11/DHT22 Sensor](https://www.youtube.com/watch?v=PBK5lEc7LMc)
- [Build LoRa Weather Station](https://www.youtube.com/watch?v=qxFJyHRV2PI)
- [LoRa Beginner Tutorial](https://www.youtube.com/watch?v=x0cWiG1bW0Y)

---

## 🛠️ Troubleshooting – Red LED Only, No Response?

### 🔴 Red LED on ESP32 – What It Means:

1. **Power Indicator**  
   A solid red LED usually means the ESP32 has power. This is normal.

2. **No Blinking or Serial Output?**  
   If your board:
   - Isn't blinking
   - Isn't showing anything in the Serial Monitor
   - Isn’t uploading code  

   Then it could be due to:
   - Bad code or a crash
   - Faulty USB cable or port
   - ESP32 stuck in boot or flash mode

---

## 🔧 Fix Steps

### ✅ Step 1: Check the Serial Monitor
- Open **Arduino IDE > Tools > Serial Monitor**
- Set **baud rate to 115200**
- Press the **RESET (EN)** button
- Look for boot logs or crash info

### ✅ Step 2: Force Boot Mode
- Hold the **BOOT** button (may be labeled IO0)
- While holding BOOT, press and release the **RESET (EN)** button
- Continue holding BOOT for 1–2 seconds, then release
- Try uploading your code again

### ✅ Step 3: Upload a Blink Sketch to Recover
```cpp
void setup() {
  pinMode(2, OUTPUT);
}

void loop() {
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  delay(500);
}



## 📺 Recommended Video Tutorials

- [ESP32 with LoRa Communication](https://www.youtube.com/watch?v=fOK3zN7rK6k)
- [ESP32 with DHT11/DHT22 Sensor](https://www.youtube.com/watch?v=PBK5lEc7LMc)
- [Build LoRa Weather Station](https://www.youtube.com/watch?v=qxFJyHRV2PI)
- [LoRa Beginner Tutorial](https://www.youtube.com/watch?v=x0cWiG1bW0Y)

---



---
##
**Youtube playlist
https://www.youtube.com/watch?v=yQxk5pGRUYg&list=PLciC3-3T2N8YkcVlGfFBNGvLlwemC4nzt
