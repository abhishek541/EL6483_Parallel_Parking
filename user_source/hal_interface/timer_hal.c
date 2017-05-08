#include "timer_hal.h"

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

void US_TIM1_Init(void) {

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 71;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 10;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&htim1);

}

void US_TIM2_Init(void) {

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 630;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&htim2);

}