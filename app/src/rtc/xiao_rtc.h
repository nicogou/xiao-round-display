#pragma once

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/rtc.h>

#define XIAO_RTC DT_ALIAS(rtc)

void xiao_rtc_init();
int xiao_rtc_set(struct rtc_time *t);
int xiao_rtc_get(struct rtc_time *t);
