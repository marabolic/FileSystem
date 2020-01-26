#pragma once
#include "particija-VS2017/part.h"

#define INDEX_SIZE ClusterSize / sizeof(ClusterNo)
#define DATA_SIZE ClusterSize / sizeof(HeaderFields)

#define signal(x) ReleaseSemaphore(x,1,NULL)
#define wait(x) WaitForSingleObject(x,INFINITE)

typedef char byte;

enum Mode { READONLY = 0, WRITEONLY = 1, READANDWRITE = 2 };

struct HeaderFields {
	byte name[8];
	byte extension[3];
	byte nothing;
	byte ind1[4];
	byte fileSize[4];
	byte freeBytes[12];
};



