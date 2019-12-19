#pragma once
#include "fs.h"

class KernelFile
{
public:
	bool isReadOnly;
	bool isWriteOnly;
	bool isReadAndWrite;

	~KernelFile();
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

