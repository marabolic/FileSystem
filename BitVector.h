#pragma once
#include "Define.h"

class BitVector
{
private:
	byte bitvect[ClusterSize];

public:
	void format();
	void clearBit();
};

