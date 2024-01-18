#include "ble_xiaos_control.h"

LOG_MODULE_REGISTER(ble_xiaos_control, LOG_LEVEL_DBG);

static uint8_t _queue_command(uint8_t opcode, uint8_t *data, uint8_t length)
{
    if (length > XIAO_CONTROL_DATA_MAX_LEN)
    {
        LOG_ERR("Data length too long");
        return 0;
    }
    else
    {
        uint8_t dest[XIAO_CONTROL_DATA_MAX_LEN];
        memcpy(&dest[0], &opcode, sizeof(uint8_t));
        memcpy(&dest[1], &length, sizeof(uint8_t));
        memcpy(&dest[2], data, length);
        char str[60];
        sprintf(str, "Building Opcode %.2x - Length %.2x - Payload", opcode, length);
        LOG_HEXDUMP_INF(data, length, str);

        int8_t ret = control_send(NULL, dest, length + 2);
        if (ret == 0)
        {
            return 1;
        }
        else
        {
            LOG_ERR("Failed to send opcode %.2x (%i)", opcode, ret);
            return 0;
        }
    }
}

uint32_t ble_xiaos_control_queue(uint8_t opcode, uint8_t *data, uint8_t len)
{
    return _queue_command(opcode, data, len);
}

bool ble_xiaos_control_ready()
{
    return ble_manager_control_is_subscribed();
}

bool ble_xiaos_control_authenticated()
{
    return true;
}

static void _resp_err(uint8_t opcode)
{
    _queue_command((uint32_t)(opcode | RESPONSE_BIT), NULL, 0);
}

uint8_t ble_xiaos_control_process(const uint8_t *const data, uint16_t length)
{
    if (length < 2)
    {
        LOG_ERR("Length of packet received is too small (%u)", length);
        return 0;
    }

    uint8_t opcode;
    uint8_t len;
    uint8_t payload[XIAO_CONTROL_DATA_MAX_LEN];
    memcpy(&opcode, &data[0], sizeof(uint8_t));
    memcpy(&len, &data[1], sizeof(uint8_t));
    memcpy(payload, &data[2], len * sizeof(uint8_t));

    char str[80];
    sprintf(str, "Control packet written with Opcode %.2x - Length %.2x - Payload", opcode, len);
    LOG_HEXDUMP_INF(data, length, str);

    switch (opcode)
    {
    
    case 0x7F:
        if (len != 0)
        {
            _resp_err(opcode);
            break;
        }

        app_reset_to_uf2();
        break;

    default:
        LOG_WRN("Unhandled opcode: %.2x", (uint8_t)opcode);
        break;
    }

    return 1;
}