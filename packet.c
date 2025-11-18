#include <stdlib.h>
#include "packet.h"

int calculatexsteps(int start, int finish, int dimention, int* xdirection)
{
	// function to calculate how many steps a packet has to move in X direction
	// we pass xdirection as a pointer, so that the function can change xdirection value
	int temp = start % dimention; 
	int temp2 = finish % dimention;

	int result = abs(temp2 - temp);

	// in xdirection, nearby routers are seperated by their modulo operation with dimention(size)
	// for example router 0 and router 1 in 3x3 are seperated by %, because 0%3=3 and 1%3=2

	if (temp2 > temp) *xdirection = 1; // this simply means packet has to move right
	else *xdirection = -1; // packet has to move left

	return result;
}

int calculateysteps(int start, int finish, int dimention, int* ydirection)
{
	// function to calculate how many steps a packet has to move in Y direction
	// we pass ydirection as a pointer, so that the function can change ydirection value

	int temp = start / dimention;
	int temp2 = finish / dimention;

	// in ydirection, nearby routers are seperated by integer division with dimention(size)
	// for example router 0 and 3 in 3x3 are seperated by /, beacuse 0/3=0, 3/3=1

	int result = abs(temp2 - temp);

	if (temp2 > temp) *ydirection = dimention; // packet has to move down
	else *ydirection = -dimention; // packet has to move up


	return result;
}



