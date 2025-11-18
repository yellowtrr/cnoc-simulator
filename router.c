#include "router.h"

int isThereFreeBuffer(int* buffer, int capacity) 
// function to check wether a buffer can store another packet
{
	for (int i = 0; i < capacity; i++)
	{
		if (buffer[i] == 0) // if there is an available buffer, the program will return the index
		{
			return i;
		}

		else
		{
			continue;
		}
	}

	return -1;
}
