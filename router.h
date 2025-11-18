#ifndef ROUTER_H
#define ROUTER_H

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

#endif
