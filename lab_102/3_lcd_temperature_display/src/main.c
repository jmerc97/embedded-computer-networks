/*
 * main.c
 *
 * this is the main lcd application
 *
 * author:    Dr. Alex Shenfield
 * date:      04/09/2018
 * purpose:   55-604481 embedded computer networks : lab 102
 */

// include the basic headers and hal drivers
#include "stm32f7xx_hal.h"

// include the shu bsp libraries for the stm32f7 discovery board
#include "pinmappings.h"
#include "clock.h"
#include "stm32746g_discovery_lcd.h"
#include "adc.h"
#include "gpio.h"

gpio_pin_t pinTemp = {PA_0, GPIOA, GPIO_PIN_0};

// LCD DEFINES

// define a message boarder (note the lcd is 28 characters wide using Font24)
#define BOARDER     "****************************"

// specify a welcome message
const char * welcome_message[2] = 
{
  "*     Hello LCD World!     *",
  "*      Welcome to SHU      *"
};

// CODE

// this is the main method
int main()
{
  // we need to initialise the hal library and set up the SystemCoreClock 
  // properly
  HAL_Init();
  init_sysclk_216MHz();
  init_adc(pinTemp);
	
  // initialise the lcd
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, SDRAM_DEVICE_ADDR);
  BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);

  // set the background colour to blue and clear the lcd
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
  BSP_LCD_Clear(LCD_COLOR_BLACK);
  
  // set the font to use
  BSP_LCD_SetFont(&Font24); 
  
  // print the welcome message ...
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_DisplayStringAtLine(0, (uint8_t *)BOARDER);
  BSP_LCD_DisplayStringAtLine(1, (uint8_t *)welcome_message[0]);
  BSP_LCD_DisplayStringAtLine(2, (uint8_t *)welcome_message[1]);
  BSP_LCD_DisplayStringAtLine(3, (uint8_t *)BOARDER); 
    
  // delay a little ...
  
  while(1)
  {
    
		uint16_t adc_val = read_adc(pinTemp);
		
		adc_val = (adc_val / 4096 * 100) * 5;
		
		char adc[12];
		sprintf(adc, "ADC = %4d", adc_val);
		BSP_LCD_DisplayStringAtLine(5, (uint8_t *)adc);
		
		HAL_Delay(1000);
  }
}

