/*
	This is the main user software application
	In this case, we have below the code necessary to toggle a set of LEDs on a	STM32 F3 Discovery PCB at 20 Hz.

	(c) Abhimanyu Ghosh, 2017
 */

#include "cpu.h"
#include "board_led.h"

#include "foo.h"

//Ultrasonic definitions
#define Ultra_Left_Echo     GPIO_Pin_11
#define Ultra_Left_EchoLine   EXTI_Line0
#define Ultra_Left_EchoIRQ  EXTI0_1_IRQn

#define Ultra_Right_Echo     GPIO_Pin_6
#define Ultra_Right_EchoLine   EXTI_Line1
#define Ultra_Right_EchoIRQ  EXTI0_1_IRQn

#define Ultra_Middle_Echo     GPIO_Pin_13
#define Ultra_Middle_EchoLine   EXTI_Line1
#define Ultra_Middle_EchoIRQ  EXTI0_1_IRQn

#define SR04_TIMIRQ   TIM2_IRQn
#define SR04_Prescaler  2
#define SR04_Trig     GPIO_Pin_0

#define SR04_GPIO     GPIOA
#define SR04_Timer    TIM2

#define FallingEdge 0
#define RisingEdge 1

void send_pulse() {

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
#ifndef HAL_COMMON_INCLUDES_H

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
      board_led_on(LED1);
      board_led_off(LED2);

      cpu_sw_delay(50U);  // Invoke a simple software busy-wait routine to delay approximately 50 milliseconds

      board_led_off(LED1);
      board_led_on(LED2);

      cpu_sw_delay(50U);

      ++i; // Increment i for the next test iteration...
    }
  }

  return 0;
}
