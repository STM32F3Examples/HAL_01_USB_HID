#include "stm32f3xx.h"

/**
 * This code use 
 */

/**
 * Configures the to PB13 as a output where the user leds is connected
 */
void led_init(void);

/**
 * Configure the pin PA13 as a input with internal pull-up, where the user button
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
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	//Configure PB13  as push pull ouput an set the output to high 
	GPIOB->MODER &=~(GPIO_MODER_MODER13);
	GPIOB->MODER |=GPIO_MODER_MODER13_0;//output
	GPIOB->ODR |= GPIO_ODR_13;
}

void button_init(void){
	//needed for interrupt source remaping
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	
	//Configure PA13 as input with pull-up
	GPIOC->MODER &=~(GPIO_MODER_MODER13);
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR13_0;

	//configure interrupt
	RCC->APB2ENR|= RCC_APB2ENR_SYSCFGEN;//Enable sysconfig registers
	SYSCFG->EXTICR[3] |=SYSCFG_EXTICR4_EXTI13_PC;
	EXTI->IMR |= EXTI_IMR_MR13;
	EXTI->RTSR |= EXTI_RTSR_TR13;
	EXTI->FTSR |= EXTI_FTSR_TR13;
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
 * Exteral interrupt handler
 */
void EXTI15_10_IRQHandler(void){
	EXTI->PR = EXTI_PR_PR13;
	GPIOB->ODR = GPIOC->IDR;//Transfer button value to led
}
