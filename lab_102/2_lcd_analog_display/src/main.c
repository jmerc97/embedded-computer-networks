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

gpio_pin_t pinPot = {PA_0, GPIOA, GPIO_PIN_0};
gpio_pin_t ledHalf = {PB_8, GPIOB, GPIO_PIN_8};
gpio_pin_t ledFull = {PB_9, GPIOB, GPIO_PIN_9};

uint16_t prevAdcVal = 0;

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
  init_adc(pinPot);
	init_gpio(ledHalf, OUTPUT);
	init_gpio(ledFull, OUTPUT);
	
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
    
		uint16_t adcVal = read_adc(pinPot);
		
		uint16_t xPos = 0;
		uint16_t yPos = 240;
		uint16_t height = 12;
		uint16_t barLength;
		
		adcVal = adcVal / 4095.00 * 100;
		
		barLength = adcVal * 480 / 100;
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_FillRect(xPos, yPos, barLength, height);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(barLength, yPos, (480-barLength), height);
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);

		char adc[12];
		char half[16];
		char full[16];
		sprintf(adc, "ADC = %4d", adcVal);
		BSP_LCD_DisplayStringAtLine(5, (uint8_t *)adc);
		
		if (adcVal < 50)
		{
			write_gpio(ledHalf, HIGH);
			write_gpio(ledFull, LOW);
			sprintf(half, "LED 1 is high");
	
			sprintf(full, "LED 2 is low ");
			BSP_LCD_DisplayStringAtLine(7, (uint8_t *)half);
			BSP_LCD_DisplayStringAtLine(8, (uint8_t *)full);
		}
		else if (adcVal > 50)
		{
			write_gpio(ledFull, HIGH);
			write_gpio(ledHalf, LOW);
			sprintf(half, "LED 1 is low ");
			sprintf(full, "LED 2 is high");
			BSP_LCD_DisplayStringAtLine(7, (uint8_t *)half);
			BSP_LCD_DisplayStringAtLine(8, (uint8_t *)full);
		}
		else if (adcVal == 50)
		{
			write_gpio(ledHalf, HIGH);
			write_gpio(ledFull, HIGH);
			sprintf(half, "LED 1 is high");
			sprintf(full, "LED 2 is high");
			BSP_LCD_DisplayStringAtLine(7, (uint8_t *)half);
			BSP_LCD_DisplayStringAtLine(8, (uint8_t *)full);
		}
		
		HAL_Delay(100);
  }
}
