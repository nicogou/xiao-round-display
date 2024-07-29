#include "xiao_rtc.h"

LOG_MODULE_REGISTER(RTC, LOG_LEVEL_DBG);

static const struct device *rtc_dev = DEVICE_DT_GET(XIAO_RTC);

int xiao_rtc_get(struct rtc_time *t)
{
	int err;
	err = rtc_get_time(rtc_dev, t);
	if (err) {
		LOG_ERR("Error getting time from rtc (%i)", err);
	}
	return 0;
}

int xiao_rtc_set(struct rtc_time *t)
{
	int err;
	err = rtc_set_time(rtc_dev, t);
	if (err) {
		LOG_ERR("Error setting time from rtc (%i)", err);
	}
	return 0;
}

void xiao_rtc_init()
{
	if (!device_is_ready(rtc_dev)) {
		LOG_ERR("RTC device not ready.");
	}
}
