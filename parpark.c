/*
	This is the main user software application
	In this case, we have below the code necessary to toggle a set of LEDs on a	STM32 F3 Discovery PCB at 20 Hz.

	(c) Abhimanyu Ghosh, 2017
 */

#include "cpu.h"
#include "timer_hal.h"
#include "board_led.h"
#include "uart.h"

#include "foo.h"

//Ultrasonic definitions
// #define Ultra_Left_Echo     GPIO_Pin_11
// #define Ultra_Left_EchoLine   EXTI_Line0

// #define Ultra_Right_Echo     GPIO_Pin_6
// #define Ultra_Right_EchoLine   EXTI_Line1

// #define Ultra_Middle_Echo     GPIO_Pin_13
// #define Ultra_Middle_EchoLine   EXTI_Line3

// #define SR04_TIMIRQ   TIM2_IRQn
// #define SR04_Prescaler  2
// #define SR04_Trig     GPIO_Pin_0

// #define SR04_GPIO     GPIOA
// #define SR04_Timer    TIM2

// #define FallingEdge 0
// #define RisingEdge 1

#define HALF_SOUND_SPEED_10USEC (171.605e-5)

volatile uint32_t trigger_val;
volatile uint32_t echo_val;
volatile uint32_t echo_duration;
volatile float distance;

// void send_pulse(void) {
//   HAL_TIM_Base_Stop_IT(&htim2);
  
//   //board_led_on(LED1);
//   HAL_TIM_Base_Start_IT(&htim2);
  
//   trigger_val = HAL_GetTick();
//   echo_val = 0;

// }

// void get_echo(void) {
//   US_TIM1_Init();
//   HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_2);
// }

void TIM1_BRK_TIM15_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim1);
  HAL_TIM_IRQHandler(&htim15);
}

void TIM1_CC_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim1);
}

void TIM3_IRQHandler(void){
  HAL_TIM_IRQHandler(&htim3);
}

void TIM4_IRQHandler(void){
  HAL_TIM_IRQHandler(&htim4);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{ 
  uart_send_data_blocking(UART_PORT1, (uint8_t *)"Capture \r\n", 15U);
  if( htim->Instance == TIM1 )
  {
    echo_val = HAL_GetTick() - trigger_val;
  }
  echo_duration = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_2 ); 
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  //uart_send_data_blocking(UART_PORT1, (uint8_t *)"Overflow \r\n", 15U);
  if( htim->Instance == TIM3 )
  {
    //uart_send_data_blocking(UART_PORT1, (uint8_t *)"Overflow \r\n", 15U);
    HAL_TIM_Base_Start_IT(&htim4);
    board_led_on();
  }
  if( htim->Instance == TIM4 )
  {
    HAL_TIM_Base_Stop_IT(&htim4);
    board_led_off();
    trigger_val = HAL_GetTick();
    HAL_TIM_IC_Start_IT( &htim1, TIM_CHANNEL_2 );
  }
  
    //board_led_toggle();
   // HAL_TIM_Base_Stop_IT(htim);
}

void calculate_distance(void) {
  distance = (float)echo_duration*HALF_SOUND_SPEED_10USEC;
}

int main()
{
  /*
    Initialize the PLL, clock tree to all peripherals, flash and Systick 1 ms time reference:
   */
  cpu_init();
  /*
    Initialize the GPIO (General-Purpose I/O) subsystem pins that are connected to the LEDs on the board:
   */
  US_TIM1_Init();
  US_TIM3_Init();
  US_TIM4_Init();

  board_led_init();

  uart_debug_init();

  HAL_TIM_Base_Start_IT(&htim3);

  HAL_NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
  HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);

  HAL_NVIC_SetPriority(TIM3_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);

  HAL_NVIC_SetPriority(TIM4_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(TIM4_IRQn);

  uart_send_data_blocking(UART_PORT1, (uint8_t *)"Measure \r\n", 15U);

  //get_echo();
  calculate_distance();
  uint8_t *s = (uint8_t *) &distance;
  uart_send_data_blocking(UART_PORT1, s, 15U);

  int i = 0;

  /*
    In an infinite loop, keep toggling the LEDs in an alternating pattern:
   */
  while(1)
  {
    /*
      Carry out a simple unit test of foo() declared in foo.h:
     */
    if(TEST_FOO(i, i+1) < 0)
    {
      /*
        If the above fails there is either a hardware, code or other undefined error.
        Now we're in an undefined state regarding processor core behavior...
       */
      while(1); // We probably have had a radiation hit or a major malfunction on the ALU of the processor...
    }
    else
    {
      calculate_distance();
      uint8_t *s = (uint8_t *) &distance;
      uart_send_data_blocking(UART_PORT1, s, 15U);
      //send_pulse();
      //HAL_Delay(49);

      //calculate_distance();
      //uint8_t *s = (uint8_t *) &distance;
      //uart_send_data_blocking(UART_PORT1, s, 15U);

      ++i; // Increment i for the next test iteration...
    }
  }

  return 0;
}
