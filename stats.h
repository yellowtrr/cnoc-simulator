#ifndef STATS_H
#define STATS_H

#include "packet.h"

double calculateAverageLatency(Packet* pack, int totalpackets);
int calculateMaxLatency(Packet* pack, int totalpackets);
int getTotalDelivered(Packet* pack, int totalpackets);
int getTotalStall(Packet* pack, int totalpackets);

#endif
