#pragma once
#include "fs.h"
#include "Define.h"
#include <windows.h>

class KernelFile
{
public:
	
	ClusterNo index1Addr;
	ClusterNo index1[INDEX_SIZE]; 

	ClusterNo index2Addr;
	ClusterNo index2[INDEX_SIZE];

	ClusterNo dataAddr;
	char data[ClusterSize];

	
	byte cursor;
	int dataBytePointer;
	int Ind1Entry;
	int Ind2Entry;

	int fileSize;

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


	static ClusterNo allocate();
	static void deallocate(ClusterNo);
	

private:
	friend class KernelFS;
	friend class File;
	KernelFile();

};

