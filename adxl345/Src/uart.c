/*
 * uart.c
 *
 *  Created on: May 11, 2023
 *  Author: Marcin Rynkiewicz
 *  Descripion: Small module to send out data via RS232.
 * 		It's used to send data from Nucleo board to PC, that's why only transmitting part implemented.
 */

#include <stdint.h>
#include "uart.h"
#include "stm32f4xx.h"

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk,  uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);

void uart2_tx_init(void)
{

	// enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	// set PA2 mode to alternate function mode
	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |= (1U<<5);

	// set PA2 alternate function type to UART_TX (AF07)
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

	// enable clock access to uart2
	RCC->APB1ENR |= UART2EN;

	// Configure baudrate
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);

	// configure the transfer direction
	USART2->CR1 =  CR1_TE;

	// enable uart module
	USART2->CR1 |= CR1_UE;

}


void uart2_write(int ch)
{
	// make sure the transmit data register is empty
	while(!(USART2->SR & SR_TXE)){}

	// write to transmit data register
	USART2->DR	=  (ch & 0xFF);
}




static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk,  uint32_t BaudRate)
{
	USARTx->BRR =  compute_uart_bd(PeriphClk,BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
	return ((PeriphClk + (BaudRate/2U))/BaudRate);
}

