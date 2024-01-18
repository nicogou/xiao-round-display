#include "ble_manager.h"

LOG_MODULE_REGISTER(BLE_MANAGER, LOG_LEVEL_DBG);

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
    BT_DATA_BYTES(BT_DATA_UUID16_ALL,
                  BT_UUID_16_ENCODE(BT_UUID_BAS_VAL)),
};

static const struct bt_data sd[] = {
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_XIAO_SERVICE_VAL),
};

static K_SEM_DEFINE(ble_init_ok, 0, 1);

static struct bt_conn *current_conn;
static struct bt_conn *auth_conn;

static void mtu_exchange_cb(struct bt_conn *conn, uint8_t err,
                            struct bt_gatt_exchange_params *params)
{
    LOG_INF("%s: MTU exchange %s (%u)", __func__,
            err == 0U ? "successful" : "failed",
            bt_gatt_get_mtu(conn));
}

static struct bt_gatt_exchange_params mtu_exchange_params = {
    .func = mtu_exchange_cb};

static void connected(struct bt_conn *conn, uint8_t err)
{
    char addr[BT_ADDR_LE_STR_LEN];

    if (err)
    {
        LOG_ERR("Connection failed (err %u)", err);
        return;
    }

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
    LOG_INF("Connected %s", addr);

    current_conn = bt_conn_ref(conn);

    int ret = bt_gatt_exchange_mtu(conn, &mtu_exchange_params);
    if (ret)
    {
        LOG_ERR("%s: MTU exchange failed (err %d)", __func__, err);
    }
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    LOG_INF("Disconnected: %s (reason %u)", addr, reason);

    if (auth_conn)
    {
        bt_conn_unref(auth_conn);
        auth_conn = NULL;
    }

    if (current_conn)
    {
        bt_conn_unref(current_conn);
        current_conn = NULL;
    }
}

#ifdef CONFIG_XIAO_SERVICE_SECURITY_ENABLED
static void security_changed(struct bt_conn *conn, bt_security_t level,
                             enum bt_security_err err)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    if (!err)
    {
        LOG_DBG("Security changed: %s level %u", addr, level);
    }
    else
    {
        LOG_ERR("Security failed: %s level %u err %d", addr,
                level, err);
    }
}
#endif

BT_CONN_CB_DEFINE(conn_callbacks) = {
    .connected = connected,
    .disconnected = disconnected,
#ifdef CONFIG_XIAO_SERVICE_SECURITY_ENABLED
    .security_changed = security_changed,
#endif
};

#if defined(CONFIG_XIAO_SERVICE_SECURITY_ENABLED)
static void auth_passkey_display(struct bt_conn *conn, unsigned int passkey)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    LOG_DBG("Passkey for %s: %06u", addr, passkey);
}

static void auth_passkey_confirm(struct bt_conn *conn, unsigned int passkey)
{
    char addr[BT_ADDR_LE_STR_LEN];

    auth_conn = bt_conn_ref(conn);

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    LOG_DBG("Passkey for %s: %06u", addr, passkey);
    LOG_DBG("Press Button 1 to confirm, Button 2 to reject.");
}

static void auth_cancel(struct bt_conn *conn)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    LOG_DBG("Pairing cancelled: %s", addr);
}

static void pairing_complete(struct bt_conn *conn, bool bonded)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    LOG_DBG("Pairing completed: %s, bonded: %d", addr, bonded);
}

static void pairing_failed(struct bt_conn *conn, enum bt_security_err reason)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    LOG_DBG("Pairing failed conn: %s, reason %d", addr, reason);
}

static struct bt_conn_auth_cb conn_auth_callbacks = {
    .passkey_display = auth_passkey_display,
    .passkey_confirm = auth_passkey_confirm,
    .cancel = auth_cancel,
};

static struct bt_conn_auth_info_cb conn_auth_info_callbacks = {
    .pairing_complete = pairing_complete,
    .pairing_failed = pairing_failed};
