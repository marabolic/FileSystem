#pragma once
#include "fs.h"
#include "file.h"
#include "Cache.h"

//#include "Define.h"
#include "BitVector.h"
#include <map>

#include "KernelFile.h"
//#include "particija-VS2017/part.h"

class KernelFS;
class Partition;
class File;
class CritSection;
class Index1;
class BitVector;
class Cache;


class KernelFS {

public:

 
	static BitVector* bitVector;

	static ClusterNo index1[INDEX_SIZE];
	static ClusterNo index1Addr;

	static ClusterNo index2[INDEX_SIZE];
	static ClusterNo index2Addr;

	static HeaderFields header[DATA_SIZE];
	static ClusterNo headerAddr;


	static ClusterNo headerPointer;
	static ClusterNo Ind1Entry;
	static ClusterNo Ind2Entry;


	static CONDITION_VARIABLE readWrite;
	static CONDITION_VARIABLE openedFiles;

	static CRITICAL_SECTION cs;

	static std::map<ClusterNo, OpenFiles*> openFileTable;
	
	static Cache* mountedPart;
	

	~KernelFS();

	static char mount(Partition * partition);
	static char unmount();
	static char format();
	static FileCnt readRootDir();
	static char doesExist(char* fname);
	static File* open(char* fname, char mode);
	static char deleteFile(char* fname);

	//my methods
	static void load(BytesCnt bytesCnt);
	static void findFreeEntry();
	
protected:
	friend class FS;
	friend class KernelFile;
	friend class File;
	friend class BitVector;
	KernelFS();
	//static KernelFile * rootDir;
	static KernelFS * kernelFS;
	ClusterNo START_ADDR;

};

