/**
 *
 *
 * @file UTILITIES.c
 * @author Valentin Korenblit
 * @date March, 2016
 * @brief File containing utilities for strings
 *
 *
 */

#include "UTILITIES.h"

uint8_t UTILITIES_IS_NUMBER(uint8_t *buffer)
{
	return (( (*(buffer))>=48) && ((*(buffer)) <=57));

}

void UTILITIES_FLUSH_HTTP_BUFFER(uint8_t *buffer, uint32_t length)
{
	uint32_t i;

	for(i=0; i < length; i++)
	{
		buffer[i] = '\0';
	}

}

void UTILITIES_FLOAT_TO_STR(float *f,uint8_t *buffer, uint8_t decimals)
{
	int integer_part;
	int fractional_part;
	float f_aux;

	f_aux=(*f);

	integer_part = (int) f_aux;
	f_aux = f_aux - integer_part;
	fractional_part = (int) (f_aux*100);
	sprintf(buffer,"%d.%d",integer_part,fractional_part);
}

