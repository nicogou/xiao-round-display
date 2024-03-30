#define DT_DRV_COMPAT seeedrounddisplay_chsc6x

#include <zephyr/sys/byteorder.h>
#include <zephyr/input/input.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

struct chsc6x_config {
	struct i2c_dt_spec i2c;
	const struct gpio_dt_spec int_gpio;
};

struct chsc6x_data {
	const struct device *dev;
	struct k_work work;
	struct gpio_callback int_gpio_cb;
};

struct chsc6x_output {
	uint8_t points;
	uint16_t x;
	uint16_t y;
} __attribute__((packed));

LOG_MODULE_REGISTER(chsc6x, CONFIG_INPUT_LOG_LEVEL);

static int chsc6x_process(const struct device *dev)
{
	// uint8_t event;
	uint16_t row, col;
	bool is_pressed;

	struct chsc6x_output output;
	const struct chsc6x_config *cfg = dev->config;

	
	int res = i2c_burst_read_dt(&cfg->i2c, 0, (uint8_t *)&output, sizeof(output));
	
	if (res < 0)
		{
			LOG_ERR("Could not read data %i", res);
			return -ENODATA;
		}

	
	is_pressed = output.points & 0xff;
	col = sys_be16_to_cpu(output.x) & 0x00ff;
	row = sys_be16_to_cpu(output.y) & 0x00ff;
	LOG_DBG("Touched : %u - Column : %u - Row : %u", is_pressed, col, row);

	if (is_pressed) {
		// These events are generated for the LVGL touch implementation.
		input_report_abs(dev, INPUT_ABS_X, col, false, K_FOREVER);
		input_report_abs(dev, INPUT_ABS_Y, row, false, K_FOREVER);
		input_report_key(dev, INPUT_BTN_TOUCH, 1, true, K_FOREVER);
	} else {
		// This event is generated for the LVGL touch implementation.
		input_report_key(dev, INPUT_BTN_TOUCH, 0, true, K_FOREVER);
	}

	return 0;
}

static void chsc6x_work_handler(struct k_work *work)
{
	struct chsc6x_data *data = CONTAINER_OF(work, struct chsc6x_data, work);

	chsc6x_process(data->dev);
}

static void chsc6x_isr_handler(const struct device *dev, struct gpio_callback *cb, uint32_t mask)
{
	struct chsc6x_data *data = CONTAINER_OF(cb, struct chsc6x_data, int_gpio_cb);

	k_work_submit(&data->work);
}

static int chsc6x_chip_init(const struct device *dev)
{	
	const struct chsc6x_config *cfg = dev->config;

	if (!device_is_ready(cfg->i2c.bus)) {
		LOG_ERR("I2C bus %s not ready", cfg->i2c.bus->name);
		return -ENODEV;
	}

	return 0;
}

static int chsc6x_init(const struct device *dev)
{
	struct chsc6x_data *data = dev->data;

	data->dev = dev;
	k_work_init(&data->work, chsc6x_work_handler);

	LOG_DBG("Initialize CHSC6X");

	const struct chsc6x_config *config = dev->config;

	if (!gpio_is_ready_dt(&config->int_gpio)) {
		LOG_ERR("GPIO port %s not ready", config->int_gpio.port->name);
		return -EIO;
	}

	if (gpio_pin_configure_dt(&config->int_gpio, GPIO_INPUT) < 0) {
		LOG_ERR("Could not configure interrupt GPIO pin");
		return -EIO;
	}

	if (gpio_pin_interrupt_configure_dt(&config->int_gpio, GPIO_INT_EDGE_TO_ACTIVE) < 0) {
		LOG_ERR("Could not configure interrupt GPIO interrupt.");
		return -EIO;
	}

	gpio_init_callback(&data->int_gpio_cb, chsc6x_isr_handler, BIT(config->int_gpio.pin));

	if (gpio_add_callback(config->int_gpio.port, &data->int_gpio_cb) < 0) {
		LOG_ERR("Could not set gpio callback");
		return -EIO;
	}

	return chsc6x_chip_init(dev);
};

// #ifdef CONFIG_PM_DEVICE
// static int chsc6x_pm_action(const struct device *dev, enum pm_device_action action)
// {
// 	const struct chsc6x_config *config = dev->config;
// 	int status;

// 	LOG_DBG("Status: %u", action);

// 	switch (action) {
// 		case PM_DEVICE_ACTION_SUSPEND: {
// 			LOG_DBG("State changed to suspended");
// 			if (device_is_ready(config->rst_gpio.port)) {
// 				status = gpio_pin_set_dt(&config->rst_gpio, 1);
// 			}

// 			break;
// 		}
// 		case PM_DEVICE_ACTION_RESUME: {
// 			LOG_DBG("State changed to active");
// 			status = chsc6x_chip_init(dev);

// 			break;
// 		}
// 		default: {
// 			return -ENOTSUP;
// 		}
// 	}

// 	return status;
// }
// #endif

#define CHSC6X_DEFINE(index)                                                               \
	static const struct chsc6x_config chsc6x_config_##index = {                           \
		.i2c = I2C_DT_SPEC_INST_GET(index),                                                 \
		.int_gpio = GPIO_DT_SPEC_INST_GET(index, irq_gpios),	            				\
	};                                                                                      \
	static struct chsc6x_data chsc6x_data_##index;                                        \
	DEVICE_DT_INST_DEFINE(index, chsc6x_init, NULL, &chsc6x_data_##index,                 \
						  &chsc6x_config_##index, POST_KERNEL, CONFIG_INPUT_INIT_PRIORITY, \
						  NULL);

DT_INST_FOREACH_STATUS_OKAY(CHSC6X_DEFINE)