#ifndef TIMER_HAL_H_
#define TIMER_HAL_H_	1

#include "hal_common_includes.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

void US_TIM1_Init(void);
void US_TIM2_Init(void);

#endif