#pragma once
#include "KernelFile.h"
#include <vector>
using namespace std;
class OpenFileTable
{
public:
	OpenFileTable();
	~OpenFileTable();
	
	bool openFile(KernelFile* file);
	void closeFile(KernelFile* file);
	void closeAll();
	long numOfOpen();


private:
	vector<KernelFile*> fileTable;
	long numOpen;
};

