#include "simulation.h"
#include <stdio.h>
#include <stdlib.h>
#include "router.h"
#include "packet.h"


int injectPacket(int t, Router r[], Packet pck[], int size,
int totalsize, int capacity, int* packetsCreated, float injectionRate, FILE* fptr, int* failedToInject)
{ // function to inject new packet
	fprintf(fptr, "-----------------------\n");
	fprintf(fptr, "Timestep: %d\n", t);

	for (int l = 0; l < totalsize; l++) // this ensures it will pick every single router 
	{
		float choice = (float)rand() / (float)RAND_MAX; // pick a floating number between 0 and 1

		if (choice <= injectionRate) // if the random number is smaller than our injectionrate, a packet gets injected
		{
			int a = l;
			int b = rand() % totalsize; // b will act as a finish point, a as a start point

			int indx = *packetsCreated; // store value of *packetsCreated
			*packetsCreated = *packetsCreated + 1; // increment whenever we create a new packet

			int nf = isThereFreeBuffer(r[a].north_input_buffer, capacity);
			int ef = isThereFreeBuffer(r[a].east_input_buffer, capacity);
			int sf = isThereFreeBuffer(r[a].south_input_buffer, capacity);
			int wf = isThereFreeBuffer(r[a].west_input_buffer, capacity);
			// check if there is buffer available to store the packet in selected router

			if (nf == -1 && ef == -1 && sf == -1 && wf == -1) // if all buffers are busy
			{
				fprintf(fptr, "Injection failed due to stall at %d router\n", a);
				*failedToInject = *failedToInject + 1;
				continue;
			} else { // if there is room for the packet
		
				pck[indx].start_point = a; // initialize its values
				pck[indx].finish_point = b;
				pck[indx].id = indx;
				pck[indx].location = a; // current location of packet
				pck[indx].isininputbuffer = 1; // packet first starts at an input buffer
		
				if (nf != -1) pck[indx].bufptr = &r[pck[indx].location].north_input_buffer[nf];
				if (ef != -1) pck[indx].bufptr = &r[pck[indx].location].east_input_buffer[ef];
				if (sf != -1) pck[indx].bufptr = &r[pck[indx].location].south_input_buffer[sf];
				if (wf != -1) pck[indx].bufptr = &r[pck[indx].location].west_input_buffer[wf];
				// update bufptr pointer to point at buffer where packet is stored
		
				*pck[indx].bufptr = 1; // mark that buffer as BUSY
				pck[indx].prevbufptr = pck[indx].bufptr; // prevbufptr stores the same value
				// prevbufptr acts as a checkpoint, in case packet cant move to next buffer
		
		
				if (a == b) // if start point = end point
				{
					pck[indx].delivered = 1; // mark as delivered
					*pck[indx].bufptr = 0; // mark its buffer as AVAILABLE
				}
		
		
				pck[indx].xsteps = calculatexsteps(a, b, size, &pck[indx].xdir); 
				pck[indx].ysteps = calculateysteps(a, b, size, &pck[indx].ydir);
				// store how many steps packet has to move in each direction
		
				fprintf(fptr, "Created packet with id: %d, start point: %d, end point: %d\n", pck[indx].id, a, b);		
			}
		}

		else continue;

		
	}
	return 0;
}

