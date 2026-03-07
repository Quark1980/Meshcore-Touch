#ifndef GRID_UI_COMMON_H
#define GRID_UI_COMMON_H

#include "lvgl.h"

// GRID Amber Theme Colors
#define GRID_COLOR_BG    lv_color_hex(0x121212) // Dark background
#define GRID_COLOR_AMBER lv_color_hex(0xFFB300) // Amber accent
#define GRID_COLOR_TEXT  lv_color_hex(0xE0E0E0) // Light grey text
#define GRID_COLOR_DIM   lv_color_hex(0x424242) // Dim grey

// Global Styles
extern lv_style_t style_grid_main;
extern lv_style_t style_grid_btn;
extern lv_style_t style_grid_label_small;
extern lv_style_t style_grid_label_mid;
extern lv_style_t style_grid_label_large;
extern lv_style_t style_grid_statusbar;
extern lv_style_t style_grid_card;

// Globaal Keyboard
extern lv_obj_t *grid_kb;
extern lv_obj_t *kb_preview_ta;

// UI Initialization
void grid_ui_init();

// UI Helpers
lv_obj_t *grid_create_btn(lv_obj_t *parent, const char *text);
lv_obj_t *grid_create_label(lv_obj_t *parent, const char *text, const lv_font_t *font);
lv_obj_t *grid_create_card(lv_obj_t *parent);
lv_obj_t *grid_create_textarea(lv_obj_t *parent, const char *placeholder);
lv_obj_t *grid_create_back_btn(lv_obj_t *parent, lv_event_cb_t cb);

#endif // GRID_UI_COMMON_H
