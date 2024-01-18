#include <main.h>

LOG_MODULE_REGISTER(MAIN, LOG_LEVEL_DBG);

void app_reset_to_uf2()
{
	NRF_POWER->GPREGRET = 0x57; // 0xA8 OTA, 0x4e Serial
	NVIC_SystemReset();
}

int main(void)
{
	while(true){
		LOG_INF("Hello World! %s - %s", CONFIG_BOARD, APP_VERSION_STRING);
		k_sleep(K_SECONDS(1));
	}
	return 0;
}
