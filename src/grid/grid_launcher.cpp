#include "grid_launcher.h"

#include <Arduino.h>
#include <stdio.h>

lv_obj_t *launcher_screen = NULL;
lv_obj_t *statusbar = NULL;
lv_obj_t *current_app_screen = NULL;
GridApp *current_app = NULL;

static lv_obj_t *status_label_left;
static lv_obj_t *status_label_right;
static lv_obj_t *chat_badge = NULL;
static lv_obj_t *global_back_btn = NULL;

static void global_back_btn_event_cb(lv_event_t *e) {
  back_to_launcher();
}

// Dummy App Functions
static void dummy_app_init(GridApp *app) {
  app->screen = lv_obj_create(NULL);
  lv_obj_add_style(app->screen, &style_grid_main, 0);

  char buf[64];
  snprintf(buf, sizeof(buf), "%s Screen", app->name);
  lv_obj_t *lbl = grid_create_label(app->screen, buf, &lv_font_montserrat_28);
  lv_obj_center(lbl);
}

static void dummy_app_unload(GridApp *app) {
  if (app->screen) {
    lv_obj_del(app->screen);
    app->screen = NULL;
  }
}

// Forward declaration for Chat
void chat_init(GridApp *app);
void chat_update(GridApp *app);
void chat_unload(GridApp *app);

// Define the 8 Apps
static GridApp apps[] = { { "Chat", "💬", NULL, chat_init, chat_update, chat_unload },
                          { "Contacts", "👥", NULL, dummy_app_init, NULL, dummy_app_unload },
                          { "Radar", "📍", NULL, dummy_app_init, NULL, dummy_app_unload },
                          { "Advertise", "📡", NULL, dummy_app_init, NULL, dummy_app_unload },
                          { "Settings", "⚙️", NULL, dummy_app_init, NULL, dummy_app_unload },
                          { "Tools", "🔧", NULL, dummy_app_init, NULL, dummy_app_unload },
                          { "Telemetry", "📊", NULL, dummy_app_init, NULL, dummy_app_unload },
                          { "Log", "📜", NULL, dummy_app_init, NULL, dummy_app_unload } };

