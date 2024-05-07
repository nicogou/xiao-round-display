#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

objects_t objects;
lv_obj_t *tick_value_change_obj;

static void event_handler_cb_main_button_start_acq(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = e->user_data;
    if (event == LV_EVENT_CLICKED) {
        flowPropagateValue(flowState, 2, 0);
    }
}

static void event_handler_cb_main_button_settings(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = e->user_data;
    if (event == LV_EVENT_CLICKED) {
        flowPropagateValue(flowState, 4, 0);
    }
}

static void event_handler_cb_settings_obj0(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = e->user_data;
    if (event == LV_EVENT_CLICKED) {
        flowPropagateValue(flowState, 2, 0);
    }
}

static void event_handler_cb_settings_obj1(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = e->user_data;
    if (event == LV_EVENT_CLICKED) {
        flowPropagateValue(flowState, 5, 0);
    }
}

static void event_handler_cb_settings_obj2(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = e->user_data;
    if (event == LV_EVENT_CLICKED) {
        flowPropagateValue(flowState, 7, 0);
    }
}

static void event_handler_cb_settings_set_date_obj3(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = e->user_data;
    if (event == LV_EVENT_CLICKED) {
        flowPropagateValue(flowState, 0, 0);
    }
}

static void event_handler_cb_settings_set_time_obj4(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = e->user_data;
    if (event == LV_EVENT_CLICKED) {
        flowPropagateValue(flowState, 0, 0);
    }
}

void create_screen_main() {
    void *flowState = getFlowState(0, 0);
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_arc_create(parent_obj);
            lv_obj_set_pos(obj, 1, 0);
            lv_obj_set_size(obj, 240, 240);
            lv_arc_set_bg_start_angle(obj, 90);
            lv_arc_set_bg_end_angle(obj, 89);
            lv_obj_set_style_arc_width(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
        }
        {
            // app_title
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.app_title = obj;
            lv_obj_set_pos(obj, 63, 39);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "IMU data logger");
        }
        {
            // button_start_acq
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.button_start_acq = obj;
            lv_obj_set_pos(obj, 70, 73);
            lv_obj_set_size(obj, 100, 28);
            lv_obj_add_event_cb(obj, event_handler_cb_main_button_start_acq, LV_EVENT_ALL, flowState);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // label_button_start_acq
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_button_start_acq = obj;
                    lv_obj_set_pos(obj, LV_PCT(28), LV_PCT(-9));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Start");
                }
            }
        }
        {
            // button_settings
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.button_settings = obj;
            lv_obj_set_pos(obj, 71, 140);
            lv_obj_set_size(obj, 100, 28);
            lv_obj_add_event_cb(obj, event_handler_cb_main_button_settings, LV_EVENT_ALL, flowState);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // label_button_settings
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_button_settings = obj;
                    lv_obj_set_pos(obj, 7, -2);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Settings");
                }
            }
        }
    }
}

void tick_screen_main() {
    void *flowState = getFlowState(0, 0);
}

void create_screen_settings() {
    void *flowState = getFlowState(0, 1);
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_arc_create(parent_obj);
            lv_obj_set_pos(obj, 1, 0);
            lv_obj_set_size(obj, 240, 240);
            lv_arc_set_bg_start_angle(obj, 90);
            lv_arc_set_bg_end_angle(obj, 89);
            lv_obj_set_style_arc_width(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
        }
        {
            // label_settings_title
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.label_settings_title = obj;
            lv_obj_set_pos(obj, 90, 38);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Settings");
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 86, 198);
            lv_obj_set_size(obj, 68, 25);
            lv_obj_add_event_cb(obj, event_handler_cb_settings_obj0, LV_EVENT_ALL, flowState);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 3, -3);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Back");
                }
            }
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 70, 70);
            lv_obj_set_size(obj, 100, 24);
            lv_obj_add_event_cb(obj, event_handler_cb_settings_obj1, LV_EVENT_ALL, flowState);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 6, -4);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Set Date");
                }
            }
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.obj2 = obj;
            lv_obj_set_pos(obj, 70, 108);
            lv_obj_set_size(obj, 100, 25);
            lv_obj_add_event_cb(obj, event_handler_cb_settings_obj2, LV_EVENT_ALL, flowState);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 5, -3);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Set Time");
                }
            }
        }
    }
}

void tick_screen_settings() {
    void *flowState = getFlowState(0, 1);
}

void create_screen_settings_set_date() {
    void *flowState = getFlowState(0, 2);
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings_set_date = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_arc_create(parent_obj);
            lv_obj_set_pos(obj, 1, 0);
            lv_obj_set_size(obj, 240, 240);
            lv_arc_set_bg_start_angle(obj, 90);
            lv_arc_set_bg_end_angle(obj, 89);
            lv_obj_set_style_arc_width(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.obj3 = obj;
            lv_obj_set_pos(obj, 86, 198);
            lv_obj_set_size(obj, 68, 25);
            lv_obj_add_event_cb(obj, event_handler_cb_settings_set_date_obj3, LV_EVENT_ALL, flowState);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 3, -3);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Back");
                }
            }
        }
        {
            // label_settings_title_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.label_settings_title_1 = obj;
            lv_obj_set_pos(obj, 89, 38);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Set Date");
        }
    }
}

void tick_screen_settings_set_date() {
    void *flowState = getFlowState(0, 2);
}

void create_screen_settings_set_time() {
    void *flowState = getFlowState(0, 3);
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings_set_time = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_arc_create(parent_obj);
            lv_obj_set_pos(obj, 1, 0);
            lv_obj_set_size(obj, 240, 240);
            lv_arc_set_bg_start_angle(obj, 90);
            lv_arc_set_bg_end_angle(obj, 89);
            lv_obj_set_style_arc_width(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.obj4 = obj;
            lv_obj_set_pos(obj, 86, 198);
            lv_obj_set_size(obj, 68, 25);
            lv_obj_add_event_cb(obj, event_handler_cb_settings_set_time_obj4, LV_EVENT_ALL, flowState);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 3, -3);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Back");
                }
            }
        }
        {
            // label_settings_title_2
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.label_settings_title_2 = obj;
            lv_obj_set_pos(obj, 88, 38);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Set Time");
        }
    }
}

void tick_screen_settings_set_time() {
    void *flowState = getFlowState(0, 3);
}


void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
    create_screen_settings();
    create_screen_settings_set_date();
    create_screen_settings_set_time();
}

typedef void (*tick_screen_func_t)();

tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_settings,
    tick_screen_settings_set_date,
    tick_screen_settings_set_time,
};

void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
