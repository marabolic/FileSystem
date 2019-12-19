#pragma once
#include "fs.h"
#include "KernelFile.h"
#include "particija-VS2017/part.h"
class KernelFS;
class Partition;
class File;
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

	static Sections * section;

	~KernelFS();

	static char mount(Partition* partition);
	static char unmount();
	static char format();
	static FileCnt readRootDir();
	static char doesExist(char* fname);

	static KernelFile * getFile(char* fname);
	static void scan();

	static KernelFile* open(char* fname, char mode);
	static char deleteFile(char* fname);
	

protected:
	friend class FS;
	KernelFS();
	static Partition * mountedPart;
};

