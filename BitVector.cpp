#include "BitVector.h"



BitVector::BitVector()
{
	bitvect = new char[ClusterSize];
}

BitVector::~BitVector()
{
	delete bitvect;
}

void BitVector::format() {
	for (int i = 0; i < ClusterSize; i++) {
		bitvect[i] = 0;
	}
}


void BitVector::clearBit() {

}