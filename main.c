#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "router.h"
#include "packet.h"
#include "stats.h"
#include "mainfunctions.h"


void assignBuffersInput(int j, Router r[], Packet pck[], int nextbuffer)
{ // choose buffer if packet goes from output buffer -> input buffer
			
	if (nextbuffer == 0) pck[j].bufptr = &r[pck[j].location].east_input_buffer[0];
	if (nextbuffer == 1) pck[j].bufptr = &r[pck[j].location].south_input_buffer[0];
	if (nextbuffer == 2) pck[j].bufptr = &r[pck[j].location].west_input_buffer[0];
	if (nextbuffer == 3) pck[j].bufptr = &r[pck[j].location].north_input_buffer[0];
	// bufptr pointer always points to first element of array
}

void assignBuffersOutput(int j, Router r[], Packet pck[], int nextbuffer)
{ // choose buffer if packet goes from input -> output buffer
	if (nextbuffer == 0) pck[j].bufptr = &r[pck[j].location].west_output_buffer[0];
	if (nextbuffer == 1) pck[j].bufptr = &r[pck[j].location].north_output_buffer[0];
	if (nextbuffer == 2) pck[j].bufptr = &r[pck[j].location].east_output_buffer[0];
	if (nextbuffer == 3) pck[j].bufptr = &r[pck[j].location].south_output_buffer[0];
}

int main()
{
	FILE *fptr;
	fptr = fopen("noc_output", "w"); // file that will store the detailed simulation
    int packetsCreated = 0;
    srand(time(NULL)); // seeding rand function
    int size, totalsteps, capacity, extradebuginfo;
    float injectionRate;
    printf("Enter size (eg type 3 for 3x3 size): ");
    scanf("%d", &size);

    int totalsize = size * size; // if size = 3, there will be a total of 9 routers

	printf("Enter capacity of each buffer (eg 3 packets): ");
	scanf("%d", &capacity);

	printf("Enter injection rate(min = 0, max = 1.0): ");
	scanf("%f", &injectionRate);

    printf("Enter total steps: ");
    scanf("%d", &totalsteps);

    printf("\n");

    printf("Do you want extra debugging info?\n");
    printf("This includes an array at each timestep that marks each buffer as free (0) or busy (1)\n");
    printf("Type 0 if you do not wish to have extra debugging info, or 1 if you do: ");
    scanf("%d", &extradebuginfo);

    Router r[totalsize]; // create totalsize routers

	for (int i = 0; i < totalsize; i++) {
		for (int k = 0; k < MAX_BUFFER_CAPACITY; k++) {
			r[i].north_input_buffer[k]  = 0;
            r[i].east_input_buffer[k]   = 0;
            r[i].south_input_buffer[k]  = 0;
            r[i].west_input_buffer[k]   = 0;
            r[i].north_output_buffer[k] = 0;
            r[i].east_output_buffer[k]  = 0;
            r[i].south_output_buffer[k] = 0;
            r[i].west_output_buffer[k]  = 0;
		}
	} // initialize all the routers and their buffers

	int totalPacketCapacity = totalsteps * totalsize; // max amount of packets
    Packet pck[totalPacketCapacity]; 

    // Initialize packets
    for (int i = 0; i < totalPacketCapacity; ++i)
        pck[i] = (Packet){0};

	for (int t = 0; t < totalsteps; t++)
	{
		// at each timestep, first inject new packets
		injectPacket(t, r, pck, size, totalsize, capacity, &packetsCreated, injectionRate, fptr);

		// then move the already existing ones
		movePackets(r, pck, size, packetsCreated, capacity, fptr);

		if (extradebuginfo)
		{
			for (int k = 0; k < totalsize; k++)
			{
				for (int h = 0; h < capacity; h++)
				{
					// extra debug info to view simulation in better detail
					fprintf(fptr, "Router %d, slot %d | NI: %d, EI: %d, SI: %d, WI: %d, NO: %d, EO: %d, SO: %d, WO: %d\n", k, h, r[k].north_input_buffer[h],
					r[k].east_input_buffer[h], r[k].south_input_buffer[h], r[k].west_input_buffer[h],
					r[k].north_output_buffer[h], r[k].east_output_buffer[h], r[k].south_output_buffer[h],
					r[k].west_output_buffer[h]);
				}	
			}
		}
	}

	printf("STATS-----------------------------------------------------\n");
	printf("In total, %d packets have been injected\n", packetsCreated);
	printf("Out of those packets, %d have been succesfully delivered\n", getTotalDelivered(pck, packetsCreated));
	printf("That equals a success rate of %.2f\n", (float)getTotalDelivered(pck, packetsCreated) / packetsCreated);	

	printf("Average latency is: %.2f\n", calculateAverageLatency(pck, packetsCreated));
	printf("Max latency is: %d\n", calculateMaxLatency(pck, packetsCreated));

	printf("To see the detailed simulation, check noc_output file\n");
	
	return 0;
}
