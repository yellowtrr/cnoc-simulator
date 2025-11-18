#ifndef PACKET_H
#define PACKET_H

#include <stdio.h>
#include "router.h"

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
	int latency;
	int delivered;
	int isininputbuffer;
	int outputbuffer;
	int* bufptr;
	int* prevbufptr;
}Packet;

int calculatexsteps(int start, int finish, int dimention, int* xdirection);

int calculateysteps(int start, int finish, int dimention, int* ydirection);

int getOutputBuffer(int xsteps, int ysteps, int xdirection, int ydirection);

int injectPacket(int t, Router r[], Packet pck[], int size, int totalsize, int capacity,
int* packetsCreated, float injectionRate, FILE* fptr);

int movePackets(Router r[], Packet pck[], int size, int packetsCreated, int capacity,
 FILE* fptr);
 
#endif
