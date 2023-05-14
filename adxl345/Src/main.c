
#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "adxl345.h"
#include "uart.h"
#include "fir.h"
#include "exti.h"

// values read from adxl345 data registers
int16_t x, y, z;
// final values of the acceleration
double xg, yg, zg;
// low pass FIR filter
FIRFilter lpfAcc;
// global flag to mark data availability from IRQ Handler
uint8_t data_ready;
// array for data from adxl data registers
extern uint8_t data_values[6];
// enable floating point unit
static void fpu_enable();

int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}

int main(void)
{

	fpu_enable();
	EXTIPC7_Init();
	adxl_init();
    uart2_tx_init();

    FIRFilter_Init(&lpfAcc);

	for(;;)
	{
		if (data_ready == 1)
		{
			adxl_read_values(DATA_START_ADDR);

			x = ((data_values[1]<<8) | data_values[0]);
			y = ((data_values[3]<<8) | data_values[2]);
			z = ((data_values[5]<<8) | data_values[4]);

			xg = x * 0.0078;
			yg = y * 0.0078;
			zg = z * 0.0078;

			FIRFilter_Updater(&lpfAcc, xg);

			printf("%.4f, %.4f\n\r", xg, lpfAcc.out);
			data_ready = 0;
		}
	}
}

static void fpu_enable()
{
	SCB->CPACR |= (1U<<20);
	SCB->CPACR |= (1U<<21);
	SCB->CPACR |= (1U<<22);
	SCB->CPACR |= (1U<<23);
}

void EXTI9_5_IRQHandler()
{
	if(EXTI->PR & (1U<<7))
	{
		data_ready = 1;
		EXTI->PR |= (1U<<7);
	}
}
