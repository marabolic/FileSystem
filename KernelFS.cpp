#include "KernelFS.h"
#include "KernelFile.h"




Cache * KernelFS::mountedPart = nullptr;


KernelFS::KernelFS() {
	mountedPart = nullptr;
}

KernelFS::~KernelFS() {
	
}

char KernelFS::mount(Partition * partition) {
	if (mountedPart == nullptr) {
		EnterCriticalSection(&cs);
		if (mountedPart == nullptr) {
			mountedPart = new Cache(partition);
		}

		LeaveCriticalSection(&cs);
		return '1';
	}
	else {
		return '0';
	}
}

char KernelFS::unmount() {
	if (mountedPart != nullptr) {
		EnterCriticalSection(&cs);
		mountedPart = nullptr;
		delete mountedPart;
		LeaveCriticalSection(&cs);
		return '1';
	}
	else {
		return '0';
	}
}

char KernelFS::format() {
	EnterCriticalSection(&cs);
	bitVector.init();
	for (int i = 0; i < INDEX_SIZE; i++)
	{
		index1[i] = 0;
	}
	rootDir = new KernelFile();
	LeaveCriticalSection(&cs);
}

FileCnt KernelFS::readRootDir() {
	FileCnt count = 0;
	for (BytesCnt i = 0; i < INDEX_SIZE * INDEX_SIZE * DATA_SIZE; i++)
	{
		load(i);
		if (header[i].name) {
			count++;
		}
	}
	return count;
} 

char KernelFS::doesExist(char* fname) {
	char found = '0';
	EnterCriticalSection(&cs);
	KernelFS::mountedPart->readCluster(index1Addr, (char*)index1);
	LeaveCriticalSection(&KernelFS::cs);

	for (ClusterNo i = 0; i < INDEX_SIZE; i++)
	{
		index2Addr = index1[i];
		if (index1Addr != 0) {

			EnterCriticalSection(&KernelFS::cs);
			KernelFS::mountedPart->readCluster(index2Addr, (char*)index2);
			LeaveCriticalSection(&KernelFS::cs);

			for (ClusterNo j = 0; j < INDEX_SIZE; j++)
			{
				headerAddr = index2[j];
				if (index2Addr != 0) {

					EnterCriticalSection(&KernelFS::cs);
					KernelFS::mountedPart->readCluster(headerAddr, (char*)header);
					LeaveCriticalSection(&KernelFS::cs);

					for (ClusterNo k = 0; k < DATA_SIZE; k++)
					{
						if (header[k].name == fname) {
							found = '1';
							headerPointer = k;
						}
					}
				}
			}
		}
	}
	return found;
}

File* KernelFS::open(char* fname, char mode) {
	
	char exists = doesExist(fname);

	KernelFile* file = new KernelFile();
	//file->index1Addr = header[headerPointer].ind1;

	switch (mode)
	{
	case 'r': 
		if (exists == '0') {
			return nullptr;
		}
		if (openFileTable.count(headerPointer) == 0) {
			OpenFiles* openfile = new OpenFiles();
			openfile->reading++;
			openFileTable.insert(pair<int, OpenFiles*>(headerPointer, openfile));
		}
		else {
			openFileTable[headerPointer]->reading++;
		}

		break;
	case 'w':
		if (exists == '1') {
			deleteFile(fname);
		}
		if (openFileTable.count(headerPointer) == 0) {
			OpenFiles* openfile = new OpenFiles();
			openfile->writing++;
			openFileTable.insert(pair<int, OpenFiles*>(headerPointer, openfile));
		}
		else {
			//wait  ---- can't write
			openFileTable[headerPointer]->writing++; 
			
		}
		//new file
		//add to map
		break;
	case 'a':
		if (exists == '0') {
			return nullptr;
		}
		if (openFileTable.count(headerPointer) == 0) {  
			OpenFiles* openfile = new OpenFiles();
			openfile->writing++;
			openFileTable.insert(pair<int, OpenFiles*>(headerPointer, openfile));
		}
		file->seek(header[headerAddr].fileSize);

		break;
	default:
		//error
		break;
	}

	 
}

char KernelFS::deleteFile(char* fname) {
	char exists = doesExist(fname);
	if (exists == '1'){
		KernelFile * file = new KernelFile();
		file->seek(0);
		file->truncate();
	}
				
}
 




File* KernelFS::getFile(char* fname, HeaderFields* hf) {
	
}


void KernelFS::scan() {

}

void KernelFS::load(BytesCnt bytesCnt) {
	EnterCriticalSection(&cs);
	KernelFS::mountedPart->readCluster(index1Addr, (char*)index1);
	LeaveCriticalSection(&KernelFS::cs);

	Ind1Entry = bytesCnt / (INDEX_SIZE * ClusterSize);
	index2Addr = index1[Ind1Entry];

	EnterCriticalSection(&KernelFS::cs);
	KernelFS::mountedPart->readCluster(index2Addr, (char*)index2);
	LeaveCriticalSection(&KernelFS::cs);

	Ind2Entry = (bytesCnt % (INDEX_SIZE * ClusterSize)) / ClusterSize;
	headerAddr = index2[Ind2Entry];

	EnterCriticalSection(&KernelFS::cs);
	KernelFS::mountedPart->readCluster(headerAddr, (char*)header);
	LeaveCriticalSection(&KernelFS::cs);

	headerPointer = (bytesCnt % (INDEX_SIZE * ClusterSize)) % ClusterSize;
}

