#pragma once
#include "Define.h"
#include "KernelFS.h"
#include <vector>
using namespace std;

class BitVector
{
private:
	 
	char *bits;
	ClusterNo size, numOfClusters;
public:
	BitVector(ClusterNo size);
	~BitVector();
	ClusterNo findFree();
	void set(ClusterNo);
	void reset(ClusterNo);
};

