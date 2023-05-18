/*
 * adxsl345.c
 *
 *  Created on: May 7, 2023
 *  Author: Marcin Rynkiewicz
 */

#include "adxl345.h"

char data;
uint8_t data_values[6];

void adxl_read_address(uint8_t reg)
{
	I2C_byteRead(DEVICE_ADDR, reg, &data);
}

void adxl_write(uint8_t reg, char value)
{
	char data[1];
	data[0] = value;

	I2C_burstWrite(DEVICE_ADDR, reg, 1, data);
}

void adxl_read_values(uint8_t reg)
{
	I2C_burstRead(DEVICE_ADDR, reg, 6, (char*)data_values);
}


void adxl_init()
{
	I2C1_Init();

	adxl_read_address(DEVID_R);

	adxl_write(DATA_FORMAT_R, FOUR_G);

	// disable interrupts
	adxl_write(INT_ENABLE, 0x00);

	// setting all interrupts to int1
	adxl_write(INT_MAP, 0x00);

	// enable DATA_READ int
	adxl_write(INT_ENABLE, 0x80);

	adxl_write(POWER_CTL_R, RESET);

	adxl_write(POWER_CTL_R, SET_MEASURE_B);

}
