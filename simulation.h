#ifndef SIMULATION_H
#define SIMULATION_H
#include "router.h"
#include "packet.h"

int injectPacket(int t, Router r[], Packet pck[], int size, int totalsize, int capacity, int* packetsCreated, float injectionRate, FILE* fptr, int* failedToInject);
int movePackets(Router r[], Packet pck[], int size, int packetsCreated, int capacity, FILE* fptr);

#endif
