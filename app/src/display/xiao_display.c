#include "xiao_display.h"

LOG_MODULE_REGISTER(XIAO_DISPLAY, LOG_LEVEL_DBG);

static const struct device *display_dev = DEVICE_DT_GET_OR_NULL(XIAO_DISPLAY);

void xiao_display_init()
{
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device display not ready.");
	}
}

int display_main()
{
	xiao_display_init();

	ui_init();
	display_blanking_off(display_dev);

	while (1) {
		lv_task_handler();
		k_sleep(K_MSEC(10));
	}
}

K_THREAD_DEFINE(display_thread_id, 2048, display_main, NULL, NULL, NULL, 7, 0, 0);
