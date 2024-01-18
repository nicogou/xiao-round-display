#ifndef BLE_MANAGER_H_
#define BLE_MANAGER_H_

#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <soc.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>

#include <zephyr/logging/log.h>

#include <xiao_service.h>
// #include <include/ble_xiaos_control.h>

#define STACKSIZE 512 // CONFIG_BT_NUS_THREAD_STACK_SIZE
#define PRIORITY 7

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

typedef enum
{
    BLE_DISCONNECTED,
    BLE_CONNECTED,
} ble_event_data_t;

uint8_t ble_manager_init();
uint32_t ble_manager_start_advertising();
uint32_t ble_manager_stop_advertising();
bool ble_manager_is_connected();
bool ble_manager_control_is_subscribed();

#endif /* BLE_MANAGER_H_ */
