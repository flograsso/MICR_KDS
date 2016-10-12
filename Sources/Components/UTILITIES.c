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


/*Sort Asc*/

void sort(uint16_t *data, uint8_t length) {
	  int j, i;
	  uint16_t temp;
	  /* Bubble Method */
	  for (i = 0; i < (length - 1); i++)
	  {
	    for (j = i + 1; j < length; j++)
	    {
	      if (data[j] < data[i])
	      {
	        temp = data[j];
	        data[j] = data[i];
	        data[i] = temp;
	      }
	    }
	  }

}

/*Median calculate*/
float median (uint16_t *data, uint8_t length){

	  float result;

	  /*Sort lowest to highest*/
	  sort(data,length);

	  /*ODD*/
	  if (length%2!=0){
		  result = data[length/2];
	  }
	  /*EVEN*/
	  else
      {
          result = (data[length/2]+data[(length/2)-1])/2.0;
      }

	  return result;
}

uint16_t UTILITIES_OUTLIER_AVG(uint16_t *data,uint8_t length){

	/*Outlier Values Detect and Delete*/

    float Q1,Q2,Q3,diff;
    uint16_t upper_lim,lower_lim;

    sort(data,length);

	/*ODD*/
	if ((length/2)%2!=0){
		/*Calculate lower quartile*/
		Q1=median(data,length/2);

		/*Calculate upper quartile*/
		Q3=median(data+length/2,length/2);
	 }
	/*EVEN*/
	else{
		/*Calculate lower quartile*/
		Q1= median(data,length/2);

		/*Calculate upper quartile*/
		Q3=median(data+length/2,length/2);
	}
	/*Calculate interquartile range*/
	diff= Q3-Q1;

	/*Calculate lower and upper limits*/
	upper_lim=Q3+1.5*diff;
	lower_lim=Q1-1.5*diff;

	/*Delete outlier values evaluating data between limits*/
	for (int i = 0; i<length; i++){
		if (data[i]<lower_lim || data[i]>upper_lim){
			for(int j=i;j<length;j++){
				data[j]=data[j+1];
			}
			length=length-1;
		}
	}

	/*Calculate average*/
	uint16_t avg = 0;
	for (int i = 0; i<length; i++){
		avg=avg+data[i];
	}
	avg=avg/length;

	/*Show values non outliers*/
	/*
    uint16_t aux1[10];
    sprintf(aux1,"%d",data[i]);
	CONSOLE_SEND(aux1,strlen(aux1));
	CONSOLE_SEND("\r\n",2);
	 */

	return avg;
}
