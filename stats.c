#include "stats.h"

double calculateAverageLatency(Packet* pack, int totalpackets)
{
	double totalLatency = 0;
	double totalDelivered = 0;

	for (int i = 0; i < totalpackets; i++)
	{
		if (pack[i].delivered)
		{
			totalLatency += pack[i].latency;
			totalDelivered += 1.0;
		}
	}

	return totalLatency / totalDelivered;
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

int getTotalStall(Packet* pack, int totalpackets)
{
	int result = 0;
	for (int i = 0; i < totalpackets; i++)
	{
		if (pack[i].stall == 1) result++;
	}

	return result;
}

