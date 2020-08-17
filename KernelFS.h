#pragma once
#include "fs.h"
#include "Cache.h"

#include "KernelFile.h"
#include "Define.h"
#include "BitVector.h"
#include <map>


#include "KernelFile.h"
#include "particija-VS2017/part.h"
class KernelFS;
class Partition;
class File;
class CritSection;
class Index1;


//typedef char byte;


class KernelFS
{
public:

	static BitVector * bitVector;

	static ClusterNo index1[INDEX_SIZE];
	static ClusterNo index1Addr;

	static ClusterNo index2[INDEX_SIZE];
	static ClusterNo index2Addr;

	static HeaderFields header[DATA_SIZE];
	static ClusterNo headerAddr;


	static int headerPointer;
	static int Ind1Entry;
	static int Ind2Entry;


	static CONDITION_VARIABLE ConditionVar;
	static CRITICAL_SECTION cs;

	static std::map<int, OpenFiles*> openFileTable;
	
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
	static File* getFile(char* fname, HeaderFields*);
	static void scan();  
	static void load(BytesCnt bytesCnt);
	
protected:
	friend class FS;
	friend class KernelFile;
	KernelFS();
	static KernelFile * rootDir;
	ClusterNo START_ADDR;

};

