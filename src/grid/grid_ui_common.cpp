#include "grid_ui_common.h"

lv_style_t style_grid_main;
lv_style_t style_grid_btn;
lv_style_t style_grid_label_small;
lv_style_t style_grid_label_mid;
lv_style_t style_grid_label_large;
lv_style_t style_grid_statusbar;
lv_style_t style_grid_card;

lv_obj_t *grid_kb = NULL;
lv_obj_t *kb_preview_ta = NULL;
static bool is_syncing = false;

static void kb_preview_ta_event_cb(lv_event_t *e) {
  if (is_syncing) return;
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_VALUE_CHANGED) {
    lv_obj_t *ta = lv_keyboard_get_textarea(grid_kb);
    if (ta) {
      is_syncing = true;
      lv_textarea_set_text(ta, lv_textarea_get_text(kb_preview_ta));
      is_syncing = false;
    }
  }
}

static void ta_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = (lv_obj_t *)lv_event_get_target(e);
  if (code == LV_EVENT_FOCUSED) {
    if (grid_kb) {
      lv_keyboard_set_textarea(grid_kb, ta);
      lv_obj_remove_flag(grid_kb, LV_OBJ_FLAG_HIDDEN);
      // Sync preview
      if (kb_preview_ta) {
        lv_obj_remove_flag(kb_preview_ta, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(kb_preview_ta);
        is_syncing = true;
        lv_textarea_set_text(kb_preview_ta, lv_textarea_get_text(ta));
        is_syncing = false;
        lv_obj_update_layout(kb_preview_ta);
      }
    }
  } else if (code == LV_EVENT_VALUE_CHANGED) {
    if (is_syncing) return;
    if (kb_preview_ta && lv_keyboard_get_textarea(grid_kb) == ta) {
      is_syncing = true;
      lv_textarea_set_text(kb_preview_ta, lv_textarea_get_text(ta));
      is_syncing = false;
    }
  } else if (code == LV_EVENT_DEFOCUSED) {
    if (grid_kb) {
      lv_keyboard_set_textarea(grid_kb, NULL);
      lv_obj_add_flag(grid_kb, LV_OBJ_FLAG_HIDDEN);
      if (kb_preview_ta) {
        lv_obj_add_flag(kb_preview_ta, LV_OBJ_FLAG_HIDDEN); // Hide preview TA
      }
    }
  }
}

