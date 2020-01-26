#pragma once
#include "Define.h"

class BitVector
{
private:
	char * bitvect;

public:
	BitVector();
	~BitVector();
	void format();
	void clearBit();
};

