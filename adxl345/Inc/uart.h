/*
 * uart.h
 *
 *  Created on: May 11, 2023
 *  Author: Marcin Rynkiewicz
 */

#ifndef UART_H_
#define UART_H_


#define GPIOAEN			(1U<<0)
#define UART2EN			(1U<<17)

#define CR1_TE			(1U<<3)
#define CR1_UE			(1U<<13)
#define SR_TXE			(1U<<7)



#define SYS_FREQ		16000000 // by default stm32f411 nucleo is clocked with 16MHz HSI
#define APB1_CLK		SYS_FREQ

#define UART_BAUDRATE		115200

void uart2_tx_init(void);
void uart2_write(int ch);


#endif /* UART_H_ */
