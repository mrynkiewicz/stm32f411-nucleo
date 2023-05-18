/*
 * adxl345.h
 *
 *  Created on: May 7, 2023
 *  Author: Marcin Rynkiewicz
 */

#ifndef ADXL345_H_
#define ADXL345_H_

#include <stdint.h>
#include "i2c.h"


// All addresses and configuration values taken from
// adxl345 datasheet
#define   DEVID_R					(0x00)
#define   DEVICE_ADDR    			(0x53)
#define   DATA_FORMAT_R   			(0x31)
#define   POWER_CTL_R 				(0x2D)
#define   DATA_START_ADDR			(0x32)
#define   DATA_FORMAT_R   			(0x31)
#define   INT_ENABLE				(0x2E)
#define   INT_MAP					(0x2F)


#define		FOUR_G					(0x01)
#define		RESET					(0x00)
#define		SET_MEASURE_B			(0x08)


void adxl_init();
void adxl_read_address(uint8_t reg);
void adxl_write(uint8_t reg, char value);
void adxl_read_values(uint8_t reg);

#endif /* ADXL345_H_ */
