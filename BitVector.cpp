#include "BitVector.h"

void BitVector::format() {
	for (int i = 0; i < ClusterSize; i++) {
		bitvect[i] = 0;
	}
}


void BitVector::clearBit() {

}