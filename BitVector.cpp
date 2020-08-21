#include "BitVector.h"
#include "KernelFS.h"

#define CHAR_SIZE 8


BitVector::BitVector(ClusterNo size)
{
	this->size= size;
	if (size % (ClusterSize * CHAR_SIZE) == 0) {
		this->numOfClusters = size / (ClusterSize * CHAR_SIZE);
	}
	else { //+1
		this->numOfClusters = size / (ClusterSize * CHAR_SIZE) + 1;
	}

	bits = new char[(this->numOfClusters * ClusterSize)];


	for (ClusterNo i = 0; i < this->numOfClusters; i++)
	{
		//TODO
		EnterCriticalSection(&KernelFS::cs);
		KernelFS::mountedPart->readCluster(i, bits + ClusterSize * i);
  		LeaveCriticalSection(&KernelFS::cs);
	}
	

}

BitVector::~BitVector() 
{
	for (ClusterNo i = 0; i < this->numOfClusters; i++)
	{
		//TODO
		EnterCriticalSection(&KernelFS::cs); 
		KernelFS::mountedPart->writeCluster(i, bits + ClusterSize * i);
		LeaveCriticalSection(&KernelFS::cs);
	}
}

void BitVector::format() {
	int size = (this->numOfClusters * ClusterSize) / CHAR_SIZE;
	memset(bits, 0, size);


	for (ClusterNo i = 0; i < numOfClusters; i++) {
		set(i);
	}
	
}



ClusterNo BitVector::findFree()
{
	EnterCriticalSection(&KernelFS::cs);
	for (ClusterNo index = 0; index < this->numOfClusters * ClusterSize; index++)
	{
		if (bits[index] != 0xffU) {
			for (int i = 7; i >= 0; i--) {
				char byte = bits[index];
				//ClusterNo id = i * sizeof(char);
				char mask = 1U << i; // 00000001 << 6 01000000  10010010
				if ((byte & mask) == 0) { //00000000
					ClusterNo id = index * CHAR_SIZE + (7 - i);
					set(id);
					LeaveCriticalSection(&KernelFS::cs);
					return id;
				}
			}
		}
	}
	LeaveCriticalSection(&KernelFS::cs);
	return -1;
}



void BitVector::set(ClusterNo id) {
	EnterCriticalSection(&KernelFS::cs);
	ClusterNo index = id / CHAR_SIZE;
	char byte = bits[index];
	ClusterNo mod = id % CHAR_SIZE;
	char mask = 1U << (7-mod); 
	bits[index] = byte | mask;
	LeaveCriticalSection(&KernelFS::cs);
}


void BitVector::reset(ClusterNo id) {
	EnterCriticalSection(&KernelFS::cs);
	ClusterNo index = id / CHAR_SIZE;
	char byte = bits[index];
	ClusterNo mod = id % CHAR_SIZE;
	char mask = 1U << (7 - mod);
	mask = ~mask;
	bits[index] = byte & mask;
	LeaveCriticalSection(&KernelFS::cs);
}


