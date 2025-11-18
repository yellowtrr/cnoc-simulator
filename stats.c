#include "stats.h"

float calculateAverageLatency(Packet* pack, int totalpackets)
{
	float totalLatency = 0;
	float totalDelivered = 0;

	for (int i = 0; i < totalpackets; i++)
	{
		if (pack[i].delivered)
		{
			totalLatency += pack[i].latency;
			totalDelivered += 1;
		}
	}

	return totalLatency/totalDelivered;
}

int calculateMaxLatency(Packet* pack, int totalpackets)
{
	int maxLatency = 0;

	for (int i = 0; i < totalpackets; i++)
	{
		if (pack[i].delivered)
		{
			if (pack[i].latency > maxLatency)
			{
				maxLatency = pack[i].latency;
			}
		}
	}

	return maxLatency;
}

int getTotalDelivered(Packet* pack, int totalpackets)
{
	int packetsDelivered = 0;
	
	for (int i = 0; i < totalpackets; i++)
	{
		if (pack[i].delivered) packetsDelivered += 1;
	}

	return packetsDelivered;
}

