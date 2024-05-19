#include "algorithm.h"


uint32_t ADCVolt_fliter(uint32_t* arr,int length)
{
	uint32_t max = arr[0];
	uint32_t min = arr[0];
	uint32_t sum = arr[0];
	
	for (int i = 1; i < length; i++)
	{
		if (arr[i] > max)
		{
			max = arr[i];
		}
		else if (arr[i] < min)
		{
			min = arr[i];
		}

		sum += arr[i];
	}

	return (sum - max - min) / (length - 2);
}