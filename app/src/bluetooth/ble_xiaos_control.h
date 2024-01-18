#pragma once

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

#include <main.h>
#include "ble_manager.h"

#include <hal/nrf_power.h>
#include <stdlib.h>

#define unpack_uint32(b) (((b)[3] << 24) | ((b)[2] << 16) | ((b)[1] << 8) | (b)[0])
#define unpack_uint16(b) (((b)[1] << 8) | (b)[0])
#define unpack_uint8(b) (b[0])

#define XIAO_CONTROL_DATA_MAX_LEN 20
#define RESPONSE_BIT (0x80)

typedef enum
{
    XIAO_SESSION_STOP,
    XIAO_SESSION_START,
} session_event_data_t;

uint8_t ble_xiaos_control_process(const uint8_t *const data, uint16_t length);
uint32_t ble_xiaos_control_queue(uint8_t opcode, uint8_t *data, uint8_t len);
bool ble_xiaos_control_ready();
