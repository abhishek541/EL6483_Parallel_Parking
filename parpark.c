/*
	This is the main user software application
	In this case, we have below the code necessary to toggle a set of LEDs on a	STM32 F3 Discovery PCB at 20 Hz.

	(c) Abhimanyu Ghosh, 2017
 */

#include "cpu.h"
#include "board_led.h"
#include "timer_hal.h"
#include "uart.h"

#include "foo.h"

//Ultrasonic definitions
#define Ultra_Left_Echo     GPIO_Pin_11
#define Ultra_Left_EchoLine   EXTI_Line0

#define Ultra_Right_Echo     GPIO_Pin_6
#define Ultra_Right_EchoLine   EXTI_Line1

#define Ultra_Middle_Echo     GPIO_Pin_13
#define Ultra_Middle_EchoLine   EXTI_Line3

#define SR04_TIMIRQ   TIM2_IRQn
#define SR04_Prescaler  2
#define SR04_Trig     GPIO_Pin_0

#define SR04_GPIO     GPIOA
#define SR04_Timer    TIM2

#define FallingEdge 0
#define RisingEdge 1

#define HALF_SOUND_SPEED_10USEC (171.605e-5)

volatile uint32_t trigger_val;
volatile uint32_t echo_val;
volatile uint32_t echo_duration;
volatile float distance;

void send_pulse(void) {
  US_TIM1_Init();
  board_led_init();
  HAL_TIM_Base_Stop(&htim1);
  
  HAL_TIM_Base_Start(&htim1);
  board_led_on(LED1);
  trigger_val = HAL_GetTick();
  echo_val = 0;

}

void get_echo(void) {
  US_TIM2_Init();
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  if( htim->Instance == TIM2 )
  {
    echo_val = HAL_GetTick() - trigger_val;
  }

  echo_duration = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_1 ); 
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
  board_led_init();

  uart_debug_init();

  uart_send_data_blocking(UART_PORT1, (uint8_t *)"Hello World!!\r\n", 15U);

  get_echo();

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
      send_pulse();
      HAL_Delay(49);

      calculate_distance();
      uint8_t *s = (uint8_t *) &distance;
      uart_send_data_blocking(UART_PORT1, s, 15U);

      ++i; // Increment i for the next test iteration...
    }
  }

  return 0;
}
