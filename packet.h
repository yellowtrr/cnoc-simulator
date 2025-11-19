#ifndef PACKET_H
#define PACKET_H

#include <stdio.h>

typedef struct
{
	int id;
	int start_point;
	int finish_point;
	int location;
	int previouslocation;
	int xsteps;
	int ysteps;
	int xdir;
	int ydir;
	int hops;
	double latency;
	int stall;
	int delivered;
	int isininputbuffer;
	int outputbuffer;
	int* bufptr;
	int* prevbufptr;
}Packet;

int calculatexsteps(int start, int finish, int dimention, int* xdirection);

int calculateysteps(int start, int finish, int dimention, int* ydirection);


 
#endif