static void statusbar_create() {
  statusbar = lv_obj_create(lv_layer_top()); // Use top layer to stay visible
  lv_obj_set_size(statusbar, LV_PCT(100), 30);
  lv_obj_add_style(statusbar, &style_grid_statusbar, 0);
  lv_obj_align(statusbar, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_scrollbar_mode(statusbar, LV_SCROLLBAR_MODE_OFF);

  status_label_left = lv_label_create(statusbar);
  lv_obj_align(status_label_left, LV_ALIGN_LEFT_MID, 10, 0);

  status_label_right = lv_label_create(statusbar);
  lv_obj_align(status_label_right, LV_ALIGN_RIGHT_MID, -10, 0);

  statusbar_update("MESH-NODE", 4, 85, "20:50");
}

void statusbar_update(const char *name, int nodes, int batt, const char *time) {
  if (!status_label_left || !status_label_right) return;

  char left[64];
  snprintf(left, sizeof(left), "%s | %d Nodes", name, nodes);
  lv_label_set_text(status_label_left, left);

  char right[64];
  snprintf(right, sizeof(right), "%d%% | %s", batt, time);
  lv_label_set_text(status_label_right, right);
}

void launcher_set_chat_badge(int count) {
  if (!chat_badge) return;
  if (count > 0) {
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", count);
    lv_label_set_text(chat_badge, buf);
    lv_obj_remove_flag(chat_badge, LV_OBJ_FLAG_HIDDEN);
  } else {
    lv_obj_add_flag(chat_badge, LV_OBJ_FLAG_HIDDEN);
  }
}

static void app_btn_event_cb(lv_event_t *e) {
  GridApp *app = (GridApp *)lv_event_get_user_data(e);
  switch_app(app);
}

void launcher_init() {
  grid_ui_init();
  statusbar_create();

  // Create Global Back Button on Top Layer
  global_back_btn = grid_create_back_btn(lv_layer_top(), [](lv_event_t *e) { back_to_launcher(); });
  lv_obj_add_flag(global_back_btn, LV_OBJ_FLAG_HIDDEN); // Hidden on Launcher

  launcher_screen = lv_obj_create(NULL);
  lv_obj_add_style(launcher_screen, &style_grid_main, 0);

  // Grid Container
  lv_obj_t *cont = lv_obj_create(launcher_screen);
  lv_obj_set_size(cont, LV_PCT(100), LV_PCT(90));
  lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_style_bg_opa(cont, 0, 0);
  lv_obj_set_style_border_width(cont, 0, 0);
  lv_obj_set_style_pad_all(cont, 10, 0);

  // Grid Layout: 2 columns, 4 rows
  static int32_t col_dsc[] = { 140, 140, LV_GRID_TEMPLATE_LAST };
  static int32_t row_dsc[] = { 100, 100, 100, 100, LV_GRID_TEMPLATE_LAST };
  lv_obj_set_layout(cont, LV_LAYOUT_GRID);
  lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
  lv_obj_set_grid_align(cont, LV_GRID_ALIGN_CENTER, LV_GRID_ALIGN_CENTER);

  for (int i = 0; i < 8; i++) {
    lv_obj_t *btn = lv_button_create(cont);
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, i % 2, 1, LV_GRID_ALIGN_STRETCH, i / 2, 1);
    lv_obj_add_style(btn, &style_grid_btn, 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_20, 0); // Subtle background

    lv_obj_t *icon = lv_label_create(btn);
    lv_label_set_text(icon, apps[i].icon_text);
    lv_obj_set_style_text_font(icon, &lv_font_montserrat_28, 0);
    lv_obj_align(icon, LV_ALIGN_CENTER, 0, -12);

    lv_obj_t *name = lv_label_create(btn);
    lv_label_set_text(name, apps[i].name);
    lv_obj_set_style_text_font(name, &lv_font_montserrat_14, 0);
    lv_obj_align(name, LV_ALIGN_CENTER, 0, 18);

    // Chat Badge (only for index 0)
    if (i == 0) {
      chat_badge = lv_label_create(btn);
      lv_obj_set_style_bg_color(chat_badge, lv_color_hex(0xFF0000), 0);
      lv_obj_set_style_bg_opa(chat_badge, LV_OPA_COVER, 0);
      lv_obj_set_style_radius(chat_badge, LV_RADIUS_CIRCLE, 0);
      lv_obj_set_style_pad_all(chat_badge, 4, 0);
      lv_obj_set_style_text_font(chat_badge, &lv_font_montserrat_14, 0);
      lv_obj_set_style_text_color(chat_badge, lv_color_white(), 0);
      lv_obj_align(chat_badge, LV_ALIGN_TOP_RIGHT, -5, 5);
      lv_obj_add_flag(chat_badge, LV_OBJ_FLAG_HIDDEN);
    }

    lv_obj_add_event_cb(btn, app_btn_event_cb, LV_EVENT_CLICKED, &apps[i]);
  }

  // Create global back button
  global_back_btn = lv_button_create(lv_layer_top());
  lv_obj_set_size(global_back_btn, 80, 30);
  lv_obj_align(global_back_btn, LV_ALIGN_TOP_LEFT, 5, 35); // Below statusbar
  lv_obj_add_style(global_back_btn, &style_grid_btn, 0);
  lv_obj_set_style_bg_opa(global_back_btn, LV_OPA_50, 0);

  lv_obj_t *back_label = lv_label_create(global_back_btn);
  lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");
  lv_obj_set_style_text_font(back_label, &lv_font_montserrat_14, 0);
  lv_obj_center(back_label);
  lv_obj_add_event_cb(global_back_btn, global_back_btn_event_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_flag(global_back_btn, LV_OBJ_FLAG_HIDDEN); // Hidden by default

  Serial.println("Launcher geladen (2x4 Grid)");
  lv_scr_load(launcher_screen);
}

void launcher_update() {
  // 1. Update statusbar (dummy nodes count and time)
  static uint32_t last_time_update = 0;
  if (millis() - last_time_update > 5000) {
    last_time_update = millis();
    // Placeholder for real time/node count
    statusbar_update("MESH-NODE", 4, 85, "21:05");
  }

  // 2. Update current app
  if (current_app && current_app->update) {
    current_app->update(current_app);
  }
}

void switch_app(GridApp *app) {
  if (app->init) {
    Serial.printf("App switched to %s\n", app->name);
    app->init(app);
    lv_scr_load(app->screen);
    current_app_screen = app->screen;
    current_app = app;

    if (global_back_btn) lv_obj_remove_flag(global_back_btn, LV_OBJ_FLAG_HIDDEN);
  }
}

void back_to_launcher() {
  if (current_app && current_app->unload) {
    current_app->unload(current_app);
  }
  lv_scr_load(launcher_screen);
  current_app_screen = NULL;
  current_app = NULL;

  if (global_back_btn) lv_obj_add_flag(global_back_btn, LV_OBJ_FLAG_HIDDEN);
}
