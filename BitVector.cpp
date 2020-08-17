#include "BitVector.h"



BitVector::BitVector(ClusterNo size)
{
	this->size = size;
	if (size % ClusterSize == 0) {
		this->numOfClusters = size / ClusterSize;
	}
	else { //+1
		this->numOfClusters = size / ClusterSize + 1; 
	}

	bits = new char[(this->numOfClusters * ClusterSize) / sizeof(char) ];


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

ClusterNo BitVector::findFree()
{
	for (ClusterNo index = 0; index < this->numOfClusters * ClusterSize / sizeof(char); index++)
	{
		if (bits[index] != 0xffU) {
			for (int i = 7; i > 0; i--) {
				char byte = bits[index];
				//ClusterNo id = i * sizeof(char);
				char mask = 1U << i; // 00000001 << 6 01000000  10010010
				if (byte & mask == 0) { //00000000
					ClusterNo id = index * sizeof(char) + (7 - i);
					set(id);
					return id;
				}
			}
		}
	}
}



void BitVector::set(ClusterNo id) {
	//TODO
	ClusterNo index = id / sizeof(char);
	char byte = bits[index];
	ClusterNo mod = id % sizeof(char);
	char mask = 1U << (7-mod); 
	bits[index] = byte | mask;
}


void BitVector::reset(ClusterNo id) {
	//TODO sinh
	ClusterNo index = id / sizeof(char);
	char byte = bits[index];
	ClusterNo mod = id % sizeof(char);
	char mask = 1U << (7 - mod);
	mask = ~mask;
	bits[index] = byte & mask;
}