#pragma once
#include "particija-VS2017/part.h"
#include <stdint.h>

#define INDEX_SIZE ClusterSize / sizeof(ClusterNo)   //256
#define DATA_SIZE ClusterSize / sizeof(HeaderFields) //64

#define signal(x) ReleaseSemaphore(x,1,NULL)
#define wait(x) WaitForSingleObject(x,INFINITE)

//typedef char byte;

enum Mode { READONLY = 0, WRITEONLY = 1, READANDWRITE = 2 };

struct HeaderFields {
	char name[8];
	char extension[3];
	char nothing;
	uint32_t ind1;
	BytesCnt fileSize;
	char freeBytes[12];
};

struct OpenFiles {
	int reading;
	int writing;
	OpenFiles() : reading(0), writing(0) {}
};


