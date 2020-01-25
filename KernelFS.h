#pragma once
#include "fs.h"
#include "Cache.h"
#include "OpenFileTable.h"
#include "KernelFile.h"
#include "Define.h"
#include "BitVector.h"
#include "CritSection.h"

#include "KernelFile.h"
#include "particija-VS2017/part.h"
class KernelFS;
class Partition;
class File;
class CritSection;
class Index1;


typedef char byte;

struct HeaderFields {
	byte name[8];
	byte extension[3];
	byte nothing;
	byte ind1[4];
	byte fileSize[4];
	byte freeBytes[12];
};

struct BitVector {
	byte bitvect[ClusterSize / sizeof(int) * ClusterSize / sizeof(int) * ClusterSize / sizeof(HeaderFields)];

	void format() {
		for (int i = 0; i < ClusterSize; i++) {
			bitvect[i] = 0;
		}
	}

	void clearBit() {

	}

};

struct  Sections {
	BitVector * bv;
	int *ind1[ClusterSize / sizeof(int)]; //2048 / 4 = 512
	int *ind2[ClusterSize / sizeof(int)]; //2048 / 4 = 512
	HeaderFields data[ClusterSize / sizeof(HeaderFields)]; //2048 / 32 = 64
};

class KernelFS
{
public:

	static BitVector bitVector;

	static ClusterNo index1[ClusterSize / sizeof(ClusterNo)];
	static ClusterNo index1Addr;

	static ClusterNo index2[ClusterSize / sizeof(ClusterNo)];
	static ClusterNo index2Addr;

	static CritSection * cs;

	~KernelFS();

	static char mount(Partition * partition);
	static char unmount();
	static char format();
	static FileCnt readRootDir();
	static char doesExist(char* fname);


	static File * getFile(char* fname);
	static void scan();

	static File* open(char* fname, char mode);
	static char deleteFile(char* fname);
	
protected:
	friend class FS;
	friend class KernelFile;
	KernelFS();
	static OpenFileTable *openFiles;
	static Partition * mountedPart;
	static KernelFile * rootDir;
	ClusterNo START_ADDR;
};

