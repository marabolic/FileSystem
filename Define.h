#pragma once
#include "particija-VS2017/part.h"
#include "fs.h"
#include <windows.h>
#include <stdint.h>

#define INDEX_SIZE ClusterSize / sizeof(ClusterNo)   //256
#define DATA_SIZE ClusterSize / sizeof(HeaderFields) //64



//typedef char byte;

enum Mode { READONLY = 0, WRITEONLY = 1, READANDWRITE = 2 };

struct HeaderFields {
	char name[FNAMELEN];
	char extension[FEXTLEN];
	char nothing;
	ClusterNo ind1;
	BytesCnt fileSize;
	char freeBytes[12];
};

struct OpenFiles {
	int reading;
	int writing;
	OpenFiles() : reading(0), writing(0) {}
};


