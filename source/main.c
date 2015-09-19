#include "stm32f3xx.h"

/**
 * This code use 
 */

/**
 * Configures the to PB13 as a output where the user leds is connected
 */
void led_init(void);

/**
 * Configure the pin PC13 as a input with internal pull-up, where the user button
 * connected, this pin low active, This function enables interrupt EXTI15_10_IRQn
 */
void button_init(void);

int main(){
	//System configuration
	led_init();
	button_init();

	//User application
	while(1){
	}
	return 0;
}


void led_init(void){
	//Turn on the GPIOB peripherial
	__GPIOB_CLK_ENABLE();
	//Configure PB13  as push pull ouput an set the output to high 
	GPIO_InitTypeDef myGPIO;
	myGPIO.Mode = GPIO_MODE_OUTPUT_PP;
	myGPIO.Pin = GPIO_PIN_13;
	myGPIO.Pull = GPIO_NOPULL;
 	myGPIO.Speed = GPIO_SPEED_LOW;
	
	HAL_GPIO_Init(GPIOB, &myGPIO);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,ENABLE);
 }

void button_init(void){
	__GPIOC_CLK_ENABLE();	
	//Configure PC13 as input with pull-up
	GPIO_InitTypeDef myGPIO;
	myGPIO.Mode = GPIO_MODE_INPUT | GPIO_MODE_IT_RISING_FALLING;
	myGPIO.Pin = GPIO_PIN_13;
	myGPIO.Pull = GPIO_PULLUP;
 	myGPIO.Speed = GPIO_SPEED_LOW;
	
	HAL_GPIO_Init(GPIOC, &myGPIO);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
 * Exteral interrupt handler
 */
void EXTI15_10_IRQHandler(void){
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
	//Transfer button value to led
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13));
}
