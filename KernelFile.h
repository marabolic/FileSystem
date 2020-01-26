#pragma once
#include "fs.h"
#include "Define.h"
#include "CritSection.h"
#include <windows.h>

class KernelFile
{
public:
	
	ClusterNo index1Addr;
	ClusterNo index1[INDEX_SIZE];

	ClusterNo index2Addr;
	ClusterNo index2[INDEX_SIZE];

	ClusterNo dataAddr;
	HeaderFields data[DATA_SIZE];
	
	byte cursor;

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
	bool writeByte(char*);
	bool readByte(char*);


	static ClusterNo allocate();
	static void deallocate(ClusterNo);
	

private:
	friend class KernelFS;
	friend class File;
	KernelFile();

};

