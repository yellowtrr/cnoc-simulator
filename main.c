#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "router.h"
#include "packet.h"
#include "simulation.h"
#include "stats.h"


void prompt() // prompt that just prints to the user instructions on how to run the program
{
	printf("\n\n");
	printf("Usage: ./[executable] [size] [buffer_capacity] [injection_rate] [timesteps] [debuginfo]\n");
	printf("size: 2D-mesh size. For example, 3 for a 3x3 2D-mesh\n");
	printf("buffer_capacity: the amount of packets a buffer can store. Max is 16\n");
	printf("injection_rate: the probability each router will create a new packet at each new timestep. Must be a float number from 0 to 1.0\n");
	printf("timesteps: total simulation steps\n");
	printf("debuginfo: 1 for extra debug info, 0 for simple readable output\n");
	printf("\n\n");
}

int main(int argc, char* argv[])
{
	FILE *fptr;
	fptr = fopen("noc_output", "w"); // file that will store the detailed simulation

	if (!fptr)
	{
		perror("Failed to open file");
		return 1;
	}
    int packetsCreated = 0;
    srand(time(NULL)); // seeding rand function
    int size, totalsteps, capacity, extradebuginfo;
    float injectionRate;
    int failedToInject = 0;

    int flag = 1;

   	if (argc < 6) // if less than 6 arguments
   	{
   		prompt();
   		return 1; // exit program, prevents segmentation fault
   	}
   	if (atoi(argv[1]) < 2) flag = 0; // check if arguments are correct, flag = 0 means there was something wrong
   	if (atoi(argv[2]) > 16 || atoi(argv[2]) < 1) flag = 0;
   	if (atof(argv[3]) > 1.0 || atof(argv[3]) < 0) flag = 0;
   	if (atoi(argv[4]) < 1) flag = 0;

   	if (flag == 0) 
   	{
   		prompt();
   		return 1; // if something in the arguments was wrong, exit the program
   	}

   	size = atoi(argv[1]); // match each argument to their variable
   	capacity = atoi(argv[2]);
   	injectionRate = atof(argv[3]);
   	totalsteps = atoi(argv[4]);
   	extradebuginfo = atoi(argv[5]);

    int totalsize = size * size; // total amount of routers. eg if size = 3 we will have a total of 9 routers

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

	unsigned int totalPacketCapacity = totalsteps * totalsize; // max amount of packets
    //Packet pck[totalPacketCapacity]; // create packets
    Packet* pck = malloc(sizeof(Packet) * totalPacketCapacity);
    if (pck == NULL)
    {
    	perror("malloc error");
    	return 1;
    }

    // Initialize packets
    for (unsigned int i = 0; i < totalPacketCapacity; ++i)
        pck[i] = (Packet){0};

	for (int t = 0; t < totalsteps; t++)
	{
		// at each timestep, first inject new packets
		injectPacket(t, r, pck, size, totalsize, capacity, &packetsCreated, injectionRate, fptr, &failedToInject);

		// then move the already existing ones
		movePackets(r, pck, size, packetsCreated, capacity, fptr);

		if (extradebuginfo) // only runs if user wants extra debug info based on the arguments he used
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
	printf("That equals a success rate of %.2f\n", 100* ((float)getTotalDelivered(pck, packetsCreated) / packetsCreated));	

	printf("Average latency is: %.2f\n", calculateAverageLatency(pck, packetsCreated));
	printf("Max latency is: %d\n", calculateMaxLatency(pck, packetsCreated));

	printf("Out of total packets injected, %d have experienced stall\n", getTotalStall(pck, packetsCreated));
	printf("In total, %d packets have failed to be injected\n", failedToInject);

	printf("To see the detailed simulation, check noc_output file\n");

	free(pck);
	
	return 0;
}
