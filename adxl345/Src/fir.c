/*
 * fir.c
 *
 *  Created on: May 12 2023
 *      Author: Marcin Rynkiewicz
 */

#include "fir.h"

// coeficients calculated using Phil's Solomony tool
static float FIR_IMPULSE_RESPONSE[FIR_FILTER_LENGTH] =
	{-0.0032906f,-0.0052635f,-0.0068811f,0.0000000f,0.0254209f,0.0724719f,0.1311260f,0.1805961f,0.2000000f,0.1805961f,0.1311260f,0.0724719f,0.0254209f,0.0000000f,-0.0068811f,-0.0052635f};

void FIRFilter_Init(FIRFilter *fir)
{
	for (uint8_t i = 0; i < FIR_FILTER_LENGTH; i++)
	{
		fir->buf[i] = 0.0f;
	}

	fir->bufIndex = 0;

	fir->out = 0.0f;
}

float FIRFilter_Updater(FIRFilter *fir, float inp)
{

	fir->buf[fir->bufIndex] = inp;

	fir->bufIndex++;

	if (fir->bufIndex == FIR_FILTER_LENGTH)
	{
		fir->bufIndex = 0;
	}

	fir->out = 0.0f;

	uint8_t sumIndex = fir->bufIndex;

	for(uint8_t i = 0; i < FIR_FILTER_LENGTH; i++)
	{
		if (sumIndex > 0)
		{
			sumIndex--;
		}
		else
		{
			sumIndex = FIR_FILTER_LENGTH - 1;
		}

		fir->out += FIR_IMPULSE_RESPONSE[i] * fir->buf[sumIndex];
	}

	return fir->out;
}
