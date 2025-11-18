#include "router.h"


int isThereFreeBuffer(int* buffer, int capacity) 
// function to check wether a buffer can store another packet
{
	for (int i = 0; i < capacity; i++) 
	{
		if (buffer[i] == 0) // if there is an available buffer, the program will return the index. 0 means available buffer, 1 means busy
		{
			return i;
		}

		else
		{
			continue;
		}
	}

	return -1; // if all buffers are busy, program will return -1
}

int getOutputBuffer(int xsteps, int ysteps, int xdirection, int ydirection) // function that returns a number based on where the packet must go next
{
	int out_buffr = 0; 
	// 0 = west, 1 = north, 2 = east, 3 = south for OUTPUT, 0 = east, 1 = south, 2 = west, 3 = north for INPUT
	if (xsteps!=0) { // if packet has to move xsteps
		if (xdirection == -1) // if packet has to move left, it will be delivered by west outputbuffer
		// or will be picked up by east input buffer
		{
			out_buffr = 0;
		}
		else if (xdirection == 1) // if packet has to move east, it will be delivered by east outputbuffer
		{ // or will be picked up by west input buffer
			out_buffr = 2;
		}
	}
	else if (xsteps == 0 && ysteps != 0) // if packet has finished moving in X direction but has to move in y direction
	{
		if (ydirection < 0) // if packet has to move north, it will be delivered by north output buffer, or be picked up by south input buffer
		{
			out_buffr = 1;
		}
		else if (ydirection > 0) // if packet has to move south, it will be delivered by south output buffer, or be picked up by north input buffer
		{
			out_buffr = 3;
		}
	}

	else if (xsteps == 0 && ysteps == 0) out_buffr = -1; // packet has arrived at its destination

	return out_buffr; // return the number
}

void assignBuffersInput(int j, Router r[], Packet pck[], int nextbuffer)
{ // choose buffer if packet goes from output buffer -> input buffer
			
	if (nextbuffer == 0) pck[j].bufptr = &r[pck[j].location].east_input_buffer[0];
	if (nextbuffer == 1) pck[j].bufptr = &r[pck[j].location].south_input_buffer[0];
	if (nextbuffer == 2) pck[j].bufptr = &r[pck[j].location].west_input_buffer[0];
	if (nextbuffer == 3) pck[j].bufptr = &r[pck[j].location].north_input_buffer[0];
	// bufptr pointer always points to first element of array
	// since isThereFreeBuffer always returns the first index a buffer has available,
	// we can always just assign the pointer as bufptr += index
	// the same applies to the next function
}

void assignBuffersOutput(int j, Router r[], Packet pck[], int nextbuffer)
{ // choose buffer if packet goes from input -> output buffer
	if (nextbuffer == 0) pck[j].bufptr = &r[pck[j].location].west_output_buffer[0];
	if (nextbuffer == 1) pck[j].bufptr = &r[pck[j].location].north_output_buffer[0];
	if (nextbuffer == 2) pck[j].bufptr = &r[pck[j].location].east_output_buffer[0];
	if (nextbuffer == 3) pck[j].bufptr = &r[pck[j].location].south_output_buffer[0];
}
