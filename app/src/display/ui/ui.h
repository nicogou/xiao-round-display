#ifndef EEZ_LVGL_UI_GUI_H
#define EEZ_LVGL_UI_GUI_H

#include <lvgl.h>
#include "screens.h"

void ui_init();
void ui_tick();

void loadScreen(enum ScreensEnum screenId);

#endif // EEZ_LVGL_UI_GUI_H
