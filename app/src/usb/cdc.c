#include "cdc.h"

#include <stdio.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/ring_buffer.h>

#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(CDC_ACM_ECHO, LOG_LEVEL_DBG);

#define RING_BUF_SIZE 1024
uint8_t rring_buffer[RING_BUF_SIZE];
uint8_t tring_buffer[RING_BUF_SIZE];

struct ring_buf rx_usb_ringbuf;
struct ring_buf tx_usb_ringbuf;

const struct device *udev = DEVICE_DT_GET_ONE(zephyr_cdc_acm_uart);

#if IS_ENABLED(CONFIG_USB_DEVICE_STACK_NEXT)
USBD_CONFIGURATION_DEFINE(config_1,
                          USB_SCD_SELF_POWERED,
                          200);

USBD_DESC_LANG_DEFINE(sample_lang);
USBD_DESC_STRING_DEFINE(sample_mfr, "ZEPHYR", 1);
USBD_DESC_STRING_DEFINE(sample_product, "Zephyr USBD CDC ACM", 2);
USBD_DESC_STRING_DEFINE(sample_sn, "0123456789ABCDEF", 3);

USBD_DEVICE_DEFINE(sample_usbd,
                   DEVICE_DT_GET(DT_NODELABEL(zephyr_udc0)),
                   0x2fe3, 0x0001);

static int enable_usb_device_next(void)
{
    int err;

    err = usbd_add_descriptor(&sample_usbd, &sample_lang);
    if (err)
    {
        LOG_ERR("Failed to initialize language descriptor (%d)", err);
        return err;
    }

    err = usbd_add_descriptor(&sample_usbd, &sample_mfr);
    if (err)
    {
        LOG_ERR("Failed to initialize manufacturer descriptor (%d)", err);
        return err;
    }

    err = usbd_add_descriptor(&sample_usbd, &sample_product);
    if (err)
    {
        LOG_ERR("Failed to initialize product descriptor (%d)", err);
        return err;
    }

    err = usbd_add_descriptor(&sample_usbd, &sample_sn);
    if (err)
    {
        LOG_ERR("Failed to initialize SN descriptor (%d)", err);
        return err;
    }

    err = usbd_add_configuration(&sample_usbd, &config_1);
    if (err)
    {
        LOG_ERR("Failed to add configuration (%d)", err);
        return err;
    }

    err = usbd_register_class(&sample_usbd, "cdc_acm_0", 1);
    if (err)
    {
        LOG_ERR("Failed to register CDC ACM class (%d)", err);
        return err;
    }

    err = usbd_init(&sample_usbd);
    if (err)
    {
        LOG_ERR("Failed to initialize device support");
        return err;
    }

    err = usbd_enable(&sample_usbd);
    if (err)
    {
        LOG_ERR("Failed to enable device support");
        return err;
    }

    LOG_DBG("USB device support enabled");

    return 0;
}
#endif /* IS_ENABLED(CONFIG_USB_DEVICE_STACK_NEXT) */

const uint8_t BOOTLOADER_PATTERN[] = "Switch to Bootloader";

void USB_Handle_RCV(void)
{
    uint8_t buffer[1024];
    int rb_len;

    rb_len = ring_buf_get(&rx_usb_ringbuf, buffer, sizeof(buffer));

    if (rb_len > 0)
    {
        if (memcmp(BOOTLOADER_PATTERN, buffer, sizeof(BOOTLOADER_PATTERN) - 1) == 0)
        {
            LOG_INF("Going to bootloader mode");
            k_sleep(K_MSEC(5));
            app_reset_to_uf2();
        }
        else
        {
            LOG_DBG("%s", buffer);
        }
    }
}

