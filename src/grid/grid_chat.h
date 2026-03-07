#ifndef GRID_CHAT_H
#define GRID_CHAT_H

#include "grid_launcher.h"

// Chat App Instance
extern GridApp chat_app;

// Functions
void chat_init(GridApp *app);
void chat_update(GridApp *app);
void chat_unload(GridApp *app);

#endif // GRID_CHAT_H
