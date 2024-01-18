#ifndef CDC_INCLUDE
#define CDC_INCLUDE

#include <main.h>

void USB_CDC_Init(void);
void USB_CDC_Send(uint8_t *buffer, int buffer_len);

#endif