/*
 * main.c
 *
 * this is the main rtos blinky application
 *
 * author:    Dr. Alex Shenfield
 * date:      01/09/2018
 * purpose:   55-604481 embedded computer networks : lab 103
 */

// include the basic headers for the hal drivers
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"

// include the clock configuration file from the shu bsp library
#include "clock.h"
#include "gpio.h"
#include "pinmappings.h"

gpio_pin_t ledOne = {PF_6, GPIOF, GPIO_PIN_6};
gpio_pin_t ledTwo = {PF_7, GPIOF, GPIO_PIN_7};
gpio_pin_t ledThree = {PF_8, GPIOF, GPIO_PIN_8};

void ledOneThread(void const *argument);
void ledTwoThread(void const *argument);
void ledThreeThread(void const *argument);

osThreadDef(ledOneThread, osPriorityNormal, 1, 0);
osThreadDef(ledTwoThread, osPriorityNormal, 1, 0);
osThreadDef(ledThreeThread, osPriorityNormal, 1, 0);

void dumb_delay(uint32_t delay);

// declare the extern method that sets everything up for us 
extern int init_thread(void);

int init_thread(void)
{
	init_gpio(ledOne, OUTPUT);
	init_gpio(ledTwo, OUTPUT);
	init_gpio(ledThree, OUTPUT);
	
	ledOneThread = osThreadCreate(osThread(ledOneThread), NULL);
	ledTwoThread = osThreadCreate(osThread(ledTwoThread), NULL);
	
	
}

// this is the main method
int main()
{ 
	
  // initialise the real time kernel
  osKernelInitialize();
  
  // we need to initialise the hal library and set up the SystemCoreClock 
  // properly
  HAL_Init();
  init_sysclk_216MHz();
  
  // at this stage the microcontroller clock setting is already configured. 
  // this is done through SystemInit() function which is called from the 
  // startup file (startup_stm32f7xx.s) before to branch to application main.
  // to reconfigure the default setting of SystemInit() function, refer to
  // system_stm32f7xx.c file
  //
  // note also that we need to set the correct core clock in the RTX_Conf_CM.c
  // file (OS_CLOCK) which we can do using the configuration wizard
    
  // initialise our threads
  init_thread();
  
  // start everything running
  osKernelStart();
}
