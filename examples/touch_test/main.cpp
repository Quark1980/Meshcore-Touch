#include "grid_chat.h"
#include "grid_launcher.h"

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <Wire.h>
#include <lvgl.h>

#define TOUCH_SDA     5
#define TOUCH_SCL     6
#define TOUCH_INT     7
#define TOUCH_RST     41
#define FT6336U_ADDR  0x38

/* LVGL Buffer Size: 1/10 screen size is a good compromise for ESP32 with PSRAM */
#define DRAW_BUF_SIZE (480 * 320 / 10 * (LV_COLOR_DEPTH / 8))

TFT_eSPI tft = TFT_eSPI();
lv_obj_t *g_touch_dot = nullptr; // global touch dot indicator

/*====================
 * LVGL DISPLAY DRIVER
 *====================*/
void my_flush_cb(lv_display_t *display, const lv_area_t *area, uint8_t *px_map) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  // Push colors to TFT
  tft.pushColors((uint16_t *)px_map, w * h, true);
  tft.endWrite();

  lv_display_flush_ready(display);
}

/*====================
 * LVGL INPUT DRIVER
 *====================*/
void my_touch_read_cb(lv_indev_t *indev, lv_indev_data_t *data) {
  static int16_t last_x = 0;
  static int16_t last_y = 0;
  bool is_pressed = false;

  // Poll I2C status register directly (more reliable than INT pin during debug)
  Wire.beginTransmission(FT6336U_ADDR);
  Wire.write(0x02); // TD_STATUS register
  if (Wire.endTransmission() == 0) {
    Wire.requestFrom((uint16_t)FT6336U_ADDR, (uint8_t)5, true);
    if (Wire.available() >= 5) {
      uint8_t touches = Wire.read() & 0x0F;
      uint8_t x_high = Wire.read();
      uint8_t x_low = Wire.read();
      uint8_t y_high = Wire.read();
      uint8_t y_low = Wire.read();

      if (touches > 0) {
        // FT6336U raw coordinates
        int16_t raw_x = ((x_high & 0x0F) << 8) | x_low;
        int16_t raw_y = ((y_high & 0x0F) << 8) | y_low;

        // Map raw coordinates to screen orientation
        // Since Rotation 0 is now correct, we use raw values or basic scaling if needed.
        last_x = raw_x;
        last_y = raw_y;

        is_pressed = true;
      }
    }
  }

  if (is_pressed) {
    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = last_x;
    data->point.y = last_y;
    if (g_touch_dot) {
      lv_obj_set_pos(g_touch_dot, last_x - 10, last_y - 10);
    }
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

/*====================
 * LVGL TICK TASK
 *====================*/
uint32_t ms_tick = 0;
void lvgl_tick_task(void *arg) {
  while (1) {
    uint32_t tick = millis() - ms_tick;
    ms_tick = millis();
    lv_tick_inc(tick);
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

/*====================
 * MAIN SETUP
 *====================*/
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\nStarting LVGL v9 Test...");

  // Power up VExt (GPIO 36)
  pinMode(36, OUTPUT);
  digitalWrite(36, HIGH);
  delay(150);

  // Initialize Touch Hardware
  pinMode(TOUCH_RST, OUTPUT);
  digitalWrite(TOUCH_RST, LOW);
  delay(10);
  digitalWrite(TOUCH_RST, HIGH);
  delay(50);

  pinMode(TOUCH_INT, INPUT);
  Wire.begin(TOUCH_SDA, TOUCH_SCL);
  Wire.setClock(400000); // Fast I2C for touch responsive

  // PWM for Backlight
  ledcSetup(0, 5000, 8);
  ledcAttachPin(21, 0);
  ledcWrite(0, 128);
  Serial.println("Backlight ON (PWM)");

  // Initialize TFT
  tft.init();
  tft.setRotation(0); // Flip 180 from Rotation 2
  Serial.println("TFT init OK (Rotation 0)");

  // Initialize LVGL Core
  lv_init();
  Serial.println("LVGL init OK");

  // Setup LVGL Display Driver (v9)
  lv_display_t *disp = lv_display_create(tft.width(), tft.height());
  lv_display_set_flush_cb(disp, my_flush_cb);

  // Allocate draw buffers (Prefer PSRAM on ESP32-S3)
  void *buf1 = heap_caps_malloc(DRAW_BUF_SIZE, MALLOC_CAP_SPIRAM);
  void *buf2 = heap_caps_malloc(DRAW_BUF_SIZE, MALLOC_CAP_SPIRAM);

  // Fallback to internal RAM if PSRAM not available/enabled
  if (!buf1) buf1 = malloc(DRAW_BUF_SIZE);
  if (!buf2) buf2 = malloc(DRAW_BUF_SIZE);

  if (buf1) {
    lv_display_set_buffers(disp, buf1, buf2, DRAW_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
    Serial.println("Display buffers allocated (PSRAM capable)");
  } else {
    Serial.println("Error: Failed to allocate display buffers!");
  }

  // Setup LVGL Input Device (v9)
  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, my_touch_read_cb);
  Serial.println("Touch driver geregistreerd");

  // Create LVGL Tick Task on Core 1
  ms_tick = millis();
  xTaskCreatePinnedToCore(lvgl_tick_task, "lvgl_tick", 4096, NULL, 1, NULL, 1);

  // Initialize GRID Launcher
  launcher_init();
  Serial.println("GRID Launcher geladen");
}

void loop() {
  // Let LVGL process UI updates
  lv_timer_handler();
  launcher_update();
  delay(5);
}
