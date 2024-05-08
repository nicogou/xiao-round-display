#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
	lv_obj_t *main;
	lv_obj_t *settings;
	lv_obj_t *settings_set_date;
	lv_obj_t *settings_set_time;
	lv_obj_t *app_title;
	lv_obj_t *button_settings;
	lv_obj_t *button_start_acq;
	lv_obj_t *label_button_settings;
	lv_obj_t *label_button_start_acq;
	lv_obj_t *label_settings_title;
	lv_obj_t *label_settings_title_1;
	lv_obj_t *label_settings_title_2;
	lv_obj_t *obj0;
	lv_obj_t *obj1;
	lv_obj_t *obj2;
	lv_obj_t *obj3;
	lv_obj_t *obj4;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
	SCREEN_ID_MAIN = 1,
	SCREEN_ID_SETTINGS = 2,
	SCREEN_ID_SETTINGS_SET_DATE = 3,
	SCREEN_ID_SETTINGS_SET_TIME = 4,
};

void create_screen_main();
void tick_screen_main();

void create_screen_settings();
void tick_screen_settings();

void create_screen_settings_set_date();
void tick_screen_settings_set_date();

void create_screen_settings_set_time();
void tick_screen_settings_set_time();

void create_screens();
void tick_screen(int screen_index);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/
