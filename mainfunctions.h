#ifndef MAINFUNCTIONS_H
#define MAINFUNCTIONS_H

#include "router.h"
#include "packet.h"

void assignBuffersInput(int j, Router r[], Packet pck[], int nextbuffer);
void assignBuffersOutput(int j, Router r[], Packet pck[], int nextbuffer);

#endif
