/*
 * fir.h
 *
 *  Created on: May 12 2023
 *      Author: Marcin Rynkiewicz
 */

#ifndef FIR_H_
#define FIR_H_

#include <stdint.h>

#define FIR_FILTER_LENGTH 16

typedef struct
{
	float buf[FIR_FILTER_LENGTH];
	uint8_t bufIndex;

	float out;
} FIRFilter;

void FIRFilter_Init(FIRFilter* fir);
float FIRFilter_Updater(FIRFilter *fir, float inp);

#endif /* FIR_H_ */
