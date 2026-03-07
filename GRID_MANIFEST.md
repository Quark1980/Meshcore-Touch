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
- **Files**:
    - `src/grid/grid_ui_common`: Styles, helper widgets, and central Keyboard.
    - `src/grid/grid_launcher`: Status bar and 2x4 icon grid.
    - `src/grid/grid_chat`: Core chat application.
- **App System**: `GridApp` struct with `init`, `update`, and `unload` callbacks.

## Key Configurations (`lv_conf.h`)
- `LV_FONT_MONTSERRAT_14`, `16`, `20`, `28` enabled.
- `LV_USE_OS LV_OS_FREERTOS` enabled.
- PSRAM buffer allocation handled in `main.cpp`.
- Helium/ARM ASM disabled (incompatible with Xtensa).

## Git Backup
- Remote: `https://github.com/Quark1980/MeshcoreGRID.git`
