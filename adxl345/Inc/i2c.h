/*
 * i2c.h
 *
 *  Created on: May 7, 2023
 *      Author: Marcin Rynkiewicz
 */

#ifndef I2C_H_
#define I2C_H_

void I2C1_Init();
void I2C_byteRead(char saddr, char maddr, char* data);
void I2C_burstRead(char saddr, char maddr, int n, char* data);
void I2C_burstWrite(char saddr, char maddr, int n, char* data);

#endif /* I2C_H_ */
