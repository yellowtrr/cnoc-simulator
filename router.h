#ifndef ROUTER_H
#define ROUTER_H

#include "packet.h"
#define MAX_BUFFER_CAPACITY 16
typedef struct
{
	int north_output_buffer[MAX_BUFFER_CAPACITY];
	int east_output_buffer[MAX_BUFFER_CAPACITY];
	int south_output_buffer[MAX_BUFFER_CAPACITY];
	int west_output_buffer[MAX_BUFFER_CAPACITY];
	
	int north_input_buffer[MAX_BUFFER_CAPACITY];
	int east_input_buffer[MAX_BUFFER_CAPACITY];
	int south_input_buffer[MAX_BUFFER_CAPACITY];
	int west_input_buffer[MAX_BUFFER_CAPACITY];
}Router;

int isThereFreeBuffer(int* buffer, int capacity);

int getOutputBuffer(int xsteps, int ysteps, int xdirection, int ydirection);

void assignBuffersInput(int j, Router r[], Packet pck[], int nextbuffer);

void assignBuffersOutput(int j, Router r[], Packet pck[], int nextbuffer);

#endif
