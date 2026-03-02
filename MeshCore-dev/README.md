# Meshcore-Touch

MeshCore is a lightweight, portable C++ library that enables multi-hop packet routing for embedded projects using LoRa and other packet radios. It is designed for developers who want to create resilient, decentralized communication networks that work without the internet.

> [!WARNING]
> **BETA VERSION**: This project is currently in active development. Features, APIs, and UI structures are subject to change. Use with caution in critical applications.

## 💎 Featured Build: Heltec v4 "TOUCH" Edition

This is a specialized fork optimized for the Heltec v4 (ESP32-S3) with a 320x240 ST7789 TFT and XPT2046 touch controller. It features a custom "premium" dark-mode UI with full touch interactivity.

#### ⚡ Quick Start
You can download the pre-compiled binary here:
- **[Download heltec_v4_touch_v1.1.0.bin](./bin/heltec_v4_touch_v1.1.0.bin)** (Flash to `0x10000`)

#### 🔌 Wiring Diagram (Heltec v4 to ST7789 + XPT2046)

| Component | Function | Heltec v4 Pin |
|-----------|----------|---------------|
| **TFT**   | SCLK     | GPIO 17       |
| **TFT**   | MOSI/SDA | GPIO 33       |
| **TFT**   | CS       | GPIO 15       |
| **TFT**   | DC       | GPIO 16       |
| **TFT**   | RESET    | GPIO 18       |
| **TFT**   | LED/BL   | GPIO 21       |
| **Touch** | T_CLK    | GPIO 17       |
| **Touch** | T_DIN    | GPIO 33       |
| **Touch** | T_DO     | GPIO 42       |
| **Touch** | T_CS     | GPIO 3        |
| **Touch** | T_IRQ    | GPIO 4        |

> [!IMPORTANT]
> SPI lines (SCLK and MOSI) are shared between the TFT and Touch controller. Ensure common grounds and appropriate power (VEXT/3.3V) are connected.

## 📱 Touch UI Menu Structure

The "TOUCH" edition introduces a refined tabbed interface:

1.  **Messages**: View a history of all received messages. Tap a message to see full details and timestamps.
2.  **Nearby**: See a list of nodes heard directly on the radio. Displays node names and "age" (how long ago they were last seen).
3.  **Chat**: 
    - **Default Channel**: Automatically selects the **Public** channel when opened.
    - **Channel Selector**: Dropdown to switch between group channels or private contacts.
    - **Keyboard**: A full on-screen QWERTY keyboard for composing messages.
    - **Success Metrics**: Sent messages show "Me:x" — where 'x' is the number of times your message was heard repeated in the mesh!
4.  **Link**: Manage your connectivity (Toggle BLE, view connection status).
5.  **Radio**: Monitor raw radio activity and toggle specialized modes.
6.  **Power**: Check battery voltage and long-press to Hibernate/Shutdown.
7.  **Offline**: Indicates when the radio is disabled or disconnected.

### ✨ Advanced Display Features
- **Double Buffering**: Flicker-free rendering using internal memory buffers.
- **Quiet Notifications**: Replaced obstructive popups with subtle **Dark Green Tab Highlights** on the MSG and CHAT tabs.
- **List Scrolling**: Dedicated "Up" and "Down" touch buttons for long message/node lists.
- **Repeat Counting**: Green "Me:x" indicator for real-time mesh feedback.

## 🔍 What is MeshCore?
MeshCore provides the ability to create wireless mesh networks, similar to Meshtastic but with a focus on lightweight multi-hop packet routing for embedded projects. It balances simplicity with scalability, making it ideal for custom hardware solutions.

## ⚡ Key Features
* **Multi-Hop Packet Routing**: Forward messages across multiple nodes to extend range.
* **Decentralized**: No central server or internet required.
* **Low Power**: Optimized for battery and solar operation.
* **Hardware Support**: Heltec, RAK Wireless, LilyGo, and generic ESP32/nRF52/RP2040 boards.

## 🚀 How to Get Started
- Watch the [MeshCore Intro Video](https://www.youtube.com/watch?v=t1qne8uJBAc) by Andy Kirby.
- Flash the MeshCore firmware using the [MeshCore Flasher](https://flasher.meshcore.co.uk).
- Connect via [Web App](https://app.meshcore.nz), Android, or iOS.

## 📜 License
MeshCore is released under the **MIT License**.

## Contributing
Please submit PRs using **'dev'** as the base branch! Keep code concise, embedded-focused, and avoid dynamic memory allocation after setup.

---
*Created by Quark1980 & The MeshCore Community*

