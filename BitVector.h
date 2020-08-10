#pragma once
#include "Define.h"
#include <vector>
using namespace std;

class BitVector
{
private:

	BitVector* next;

public:
	BitVector();
	~BitVector();
	void format();
	void clearBit();
	int size();
	void init();
};

