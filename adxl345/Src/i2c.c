/*
 * i2c.c
 *
 *  Created on: May 7, 2023
 *  Author: Marcin Rynkiewicz
 */
#include "stm32f4xx.h"

#define		GPIOBEN			(1U<<1)
#define		I2C1EN			(1U<<21)

#define		I2C_100KHZ				80
#define		SD_MODE_MAX_RISE_TIME	17

#define		CR1_PE			(1U<<0)
#define		SR2_BUSY		(1U<<1)
#define		CR1_START		(1U<<8)
#define		SR1_SB			(1U<<0)
#define		SR1_ADDR		(1U<<1)
#define		SR1_TXE			(1U<<7)
#define		CR1_ACK			(1U<<10)
#define		CR1_STOP		(1U<<9)
#define		SR1_RXNE		(1U<<6)
#define		SR1_BTF			(1U<<2)

// PB8 scl
// PB9 sda
void I2C1_Init()
{
	// enable clock for GPIOB
	RCC->AHB1ENR |= GPIOBEN;

	// set pb8 and pb9 to alternate
	GPIOB->MODER &= ~(1U<<16);
	GPIOB->MODER |= (1U<<17);

	GPIOB->MODER &= ~(1U<<18);
	GPIOB->MODER |= (1U<<19);

	// set pb8 and pb9 as open drain
	GPIOB->OTYPER |= (1U<<8);
	GPIOB->OTYPER |= (1U<<9);

	GPIOB->AFR[1] &= ~(1U<<0);
	GPIOB->AFR[1] &= ~(1U<<1);
	GPIOB->AFR[1] |= (1U<<2);
	GPIOB->AFR[1] &= ~(1U<<3);

	GPIOB->AFR[1] &= ~(1U<<4);
	GPIOB->AFR[1] &= ~(1U<<5);
	GPIOB->AFR[1] |= (1U<<6);
	GPIOB->AFR[1] &= ~(1U<<7);

	// enable clock for I2C1
	RCC->APB1ENR |= I2C1EN;

	// enter reset mode
	I2C1->CR1 |= (1U<<15);

	// leave reset mode
	I2C1->CR1 &= ~(1U<<15);

	// set peripheral clock to 16MHz
	I2C1->CR2 = (1U<<4);

	// set 100khz mode
	I2C1->CCR = I2C_100KHZ;

	// set rise time
	I2C1->TRISE = SD_MODE_MAX_RISE_TIME;

	// enable i2c1 module
	I2C1->CR1 |= CR1_PE;
}

void I2C_byteRead(char saddr, char maddr, char* data)
{

	// wait until bus is free
	while (I2C1->SR2 & SR2_BUSY) {}

	// generate start bit
	I2C1->CR1 |= CR1_START;

	// wait until start bit is set
	while (!(I2C1->SR1 & (SR1_SB))) {}

	// transmit slave address
	I2C1->DR = saddr << 1;

	// wait for address flag
	while (!(I2C1->SR1 & (SR1_ADDR))) {}

	// clear addr flag
	(void)I2C1->SR2;

	// send memory address
	I2C1->DR = maddr;

	// wait until out buffer empty
	while (!(I2C1->SR1 & (SR1_TXE))) {}

	// generate restart
	I2C1->CR1 |= CR1_START;

	// wait until start bit is set
	while (!(I2C1->SR1 & (SR1_SB))) {}

	// transmit slave address + read
	I2C1->DR = saddr << 1 | 1;

	// wait for address flag
	while (!(I2C1->SR1 & (SR1_ADDR))) {}

	// disable ack
	I2C1->CR1 &= ~CR1_ACK;

	// clear addr flag
	(void)I2C1->SR2;

	// generate stop after received data
	I2C1->CR1 |= CR1_STOP;

	while (!(I2C1->SR1 & SR1_RXNE)) {}

	// read the data
	*data++ = I2C1->DR;
}

void I2C_burstRead(char saddr, char maddr, int n, char* data)
{

	/* Wait until bus not busy */
	while (I2C1->SR2 & (SR2_BUSY));

    /* Generate start */
    I2C1->CR1 |= CR1_START;

    /* Wait until start flag is set */
    while (!(I2C1->SR1 & SR1_SB));

    /* Transmit slave address */
    I2C1->DR = saddr << 1;

    /* Wait until addr flag is set */
    while (!(I2C1->SR1 & SR1_ADDR));

    /* Clear addr flag */
    (void)I2C1->SR2;

    /* Wait until transmitter empty */
    while (!(I2C1->SR1 & SR1_TXE));

    /*Send memory address */
    I2C1->DR = maddr;

    /*Wait until transmitter empty */
    while (!(I2C1->SR1 & SR1_TXE));

    /*Generate restart */
    I2C1->CR1 |= CR1_START;

    /* Wait until start flag is set */
    while (!(I2C1->SR1 & SR1_SB));

    /* Transmit slave address */
    I2C1->DR = saddr << 1 | 1;

    /* Wait until addr flag is set */
    while (!(I2C1->SR1 & (SR1_ADDR)));

    /* Clear addr flag */
    (void)I2C1->SR2;

    /* Enable Acknowledge */
      I2C1->CR1 |=  CR1_ACK;

    while(n > 0)
    {
    	/*if one byte*/
    	if(n == 1)
    	{
    		/* Disable Acknowledge */
    	    I2C1->CR1 &= ~CR1_ACK;

    	    /* Generate Stop */
    	    I2C1->CR1 |= CR1_STOP;

 			/* Wait for RXNE flag set */
            while (!(I2C1->SR1 & SR1_RXNE));

            /* Read data from DR */
            *data++ = I2C1->DR;
            break;
    	}
    	else
    	{
       	 /* Wait until RXNE flag is set */
           while (!(I2C1->SR1 & SR1_RXNE));

           /* Read data from DR */
           (*data++) = I2C1->DR;

           n--;
    	}
    }
}

void I2C_burstWrite(char saddr, char maddr, int n, char* data)
{

	// wait until bus is free
	while (I2C1->SR2 & (SR2_BUSY));

	// generate start bit
	I2C1->CR1 |= CR1_START;

	// wait until start bit is set
	while (!(I2C1->SR1 & (SR1_SB)));

	// transmit slave address
	I2C1->DR = saddr << 1;

	// wait for address flag
	while (!(I2C1->SR1 & (SR1_ADDR)));

	// clear addr flag
	(void)I2C1->SR2;

	// wait until out buffer empty
	while (!(I2C1->SR1 & (SR1_TXE)));

	// send memory address
	I2C1->DR = maddr;

	for (int i = 0; i < n; i++)
	{
		// wait until out buffer empty
		while (!(I2C1->SR1 & (SR1_TXE)));

		I2C1->DR = *data++;

	}

	// wait until transfer finished
	while (!(I2C1->SR1 & SR1_BTF));

	I2C1->CR1 |= CR1_STOP;
}
