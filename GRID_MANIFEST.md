# GRID OS Manifest

Essential settings and information for the MeshCore-Touch project.

## Hardware: Heltec V4 (ESP32-S3)
- **Display**: ST7796S (3.5" or similar, SPI)
    - **Pins**: SCK: 9, MOSI: 10, MISO: 11, CS: 3, DC: 4, RST: 5, BL: 6
    - **Resolution**: 320x480
    - **Driver**: TFT_eSPI (configured for `Rotation 0`)
- **Touch**: FT6336U (I2C)
    - **Pins**: SDA: 41, SCL: 42, INT: 7, RST: 1
    - **Mapping**: Direct raw mapping to 320x480 (No inversion needed in `Rotation 0`).

## Software Stack
- **Framework**: Arduino / PlatformIO
- **UI Library**: LVGL v9.5.0
- **Base**: MeshCore v1.14.0

## UI Architecture
- **Theme**: Amber/Dark (#FFB300 on #121212).
- **Navigation**:
    - **Global Back Button**: Persistent `lv_button` on `lv_layer_top()` for universal navigation.
    - **Status Bar**: Persistent `lv_obj` on `lv_layer_top()` for system metrics.
- **Applications**:
    - `src/grid/grid_chat`: **Professional Chat App** featuring:
        - Message Bubbles (Left/Right alignment).
        - Sender Metadata (Name + Type).
        - PUBLIC vs DMs Tab Selector.
        - Auto-scroll and Unread Notification Badges.
    - `src/grid/grid_ui_common`: Centralized styles and **Float-Protected Keyboard** (prevents circular event crashes).
    - `src/grid/grid_launcher`: Multi-app grid and lifecycle management (`switch_app`).

## Key Configurations (`lv_conf.h`)
- `LV_FONT_MONTSERRAT_14`, `16`, `20`, `28` enabled.
- `LV_USE_OS LV_OS_FREERTOS` enabled.
- PSRAM buffer allocation handled in `main.cpp`.
- Helium/ARM ASM disabled (incompatible with Xtensa).

## Progress Log (Current)
- [x] Boot stable with TFT_eSPI & FT6336U.
- [x] Fixed circular sync crash in keyboard preview.
- [x] Implemented global nav logic on top layer.
- [x] Implemented professional bubble chat with tab selection.

## Git Backup
- Remote: `https://github.com/Quark1980/MeshcoreGRID.git`
