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
static lv_obj_t *hello_world_label;
static lv_obj_t *count_label;

int display_main(){
    xiao_display_init();

    hello_world_label = lv_label_create(lv_scr_act());

    lv_label_set_text(hello_world_label, "Hello world!");
    lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);

    count_label = lv_label_create(lv_scr_act());
    lv_obj_align(count_label, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_task_handler();
    display_blanking_off(display_dev);

    while (1)
    {
        if ((count % 100) == 0U)
        {
            sprintf(count_str, "%d", count / 100U);
            lv_label_set_text(count_label, count_str);
        }
        lv_task_handler();
        ++count;
        k_sleep(K_MSEC(10));
    }
}

K_THREAD_DEFINE(display_thread_id, 2048, display_main, NULL, NULL, NULL, 7, 0, 0);