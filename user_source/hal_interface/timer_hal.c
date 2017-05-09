#include "timer_hal.h"

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim15;

void US_TIM1_Init(void) {
  
  TIM_IC_InitTypeDef tim_conf_IC;
  TIM_ClockConfigTypeDef tim_clk_init;

  htim1.Instance = TIM1;
  htim1.Channel = HAL_TIM_ACTIVE_CHANNEL_2;
  htim1.Init.Prescaler = 630;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&htim1);
  
  tim_clk_init.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  tim_clk_init.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
  HAL_TIM_ConfigClockSource(&htim1, &tim_clk_init);

  HAL_TIM_IC_Init(&htim1);

  tim_conf_IC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  tim_conf_IC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  tim_conf_IC.ICPrescaler = TIM_ICPSC_DIV1;
  tim_conf_IC.ICFilter = 0;
  HAL_TIM_IC_ConfigChannel(&htim1, &tim_conf_IC, TIM_CHANNEL_2);  

}

void US_TIM2_Init(void) {

  __TIM2_CLK_ENABLE();
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 250;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.RepetitionCounter = 0;
  
  HAL_TIM_Base_Init(&htim2);

}

void US_TIM3_Init(void) {

  __TIM3_CLK_ENABLE();
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 71;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.RepetitionCounter = 0;
  
  HAL_TIM_Base_Init(&htim3);

}

void US_TIM4_Init(void) {
  
  __TIM4_CLK_ENABLE();
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 71;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 9;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.RepetitionCounter = 0;

  HAL_TIM_Base_Init(&htim4);

}

void US_TIM15_Init(void) {

  __TIM15_CLK_ENABLE();
  htim15.Instance = TIM15;
  htim15.Init.Prescaler = 71;
  htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim15.Init.Period = 250;
  htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim15.Init.RepetitionCounter = 0;
  
  HAL_TIM_Base_Init(&htim15);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim->Instance==TIM1)
  {
    __TIM1_CLK_ENABLE();
  
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;

    __HAL_RCC_GPIOE_CLK_ENABLE();
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
    HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim)
{
  if(htim->Instance==TIM1)
  {
    __TIM1_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_11);

    HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);
  }
}