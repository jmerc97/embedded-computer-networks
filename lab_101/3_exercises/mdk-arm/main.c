/*
 * b_led_dice.c
 *
 * this is the main application skeleton for the student exercises in task 3
 *
 */

// include the hal drivers
#include "stm32f7xx_hal.h"

// include the shu bsp libraries for the stm32f7 discovery board
#include "pinmappings.h"
#include "clock.h"
#include "gpio.h"
#include "random_numbers.h"

// map the led to GPIO PA8
gpio_pin_t ledOne = {PI_1, GPIOI, GPIO_PIN_1};
gpio_pin_t ledTwo = {PB_14, GPIOB, GPIO_PIN_14};
gpio_pin_t ledThree = {PB_15, GPIOB, GPIO_PIN_15};

uint32_t randNum;

// this is the main method
int main()
{
	
	HAL_Init();
  init_sysclk_216MHz();
  init_gpio(ledOne, OUTPUT);
  init_gpio(ledTwo, OUTPUT);
	init_gpio(ledThree, OUTPUT);
	init_random();
	
	while(1)
  {
		
		randNum = (get_random_int() % 3) + 1;
		
    // toggle the led on the gpio pin
		if (randNum == 1)
		{
				write_gpio(ledOne, HIGH);
		}
		else if (randNum == 2)
		{
				write_gpio(ledOne, HIGH);
				write_gpio(ledTwo, HIGH);
		}
		else if (randNum == 3)
		{
				write_gpio(ledOne, HIGH);
				write_gpio(ledTwo, HIGH);
				write_gpio(ledThree, HIGH);
		}
		
		HAL_Delay(1000);
		
		write_gpio(ledOne, LOW);
		write_gpio(ledTwo, LOW);
		write_gpio(ledThree, LOW);
		
		HAL_Delay(1000);
		
  }
	
}
