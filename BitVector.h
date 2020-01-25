#pragma once
#include "Define.h"

class BitVector
{
private: 
	byte bitvect[ClusterSize];

public:
	 void format() {
		for (int i = 0; i < ClusterSize; i++) {
			bitvect[i] = 0;
		}
	 } 

	void clearBit() {

	}
};

