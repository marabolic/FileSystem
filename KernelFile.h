#pragma once
#include "fs.h"
#include "Define.h"


class KernelFile
{
public:
	
	ClusterNo index1Addr;
	ClusterNo index1[INDEX_SIZE]; 

	ClusterNo index2Addr;
	ClusterNo index2[INDEX_SIZE];

	ClusterNo dataAddr;
	char data[ClusterSize];

	
	BytesCnt cursor;
	ClusterNo dataBytePointer;
	ClusterNo Ind1Entry;
	ClusterNo Ind2Entry;


	HeaderFields headerAddr;

	int index2Set;
	int dataSet;

	ClusterNo fileSize;

	Mode mode;

	~KernelFile();


	char write(BytesCnt, char* buffer);
	BytesCnt read(BytesCnt, char* buffer);
	char seek(BytesCnt);
	BytesCnt filePos();
	char eof();  
	BytesCnt getFileSize();
	char truncate();



	void load(BytesCnt);
	void writeByte(char*, int);
	char readByte();
	bool canExtend();
	void extend();


private:
	friend class KernelFS;
	friend class File;
	KernelFile(HeaderFields, Mode);
	KernelFile();

};

