#ifndef GRID_LAUNCHER_H
#define GRID_LAUNCHER_H

#include "grid_ui_common.h"

// Grid App Structure
struct GridApp {
  const char *name;
  const char *icon_text;
  lv_obj_t *screen;
  void (*init)(struct GridApp *app);
  void (*update)(struct GridApp *app);
  void (*unload)(struct GridApp *app);
};

// UI Components
extern lv_obj_t *launcher_screen;
extern lv_obj_t *statusbar;

// Initialization
void launcher_init();
void launcher_update();

// App Switching
void switch_app(GridApp *app);
void back_to_launcher();

// Status Bar Updates (Dummy for now)
void statusbar_update(const char *name, int nodes, int batt, const char *time);
void launcher_set_chat_badge(int count);

#endif // GRID_LAUNCHER_H
