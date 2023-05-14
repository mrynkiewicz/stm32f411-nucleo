/*
 * exti.c
 *
 *  Created on: 14 may 2023
 *  Author: Marcin Rynkiewicz
 *  Description: driver to handle external interrupt from pin PC7.
 *  This pin is connected to INT1 output of the adxl345 board
 */


#include "exti.h"


#define SYSCFGEN		(1U<<14)
#define GPIOCEN			(1U<<2)

void EXTIPC7_Init()
{
	// disable global interrupts
	__disable_irq();

	// enable clock access for GPIO
	RCC->AHB1ENR |= GPIOCEN;

	// set PC7 pin as input
	GPIOC->MODER &= ~(1U<<15);
	GPIOC->MODER &= ~(1U<<14);

	// enable clock access for SYSCFG
	RCC->APB2ENR |= SYSCFGEN;

	// select port for EXTI7
	SYSCFG->EXTICR[1] |= (1U<<13);

	// Unmask EXTI7
	EXTI->IMR |= (1U<<7);

	// select rising edge trigger
	EXTI->RTSR |= (1U<<7);

	// enable EXTI line in NVIC
	NVIC_EnableIRQ(EXTI9_5_IRQn);

	// enable global interrupts
	__enable_irq();
}
