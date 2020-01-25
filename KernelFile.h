#pragma once
#include "fs.h"
#include "Define.h"
#include "CritSection.h"
#include <windows.h>

class KernelFile
{
public:
	
	ClusterNo index1Addr;
	ClusterNo index1[ClusterSize / sizeof(ClusterNo)];

	ClusterNo index2Addr;
	ClusterNo index2[ClusterSize / sizeof(ClusterNo)];

	ClusterNo dataAddr;
	HeaderFields data[ClusterSize / sizeof(HeaderFields)];

	Mode mode;

	~KernelFile();

	void load(BytesCnt);

	char write(BytesCnt, char* buffer);
	BytesCnt read(BytesCnt, char* buffer);
	char seek(BytesCnt);
	BytesCnt filePos();
	char eof();  
	BytesCnt getFileSize();
	char truncate();

	

private:
	friend class KernelFS;
	friend class File;
	KernelFile();

};

