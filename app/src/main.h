#pragma once

#include <stdio.h>
#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <app_version.h>

#include "bluetooth/ble_manager.h"

#include "sd/sd.h"

void app_reset_to_uf2();
