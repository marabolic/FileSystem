#pragma once
#include "particija-VS2017/part.h"
#include <stdint.h>

#define INDEX_SIZE ClusterSize / sizeof(ClusterNo)
#define DATA_SIZE ClusterSize / sizeof(HeaderFields)

#define signal(x) ReleaseSemaphore(x,1,NULL)
#define wait(x) WaitForSingleObject(x,INFINITE)

//typedef char byte;

enum Mode { READONLY = 0, WRITEONLY = 1, READANDWRITE = 2 };

struct HeaderFields {
	char name[8];
	char extension[3];
	char nothing;
	uint32_t ind1;
	char fileSize[4];
	char freeBytes[12];
};



