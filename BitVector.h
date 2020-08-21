#pragma once
#include "Define.h"
//#include "particija-VS2017/part.h"
#include <vector>
using namespace std;


class BitVector
{
private:
	 
	char *bits;
	ClusterNo size;
public:
	BitVector(ClusterNo size);
	~BitVector();
	void format();
	ClusterNo findFree();
	void set(ClusterNo);
	void reset(ClusterNo);

	ClusterNo numOfClusters;
};

