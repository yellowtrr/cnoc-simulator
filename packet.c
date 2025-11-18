#include <stdlib.h>
#include "packet.h"
#include "mainfunctions.h"

int calculatexsteps(int start, int finish, int dimention, int* xdirection)
{
	// function to calculate how many steps a packet has to move in X direction
	// we pass xdirection as a pointer, so that the function can change xdirection value
	int temp = start % dimention;
	int temp2 = finish % dimention;

	int result = abs(temp2 - temp);

	if (temp2 > temp) *xdirection = 1; // this simply means packet has to move right
	else *xdirection = -1; // packet has to move left

	return result;
}

int calculateysteps(int start, int finish, int dimention, int* ydirection)
{
	// function to calculate how many steps a packet has to move in Y direction

	int temp = start / dimention;
	int temp2 = finish / dimention;

	int result = abs(temp2 - temp);

	if (temp2 > temp) *ydirection = dimention; // packet has to move down
	else *ydirection = -dimention; // packet has to move up


	return result;
}

int getOutputBuffer(int xsteps, int ysteps, int xdirection, int ydirection)
{
	int out_buffr = 0; 
	// 0 = west, 1 = north, 2 = east, 3 = south for OUTPUT, 0 = east, 1 = south, 2 = west, 3 = north for INPUT
	if (xsteps!=0) { // if packet has to move xsteps
		if (xdirection == -1) // if packet has to move left, it will be delivered by west outputbuffer
		// or will be picked up by east input buffer
		{
			out_buffr = 0;
		}
		else if (xdirection == 1) // if packet has to move east, it will be delivered by east outputbuffer
		{
			out_buffr = 2;
		}
	}
	else if (xsteps == 0 && ysteps != 0) // if packet has finished moving in X direction but has to move in y direction
	{
		if (ydirection < 0) // if packet has to move north, it will be delivered by north buffer
		{
			out_buffr = 1;
		}
		else if (ydirection > 0) // if packet has to move south, it will be delivered by south buffer
		{
			out_buffr = 3;
		}
	}

	else if (xsteps == 0 && ysteps == 0) out_buffr = -1; // packet has arrived at its destination

	return out_buffr;
}

int injectPacket(int t, Router r[], Packet pck[], int size,
int totalsize, int capacity, int* packetsCreated, float injectionRate, FILE* fptr)
{ // function to inject new packet
	fprintf(fptr, "-----------------------\n");
	fprintf(fptr, "Timestep: %d\n", t);

	for (int l = 0; l < totalsize; l++)
	{
		float choice = (float)rand() / (float)RAND_MAX; // pick a number between 0 and 1

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
				return 0;
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
	for (int j = 0; j < packetsCreated; j++)
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
			assignBuffersInput(j, r, pck, nextbuffer); 

			pck[j].latency += 1;
			int index = isThereFreeBuffer(pck[j].bufptr, capacity); // function returns the index that is AVAILABLE

			if (index != -1) // if there is available buffer
			{
				pck[j].bufptr += index; // bufptr points at the location of the available buffer

				fprintf(fptr, "Packet ID: %dmoves from previous location : (%d) to location: (%d)\n",
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
			}

			
		}
	}

	return 0;
}

