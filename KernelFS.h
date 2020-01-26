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


class KernelFS
{
public:

	static BitVector bitVector;

	static ClusterNo index1[INDEX_SIZE];
	static ClusterNo index1Addr;

	static ClusterNo index2[INDEX_SIZE];
	static ClusterNo index2Addr;

	static HeaderFields header[DATA_SIZE];
	static HeaderFields headerAddr;

	static KernelFile rootDir;

	static CritSection * cs;

	~KernelFS();

	static char mount(Partition * partition);
	static char unmount();
	static char format();
	static FileCnt readRootDir();
	static char doesExist(char* fname);


	static HeaderFields getFile(char* fname);
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