int movePackets(Router r[], Packet pck[], int size, int packetsCreated, int capacity, FILE* fptr)
{
	for (int j = 0; j < packetsCreated; j++) // for every packet that has been created
	{
		pck[j].xsteps = calculatexsteps(pck[j].location, pck[j].finish_point, size, &pck[j].xdir);
		pck[j].ysteps = calculateysteps(pck[j].location, pck[j].finish_point, size, &pck[j].ydir);
		// calculate how many steps remaining
		pck[j].outputbuffer = getOutputBuffer(pck[j].xsteps, pck[j].ysteps, pck[j].xdir, pck[j].ydir);
		// calculate where packet has to move next
		int nextbuffer = pck[j].outputbuffer;
		// store that into a variable

		if (!pck[j].isininputbuffer) // if packet is currently occupying an OUTPUT buffer
		{

			if (pck[j].delivered) continue; // if packet has already been delivered, ignore it


			if (pck[j].xsteps > 0) // if packet has to move xsteps
			{
	
				pck[j].previouslocation = pck[j].location; // save initial location
				pck[j].location = pck[j].location + pck[j].xdir; // move to new location

									
			}
			else if (pck[j].ysteps > 0) // if packet has to move ysteps
			{

			pck[j].previouslocation = pck[j].location; // save initial location
			pck[j].location = pck[j].location + pck[j].ydir; // move to new location
									

			}
			
			else if (pck[j].xsteps == 0 && pck[j].ysteps == 0) // if packet arrived at final destination
			{
				pck[j].delivered = 1; // mark as delivered
			
				if (pck[j].bufptr != NULL) *pck[j].bufptr = 0; 
				if (pck[j].prevbufptr != NULL) *pck[j].prevbufptr = 0;
				// mark its buffers as AVAILABLE
				continue;
			}

			if (pck[j].bufptr != NULL) pck[j].prevbufptr = pck[j].bufptr; // save pointer
			assignBuffersInput(j, r, pck, nextbuffer); // assign the pointers to the correct buffer location in memory, index 0

			pck[j].latency += 1;
			int index = isThereFreeBuffer(pck[j].bufptr, capacity); // function returns the index that is AVAILABLE

			if (index != -1) // if there is available buffer
			{
				pck[j].bufptr += index; // bufptr points at the specific location of the available buffer

				fprintf(fptr, "Packet ID: %d moves from previous location : (%d) to location: (%d)\n",
				 pck[j].id, pck[j].previouslocation, pck[j].location);
				*pck[j].bufptr = 1; // mark buffer as BUSY
				pck[j].isininputbuffer = 1; // packet moves to input buffer
				if (pck[j].prevbufptr != NULL) *pck[j].prevbufptr = 0; // mark previous buffer as AVAILABLE

				if (pck[j].xsteps>0) pck[j].xsteps = pck[j].xsteps - 1; // decrease xsteps left
				else if (pck[j].ysteps>0) pck[j].ysteps = pck[j].ysteps - 1; // decrease ysteps left
				
			}
			
			else if (index == -1) // if no buffer available
			{
				fprintf(fptr, "ID: %d We have stall. Added latency\n", pck[j].id);
				pck[j].location = pck[j].previouslocation; // packet can not move to next location
				pck[j].stall = 1;
			}
		}

		else if (pck[j].isininputbuffer) // if packet is in input buffer
		{
			if (pck[j].delivered) continue; // if packet has already been delivered, ignore it

			if (pck[j].location == pck[j].finish_point) // if packet just arrived at final destination
			{
				pck[j].delivered = 1; // mark as delivered
				fprintf(fptr, "Packet ID: %d succesfully delivered after %d latency!\n", pck[j].id,
				pck[j].latency);
				*pck[j].bufptr = 0; // mark its buffer as AVAILABLE
				continue; // move on to next packet
			}

			if (pck[j].bufptr != NULL) pck[j].prevbufptr = pck[j].bufptr; // store bufptr address

			assignBuffersOutput(j, r, pck, nextbuffer);
			pck[j].latency += 1; 

			int index = isThereFreeBuffer(pck[j].bufptr, capacity);
			if (index != -1) // if there is available buffer
			{
				pck[j].bufptr += index; // bufptr points at address of available buffer
				fprintf(fptr, "Packet ID: %d moves from input buffer to output buffer at %d\n",
				pck[j].id, pck[j].location);
				*pck[j].bufptr = 1; // mark buffer as BUSY
				pck[j].isininputbuffer = 0; // packet moves to output BUFFER 
				if (pck[j].prevbufptr != NULL) *pck[j].prevbufptr = 0; // mark previous buffer as available
			}

			else if (index == -1) // if no buffer available
			{
				fprintf(fptr, "ID: %d We have stall. Added latency.\n", pck[j].id);
				pck[j].stall = 1;
			}

			
		}
	}

	return 0;
}