static void interrupt_handler(const struct device *dev, void *user_data)
{
    ARG_UNUSED(user_data);

    while (uart_irq_update(dev) && uart_irq_is_pending(dev))
    {
        if (uart_irq_rx_ready(dev))
        {
            int recv_len;
            uint8_t buffer[300];

            recv_len = uart_fifo_read(dev, buffer, 300);
            if (recv_len < 0)
            {
                recv_len = 0;
            };

            ring_buf_put(&rx_usb_ringbuf, buffer, recv_len);

            for (size_t i = 0; i < recv_len; i++)
            {
                if (buffer[i] == '\n')
                {
                    USB_Handle_RCV();
                }
            }
        }

        if (uart_irq_tx_ready(dev))
        {
            uint8_t buffer[64];
            int rb_len, send_len;

            rb_len = ring_buf_get(&tx_usb_ringbuf, buffer, sizeof(buffer));
            if (!rb_len)
            {
                LOG_DBG("Ring buffer empty, disable TX IRQ");
                uart_irq_tx_disable(dev);
                continue;
            }

            send_len = uart_fifo_fill(dev, buffer, rb_len);
        }
    }
}

void USB_CDC_Init(void)
{
    uint32_t baudrate, dtr = 0U;
    int ret;

    if (!device_is_ready(udev))
    {
        LOG_ERR("CDC ACM device not ready");
        return;
    }

#if IS_ENABLED(CONFIG_USB_DEVICE_STACK_NEXT)
    ret = enable_usb_device_next();
#else
    ret = usb_enable(NULL);
#endif

    if (ret != 0 && ret != -EALREADY)
    {
        LOG_ERR("Failed to enable USB");
        return;
    }

    ring_buf_init(&rx_usb_ringbuf, sizeof(rring_buffer), rring_buffer);
    ring_buf_init(&tx_usb_ringbuf, sizeof(tring_buffer), tring_buffer);

    LOG_INF("Wait for DTR");

    while (true)
    {
        uart_line_ctrl_get(udev, UART_LINE_CTRL_DTR, &dtr);
        if (dtr)
        {
            break;
        }
        else
        {
            /* Give CPU resources to low priority threads. */
            k_sleep(K_MSEC(100));
        }
    }

    LOG_INF("DTR set");

    /* They are optional, we use them to test the interrupt endpoint */
    ret = uart_line_ctrl_set(udev, UART_LINE_CTRL_DCD, 1);
    if (ret)
    {
        LOG_WRN("Failed to set DCD, ret code %d", ret);
    }

    ret = uart_line_ctrl_set(udev, UART_LINE_CTRL_DSR, 1);
    if (ret)
    {
        LOG_WRN("Failed to set DSR, ret code %d", ret);
    }

    /* Wait 1 sec for the host to do all settings */
    k_busy_wait(1000000);

    ret = uart_line_ctrl_get(udev, UART_LINE_CTRL_BAUD_RATE, &baudrate);
    if (ret)
    {
        LOG_WRN("Failed to get baudrate, ret code %d", ret);
    }
    else
    {
        LOG_INF("Baudrate detected: %d", baudrate);
    }

    uart_irq_callback_set(udev, interrupt_handler);

    /* Enable rx interrupts */
    uart_irq_rx_enable(udev);
}

void USB_CDC_Send(uint8_t *buffer, int buffer_len)
{
    int rb_len;

    if (buffer_len < 0)
    {
        buffer_len = 0;
    }
    else if (buffer_len > 0)
    {
        if (buffer_len > 0)
        {
            rb_len = ring_buf_put(&tx_usb_ringbuf, buffer, buffer_len);
            if (rb_len < buffer_len)
            {
                LOG_ERR("Drop %u bytes", buffer_len - rb_len);
            }

            LOG_DBG("tty fifo -> ringbuf %d bytes", rb_len);
            if (rb_len)
            {
                uart_irq_tx_enable(udev);
            }
        }
    }
}

K_THREAD_DEFINE(cdc_init_thread_id, 1024, USB_CDC_Init, NULL, NULL, NULL, 7, 0, 0);