void grid_ui_init() {
  if (grid_kb) return; // Already initialized

  // Main Background Style
  lv_style_init(&style_grid_main);
  lv_style_set_bg_color(&style_grid_main, GRID_COLOR_BG);
  lv_style_set_text_color(&style_grid_main, GRID_COLOR_TEXT);

  // Button Style
  lv_style_init(&style_grid_btn);
  lv_style_set_bg_color(&style_grid_btn, GRID_COLOR_DIM);
  lv_style_set_border_color(&style_grid_btn, GRID_COLOR_AMBER);
  lv_style_set_border_width(&style_grid_btn, 2);
  lv_style_set_radius(&style_grid_btn, 8);
  lv_style_set_text_color(&style_grid_btn, GRID_COLOR_AMBER);

  // Card Style
  lv_style_init(&style_grid_card);
  lv_style_set_bg_color(&style_grid_card, lv_color_hex(0x1E1E1E));
  lv_style_set_border_color(&style_grid_card, GRID_COLOR_DIM);
  lv_style_set_border_width(&style_grid_card, 1);
  lv_style_set_radius(&style_grid_card, 12);

  // Statusbar Style
  lv_style_init(&style_grid_statusbar);
  lv_style_set_bg_color(&style_grid_statusbar, lv_color_hex(0x000000));
  lv_style_set_bg_opa(&style_grid_statusbar, LV_OPA_COVER);
  lv_style_set_border_side(&style_grid_statusbar, LV_BORDER_SIDE_BOTTOM);
  lv_style_set_border_color(&style_grid_statusbar, GRID_COLOR_DIM);
  lv_style_set_border_width(&style_grid_statusbar, 1);
  lv_style_set_text_color(&style_grid_statusbar, GRID_COLOR_AMBER);
  lv_style_set_text_font(&style_grid_statusbar, &lv_font_montserrat_14);

  // Label Styles
  lv_style_init(&style_grid_label_small);
  lv_style_set_text_font(&style_grid_label_small, &lv_font_montserrat_14);

  lv_style_init(&style_grid_label_mid);
  lv_style_set_text_font(&style_grid_label_mid, &lv_font_montserrat_20);

  lv_style_init(&style_grid_label_large);
  lv_style_set_text_font(&style_grid_label_large, &lv_font_montserrat_28);

  // Central Keyboard on Top Layer
  grid_kb = lv_keyboard_create(lv_layer_top());
  lv_obj_add_flag(grid_kb, LV_OBJ_FLAG_HIDDEN);
  lv_obj_set_style_bg_color(grid_kb, GRID_COLOR_BG, 0);
  lv_obj_set_style_text_color(grid_kb, GRID_COLOR_AMBER, 0);
  lv_obj_set_size(grid_kb, LV_HOR_RES, 200); // Fixed height for predictability
  lv_obj_align(grid_kb, LV_ALIGN_BOTTOM_MID, 0, 0);

  // Floating Keyboard Preview field (also on Top Layer to avoid clipping)
  kb_preview_ta = lv_textarea_create(lv_layer_top());
  lv_obj_add_flag(kb_preview_ta, LV_OBJ_FLAG_HIDDEN);
  lv_obj_set_size(kb_preview_ta, LV_PCT(95), 45);
  // Position it floating just above the keyboard height (200)
  lv_obj_align(kb_preview_ta, LV_ALIGN_BOTTOM_MID, 0, -205);

  lv_obj_set_style_bg_color(kb_preview_ta, lv_color_hex(0x2A2A2A), 0);
  lv_obj_set_style_bg_opa(kb_preview_ta, LV_OPA_90, 0);
  lv_obj_set_style_text_color(kb_preview_ta, GRID_COLOR_AMBER, 0);
  lv_obj_set_style_border_color(kb_preview_ta, GRID_COLOR_AMBER, 0);
  lv_obj_set_style_border_width(kb_preview_ta, 1, 0);
  lv_obj_set_style_radius(kb_preview_ta, 5, 0);
  lv_textarea_set_one_line(kb_preview_ta, true);
  lv_obj_add_event_cb(kb_preview_ta, kb_preview_ta_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

lv_obj_t *grid_create_btn(lv_obj_t *parent, const char *text) {
  lv_obj_t *btn = lv_button_create(parent);
  lv_obj_add_style(btn, &style_grid_btn, 0);

  lv_obj_t *label = lv_label_create(btn);
  lv_label_set_text(label, text);
  lv_obj_center(label);

  return btn;
}

lv_obj_t *grid_create_label(lv_obj_t *parent, const char *text, const lv_font_t *font) {
  lv_obj_t *label = lv_label_create(parent);
  lv_label_set_text(label, text);
  if (font) {
    lv_obj_set_style_text_font(label, font, 0);
  }
  return label;
}

lv_obj_t *grid_create_card(lv_obj_t *parent) {
  lv_obj_t *card = lv_obj_create(parent);
  lv_obj_add_style(card, &style_grid_card, 0);
  lv_obj_set_scrollbar_mode(card, LV_SCROLLBAR_MODE_AUTO);
  return card;
}

lv_obj_t *grid_create_textarea(lv_obj_t *parent, const char *placeholder) {
  lv_obj_t *ta = lv_textarea_create(parent);
  lv_textarea_set_placeholder_text(ta, placeholder);
  lv_obj_set_style_bg_color(ta, lv_color_hex(0x2A2A2A), 0);
  lv_obj_set_style_text_color(ta, GRID_COLOR_TEXT, 0);
  lv_obj_set_style_border_color(ta, GRID_COLOR_DIM, 0);
  lv_obj_add_event_cb(ta, ta_event_cb, LV_EVENT_ALL, NULL);
  return ta;
}

lv_obj_t *grid_create_back_btn(lv_obj_t *parent, lv_event_cb_t cb) {
  lv_obj_t *btn = grid_create_btn(parent, "BACK");
  lv_obj_set_size(btn, 70, 35);
  lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 5, 35); // Just below status bar
  lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, NULL);
  return btn;
}
