#include "xiao_display.h"

LOG_MODULE_REGISTER(XIAO_DISPLAY, LOG_LEVEL_DBG);

static const struct device *display_dev = DEVICE_DT_GET_OR_NULL(XIAO_DISPLAY);

void xiao_display_init(){
    if (!device_is_ready(display_dev))
    {
        LOG_ERR("Device display not ready.");
    }
}

static uint32_t count;
static char count_str[11] = {0};
static lv_obj_t *btn1_label, *btn2_label, *btn3_label, *btn4_label;
static lv_obj_t *count_label;

static void lv_btn_click_callback(lv_event_t *e)
{
	ARG_UNUSED(e);

	count = 0;
    sprintf(count_str, "%d", count / 1000U);
    lv_label_set_text(count_label, count_str);
}

int display_main(){
    xiao_display_init();

    lv_obj_t *btn1, *btn2, *btn3, *btn4;

	btn1 = lv_btn_create(lv_scr_act());
	lv_obj_align(btn1, LV_ALIGN_CENTER, -30, -30);
	lv_obj_add_event_cb(btn1, lv_btn_click_callback, LV_EVENT_CLICKED, NULL);
	btn1_label = lv_label_create(btn1);
    lv_label_set_text(btn1_label, "1");
    lv_obj_align(btn1_label, LV_ALIGN_CENTER, 0, 0);

    btn2 = lv_btn_create(lv_scr_act());
	lv_obj_align(btn2, LV_ALIGN_CENTER, -30, 30);
	lv_obj_add_event_cb(btn1, lv_btn_click_callback, LV_EVENT_CLICKED, NULL);
	btn2_label = lv_label_create(btn2);
    lv_label_set_text(btn2_label, "2");
    lv_obj_align(btn2_label, LV_ALIGN_CENTER, 0, 0);

    btn3 = lv_btn_create(lv_scr_act());
	lv_obj_align(btn3, LV_ALIGN_CENTER, 30, -30);
	lv_obj_add_event_cb(btn3, lv_btn_click_callback, LV_EVENT_CLICKED, NULL);
	btn3_label = lv_label_create(btn3);
    lv_label_set_text(btn3_label, "3");
    lv_obj_align(btn3_label, LV_ALIGN_CENTER, 0, 0);

    btn4 = lv_btn_create(lv_scr_act());
	lv_obj_align(btn4, LV_ALIGN_CENTER, 30, 30);
	lv_obj_add_event_cb(btn4, lv_btn_click_callback, LV_EVENT_CLICKED, NULL);
	btn4_label = lv_label_create(btn4);
    lv_label_set_text(btn4_label, "4");
    lv_obj_align(btn4_label, LV_ALIGN_CENTER, 0, 0);

    count_label = lv_label_create(lv_scr_act());
    lv_obj_align(count_label, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_task_handler();
    display_blanking_off(display_dev);

    while (1)
    {
        if ((count % 1000) == 0U)
        {
            sprintf(count_str, "%d", count / 1000U);
            lv_label_set_text(count_label, count_str);
        }
        lv_task_handler();
        ++count;
        k_sleep(K_MSEC(1));
    }
}

K_THREAD_DEFINE(display_thread_id, 2048, display_main, NULL, NULL, NULL, 7, 0, 0);