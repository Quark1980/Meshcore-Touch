#include "grid_chat.h"

#include <Arduino.h>
#include <string>
#include <vector>

static lv_obj_t *chat_cont = NULL;
static lv_obj_t *ta_input = NULL;
static lv_obj_t *tab_public = NULL;
static lv_obj_t *tab_dms = NULL;
static lv_obj_t *active_list = NULL;
static bool is_public_mode = true;

// Helper to add a message bubble
void add_message_bubble(lv_obj_t *parent, const char *sender, const char *text, const char *time,
                        bool is_me) {
  lv_obj_t *item_cont = lv_obj_create(parent);
  lv_obj_set_width(item_cont, LV_PCT(100));
  lv_obj_set_height(item_cont, LV_SIZE_CONTENT);
  lv_obj_set_style_bg_opa(item_cont, 0, 0);
  lv_obj_set_style_border_width(item_cont, 0, 0);
  lv_obj_set_style_pad_all(item_cont, 5, 0);
  lv_obj_set_flex_flow(item_cont, LV_FLEX_FLOW_COLUMN);

  // Sender Label
  lv_obj_t *info_lbl = grid_create_label(item_cont, sender, &lv_font_montserrat_14);
  lv_obj_set_style_text_color(info_lbl, GRID_COLOR_DIM, 0);

  // Bubble
  lv_obj_t *bubble = lv_obj_create(item_cont);
  lv_obj_set_width(bubble, LV_PCT(80));
  lv_obj_set_height(bubble, LV_SIZE_CONTENT);
  lv_obj_set_style_radius(bubble, 12, 0);
  lv_obj_set_style_pad_all(bubble, 10, 0);
  lv_obj_set_style_border_width(bubble, 0, 0);

  if (is_me) {
    lv_obj_set_align(item_cont, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_style_align(info_lbl, LV_ALIGN_TOP_RIGHT, 0);
    lv_obj_set_style_align(bubble, LV_ALIGN_TOP_RIGHT, 0);
    lv_obj_set_style_bg_color(bubble, GRID_COLOR_AMBER, 0);
    lv_obj_set_style_text_color(bubble, GRID_COLOR_BG, 0);
  } else {
    lv_obj_set_align(item_cont, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_align(info_lbl, LV_ALIGN_TOP_LEFT, 0);
    lv_obj_set_style_align(bubble, LV_ALIGN_TOP_LEFT, 0);
    lv_obj_set_style_bg_color(bubble, lv_color_hex(0x333333), 0);
    lv_obj_set_style_text_color(bubble, lv_color_hex(0xFFFFFF), 0);
  }

  lv_obj_t *msg_lbl = grid_create_label(bubble, text, &lv_font_montserrat_16);
  lv_label_set_long_mode(msg_lbl, LV_LABEL_LONG_WRAP);
  lv_obj_set_width(msg_lbl, LV_PCT(100));

  // Time Label (inside or below bubble)
  lv_obj_t *time_lbl = grid_create_label(bubble, time, &lv_font_montserrat_14);
  lv_obj_set_style_text_color(time_lbl, is_me ? GRID_COLOR_BG : GRID_COLOR_DIM, 0);
  lv_obj_set_style_opa(time_lbl, LV_OPA_70, 0);
  lv_obj_align(time_lbl, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
  lv_obj_set_style_pad_top(time_lbl, 5, 0);

  lv_obj_scroll_to_view(item_cont, LV_ANIM_ON);
}

static void send_msg_event_cb(lv_event_t *e) {
  const char *txt = lv_textarea_get_text(ta_input);
  if (strlen(txt) > 0) {
    add_message_bubble(active_list, "ME", txt, "21:18", true);
    lv_textarea_set_text(ta_input, "");
    // Later: MessageStore.addMessage(...)
  }
}

static void tab_event_cb(lv_event_t *e) {
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  is_public_mode = (btn == tab_public);

  // Toggle active state colors
  lv_obj_set_style_bg_color(tab_public, is_public_mode ? GRID_COLOR_AMBER : GRID_COLOR_DIM, 0);
  lv_obj_set_style_text_color(tab_public, is_public_mode ? GRID_COLOR_BG : GRID_COLOR_AMBER, 0);
  lv_obj_set_style_bg_color(tab_dms, !is_public_mode ? GRID_COLOR_AMBER : GRID_COLOR_DIM, 0);
  lv_obj_set_style_text_color(tab_dms, !is_public_mode ? GRID_COLOR_BG : GRID_COLOR_AMBER, 0);

  // Switch lists (In a real app, we'd clear and reload or swap containers)
  // For this dummy, we'll just add a divider
  lv_obj_t *sep = lv_label_create(active_list);
  lv_label_set_text(sep, is_public_mode ? "--- Switched to PUBLIC ---" : "--- Switched to DMs ---");
  lv_obj_set_style_text_color(sep, GRID_COLOR_DIM, 0);
  lv_obj_set_width(sep, LV_PCT(100));
  lv_obj_set_style_text_align(sep, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_scroll_to_view(sep, LV_ANIM_ON);
}

void chat_init(GridApp *app) {
  app->screen = lv_obj_create(NULL);
  lv_obj_add_style(app->screen, &style_grid_main, 0);

  // Header with Tabs
  lv_obj_t *header = lv_obj_create(app->screen);
  lv_obj_set_size(header, LV_HOR_RES, 45);
  lv_obj_align(header, LV_ALIGN_TOP_MID, 0, 35); // Below statusbar
  lv_obj_add_style(header, &style_grid_statusbar, 0);

  // Prevent overlap with global "BACK" button (which is 80px wide + 5px margin)
  lv_obj_set_style_pad_left(header, 85, 0);

  lv_obj_set_flex_flow(header, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(header, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_all(header, 5, 0);

  tab_public = grid_create_btn(header, "PUBLIC");
  lv_obj_set_size(tab_public, 70, 32);
  lv_obj_add_event_cb(tab_public, tab_event_cb, LV_EVENT_CLICKED, NULL);

  tab_dms = grid_create_btn(header, "DMs");
  lv_obj_set_size(tab_dms, 70, 32);
  lv_obj_add_event_cb(tab_dms, tab_event_cb, LV_EVENT_CLICKED, NULL);

  // Set initial active tab
  lv_obj_set_style_bg_color(tab_public, GRID_COLOR_AMBER, 0);
  lv_obj_set_style_text_color(tab_public, GRID_COLOR_BG, 0);

  // Main Chat Area (Scrollable container)
  active_list = lv_obj_create(app->screen);
  lv_obj_set_size(active_list, LV_HOR_RES, 260); // Adjust for input
  lv_obj_align(active_list, LV_ALIGN_TOP_MID, 0, 80);
  lv_obj_set_style_bg_opa(active_list, 0, 0);
  lv_obj_set_style_border_width(active_list, 0, 0);
  lv_obj_set_flex_flow(active_list, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_all(active_list, 10, 0);
  lv_obj_set_style_pad_gap(active_list, 10, 0);
  lv_obj_set_scrollbar_mode(active_list, LV_SCROLLBAR_MODE_AUTO);

  // Bottom Input Area
  lv_obj_t *footer = lv_obj_create(app->screen);
  lv_obj_set_size(footer, LV_HOR_RES, 70);
  lv_obj_align(footer, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_style_bg_color(footer, lv_color_hex(0x000000), 0);
  lv_obj_set_style_border_side(footer, LV_BORDER_SIDE_TOP, 0);
  lv_obj_set_style_border_color(footer, GRID_COLOR_DIM, 0);
  lv_obj_set_style_border_width(footer, 1, 0);
  lv_obj_set_style_pad_all(footer, 5, 0);

  ta_input = grid_create_textarea(footer, "Write message...");
  lv_obj_set_size(ta_input, 160, 45);
  lv_obj_align(ta_input, LV_ALIGN_LEFT_MID, 5, 0);

  lv_obj_t *btn_send = grid_create_btn(footer, LV_SYMBOL_PLAY); // Send icon
  lv_obj_set_size(btn_send, 60, 45);
  lv_obj_align(btn_send, LV_ALIGN_RIGHT_MID, -5, 0);
  lv_obj_add_event_cb(btn_send, send_msg_event_cb, LV_EVENT_CLICKED, NULL);

  // Initial dummy messages
  add_message_bubble(active_list, "MeshNode_7A", "Hello world! This is GRID Chat.", "21:00", false);
}

void chat_update(GridApp *app) {
  // Only simulate if this screen is active
  if (lv_screen_active() != app->screen) {
    // Background logic: increment notification badge occasionally
    static uint32_t last_notif = 0;
    if (millis() - last_notif > 15000) {
      last_notif = millis();
      static int count = 0;
      launcher_set_chat_badge(++count);
    }
    return;
  }

  static uint32_t last_msg = 0;
  if (millis() - last_msg > 7000) {
    last_msg = millis();
    if (is_public_mode) {
      add_message_bubble(active_list, "Radio_Bob", "Does anyone hear me on Public?", "21:19", false);
    } else {
      add_message_bubble(active_list, "Alice", "Hey, are you coming to the meet?", "21:20", false);
    }
    // Reset badge when in app
    launcher_set_chat_badge(0);
  }
}

void chat_unload(GridApp *app) {
  // Cleanup if needed
}

GridApp chat_app = { .name = "Chat",
                     // We'll leave icon blank or use a string if the struct allows, but typically it's name +
                     // screen .icon = "💬",
                     .init = chat_init,
                     .update = chat_update,
                     .unload = chat_unload };
