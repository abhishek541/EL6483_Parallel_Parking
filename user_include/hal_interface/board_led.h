#ifndef BOARD_LED_H
#define BOARD_LED_H		1

#include "hal_common_includes.h"

void board_led_init(void);
void board_led_on(void);
void board_led_off(void);
void board_led_toggle(void);

#endif