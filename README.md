# Meshcore-Touch

![MeshCore Touch](logo/touch.png)

MeshCore is a lightweight, portable C++ library that enables multi-hop packet routing for embedded projects. This specialized **"TOUCH" Edition** is optimized for the Heltec v4 (ESP32-S3), featuring a premium dark-mode UI and full touch interactivity.

> [!WARNING]
> **BETA VERSION**: This project is in active development. Features and UI are subject to change.

## 💎 Featured Build: Heltec v4 "TOUCH"

Optimized for the ESP32-S3 with a 320x240 ST7789 TFT and XPT2046 touch controller.

### ⚡ HOWTO: Quick Start (Flashing)
If you just want to get up and running quickly:
1. **Download**: [heltec_v4_tft_touch_v1.2.3_repeats_ble.bin](./bin/heltec_v4_tft_touch_v1.2.3_repeats_ble.bin) (Latest: v1.2.3)
   - Older: [heltec_v4_meshcore_touch.bin](./bin/heltec_v4_meshcore_touch.bin)
2. **Flash**: Use the [MeshCore Flasher](https://flasher.meshcore.co.uk) (select Custom File) or use `esptool`:
   ```bash
   esptool.py --chip esp32s3 write_flash 0x10000 bin/heltec_v4_tft_touch_v1.2.3_repeats_ble.bin
   ```
3. **Enjoy**: The device will boot into the premium touch interface!

### 🛠️ HOWTO: Building from Source
For developers who want to customize the project:
1. **Prerequisites**: Install [Visual Studio Code](https://code.visualstudio.com/) and the [PlatformIO IDE](https://platformio.org/) extension.
2. **Clone**: 
   ```bash
   git clone https://github.com/Quark1980/Meshcore-Touch.git
   ```
3. **Build**:
   ```bash
   pio run -e heltec_v4_tft_touch_companion_radio_ble
   ```
4. **Upload**:
   ```bash
   pio run -e heltec_v4_tft_touch_companion_radio_ble --target upload
   ```

#### 🔌 Wiring Diagram (Heltec v4 to ST7789 + XPT2046)

| Component | Function | Heltec v4 Pin |
|-----------|----------|---------------|
| **TFT**   | SCLK     | GPIO 17       |
| **TFT**   | MOSI/SDA | GPIO 33       |
| **TFT**   | CS       | GPIO 15       |
| **TFT**   | DC       | GPIO 16       |
| **TFT**   | RESET    | GPIO 18       |
| **Touch** | T_CLK    | GPIO 17       |
| **Touch** | T_DIN    | GPIO 33       |
| **Touch** | T_DO     | GPIO 42       |
| **Touch** | T_CS     | GPIO 3        |
| **Touch** | T_IRQ    | GPIO 4        |

### 🔧 Physical Connection Guide
To get your Heltec V4 working with a 2.4" or 2.8" SPI TFT (ST7789) and Touch (XPT2046):

1. **Shared SPI Bus**: Note that `SCLK` (GPIO 17) and `MOSI` (GPIO 33) are shared between both the display and the touch controller. You must wire both components to these same physical pins on the Heltec.
2. **Dedicated Chip Selects**: Each component has its own `CS` pin (`GPIO 15` for TFT, `GPIO 3` for Touch) to allow the ESP32-S3 to talk to them individually.
3. **Power (3.3V)**: Connect the `VCC` of both the TFT and Touch to the Heltec's `3.3V` or `VEXT` pin. If using `VEXT`, ensure your code or configuration enables the external power rail.
4. **Backlight**: The code expects the backlight to be hardwired or managed. For Heltec V4, ensure your ground connections are solid to avoid flicker.

## 📱 Premium Tactical Touch UI
The "TOUCH" edition features a custom-built, high-performance UI designed for a deep tactical aesthetic and mission-critical reliability:

- **Tactical Icon Pack**: High-resolution, custom 64x64 RGB565 icons (gears, mesh networks, compasses, radios) replacing standard flat designs.
- **Military-Industrial Aesthetic**: A cohesive color palette featuring Deep Tactical Black backgrounds, Warm Steel Grey frames, and High-Visibility Tactical Amber/Orange accents.
- **3-Icon Carousel Navigation**: Fluent, touch-friendly carousel with prominent tiles. Functions include:
  - **CLOCK**: Large, centered time display with date.
  - **CHAT**: Read and send encrypted messages, view channel history, with an unread message indicator.
  - **NODE**: Discover and track active mesh nodes dynamically with hop counts.
  - **RADIO**: Access radio settings and hardware info.
  - **CFG (Config)**: General device settings and preferences.
  - **LOG**: View system logs and debug information.
  - **BLE/LINK**: Manage Bluetooth connections and PIN pairing.
  - **PWR**: Power menu and calibration.
- **Ergonomic Touch Targets**: Enlarged Back buttons and intuitive overlays for easy finger tapping with gloves or in the field.
- **Tactical Status Bar**: Real-time counters for unread messages (✉) and discovered nodes (⛫), plus accurate battery voltage intel.
- **Full QWERTY Keyboard**: Compose messages directly on-screen with quick-toggle Shift/Number pads.
- **Repeats Heard Tracking (NEW)**: Real-time counter (`Me:X`) showing mesh propagation status for sent messages.
- **BLE Interception (NEW)**: Device chat history stay in sync with activity from external BLE apps (like MeshCore Mobile).
- **Power Efficiency**: Intelligent backlight management, splash screen auto-dismiss, and optimized redraws.

---
> [!NOTE]
> **Experimental Features**: The 'Repeats Heard' and updated BLE logic are currently in testing. While highly functional, please report any edge cases in our community.

---
*Created by Quark1980 & The MeshCore Community*