#else
static struct bt_conn_auth_cb conn_auth_callbacks;
static struct bt_conn_auth_info_cb conn_auth_info_callbacks;
#endif

static void bt_receive_cb(struct bt_conn *conn, const uint8_t *const data,
                          uint16_t len)
{
    char addr[BT_ADDR_LE_STR_LEN] = {0};

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, ARRAY_SIZE(addr));

    ble_xiaos_control_process(data, len);
}

static void bt_subscribe_cb(enum xiao_service_send_status status)
{
    return;
}

static struct xiao_service_cb xiao_cb = {
    .control_received = bt_receive_cb,
    .control_send_enabled = bt_subscribe_cb,
};

#ifdef CONFIG_XIAO_SERVICE_SECURITY_ENABLED
static void num_comp_reply(bool accept)
{
    if (accept)
    {
        bt_conn_auth_passkey_confirm(auth_conn);
        LOG_DBG("Numeric Match, conn %p", (void *)auth_conn);
    }
    else
    {
        bt_conn_auth_cancel(auth_conn);
        LOG_DBG("Numeric Reject, conn %p", (void *)auth_conn);
    }

    bt_conn_unref(auth_conn);
    auth_conn = NULL;
}

void button_changed(uint32_t button_state, uint32_t has_changed)
{
    uint32_t buttons = button_state & has_changed;

    if (auth_conn)
    {
        if (buttons & KEY_PASSKEY_ACCEPT)
        {
            num_comp_reply(true);
        }

        if (buttons & KEY_PASSKEY_REJECT)
        {
            num_comp_reply(false);
        }
    }
}
#endif /* CONFIG_XIAO_SERVICE_SECURITY_ENABLED */

bool ble_manager_is_connected()
{
    // LOG_DBG("Connection state is %u", current_conn != NULL);
    return current_conn != NULL;
}

bool ble_manager_control_is_subscribed()
{
    return bt_gatt_is_subscribed(current_conn, bt_gatt_find_by_uuid(NULL, 1, BT_UUID_CONTROL), BT_GATT_CCC_INDICATE);
}

uint32_t ble_manager_start_advertising()
{
    int err = 0;
    err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd,
                          ARRAY_SIZE(sd));
    if (err)
    {
        if (err == -EALREADY)
        {
            LOG_DBG("Advertising already started.");
        }
        else
        {
            LOG_ERR("Advertising failed to start (err %d)", err);
            return 0;
        }
    }
    return 1;
}

uint32_t ble_manager_stop_advertising()
{
    int err = 0;
    err = bt_le_adv_stop();
    if (err)
    {
        if (err == -EALREADY)
        {
            LOG_DBG("Advertising already stopped.");
        }
        else
        {
            LOG_ERR("Advertising failed to stop (err %d)", err);
            return 0;
        }
    }
    return 1;
}

uint8_t ble_manager_init()
{
    int err = 0;

    if (IS_ENABLED(CONFIG_XIAO_SERVICE_SECURITY_ENABLED))
    {
        err = bt_conn_auth_cb_register(&conn_auth_callbacks);
        if (err)
        {
            LOG_ERR("Failed to register authorization callbacks.");
            return 0;
        }

        err = bt_conn_auth_info_cb_register(&conn_auth_info_callbacks);
        if (err)
        {
            LOG_ERR("Failed to register authorization info callbacks.");
            return 0;
        }
    }

    err = bt_enable(NULL);
    if (err)
    {
        LOG_ERR("Error enabling the Bluetooth stack...");
    }

    LOG_INF("Bluetooth initialized");

    k_sem_give(&ble_init_ok);

    if (IS_ENABLED(CONFIG_SETTINGS))
    {
        settings_load();
    }

    err = xiao_service_init(&xiao_cb);
    if (err)
    {
        LOG_ERR("Failed to initialize XIAO service (err: %d)", err);
        return 0;
    }
    return 1;
}
