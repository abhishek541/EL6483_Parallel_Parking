#ifndef TIMER_HAL_H_
#define TIMER_HAL_H_	1

#include "hal_common_includes.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim15;

void US_TIM1_Init(void);
void US_TIM2_Init(void);
void US_TIM3_Init(void);
void US_TIM4_Init(void);
void US_TIM15_Init(void);

#endif