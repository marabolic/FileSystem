#include "KernelFS.h"


BitVector * KernelFS::bitVector = nullptr;
KernelFS* KernelFS::kernelFS = nullptr;
Cache* KernelFS::mountedPart = nullptr;
std::map<ClusterNo, OpenFiles*> KernelFS::openFileTable =  map<ClusterNo, OpenFiles*>();
ClusterNo KernelFS::index1Addr = 0;
ClusterNo KernelFS::index2Addr = 0;
ClusterNo KernelFS::headerAddr = 0;
ClusterNo KernelFS::Ind1Entry = 0;
ClusterNo KernelFS::Ind2Entry = 0;
ClusterNo KernelFS::headerPointer = 0;
ClusterNo KernelFS::index1[INDEX_SIZE];
ClusterNo KernelFS::index2[INDEX_SIZE]; 
HeaderFields KernelFS::header[DATA_SIZE];
CRITICAL_SECTION KernelFS::cs;
CONDITION_VARIABLE KernelFS::readWrite; 



KernelFS::KernelFS() {
	mountedPart = nullptr;
}

KernelFS::~KernelFS() {
	EnterCriticalSection(&cs);
	mountedPart->writeCluster(headerAddr, (char*)header);
	mountedPart->writeCluster(index2Addr, (char*)index2);
	mountedPart->writeCluster(index1Addr, (char*)index1);
	LeaveCriticalSection(&cs);

	delete mountedPart;
	delete bitVector;
}



char KernelFS::mount(Partition * partition) {

	
	InitializeCriticalSection(&cs);
	InitializeConditionVariable(&readWrite);

	if (mountedPart == nullptr) {
		EnterCriticalSection(&cs);
		if (mountedPart == nullptr) {
			mountedPart = new Cache(partition);
			bitVector = new BitVector(mountedPart->getNumOfClusters());
			index1Addr = bitVector->numOfClusters;
			KernelFS::mountedPart->readCluster(index1Addr, (char*)index1);
			
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
		if (!openFileTable.empty()) {
			LeaveCriticalSection(&cs);
			return '0';
		} 
		//change
		KernelFS::mountedPart->writeCluster(index1Addr, (char*)index2);

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
	bitVector->format();
	memset(index1, 0, ClusterSize);
	bitVector->set(index1Addr);
	LeaveCriticalSection(&cs);
	return '1';
}

FileCnt KernelFS::readRootDir()  {

	FileCnt cnt = 0;

	for (ClusterNo i = 0; i < INDEX_SIZE; i++) {
		ClusterNo index2AddrNew = index1[i];
		Ind1Entry = i;
		if (index2AddrNew != 0) {

			if (index2Addr != index2AddrNew) {
				index2Addr = index2AddrNew;
				EnterCriticalSection(&KernelFS::cs);
				KernelFS::mountedPart->readCluster(index2Addr, (char*)index2);
				LeaveCriticalSection(&KernelFS::cs);
			}

			for (ClusterNo j = 0; j < INDEX_SIZE; j++)
			{
				ClusterNo headerAddrNew = index2[j];
				Ind2Entry = j;
				if (headerAddrNew != 0) {

					if (headerAddr != headerAddrNew) {
						headerAddr = headerAddrNew;
						EnterCriticalSection(&KernelFS::cs);
						KernelFS::mountedPart->readCluster(headerAddr, (char*)header);
						LeaveCriticalSection(&KernelFS::cs);
					}

					for (ClusterNo k = 0; k < DATA_SIZE; k++)
					{
						headerPointer = k;
						if (memcmp(header[k].name, "\0\0\0\0\0\0\0", FNAMELEN) != 0) {
							cnt++;
						}
					}
				}
			}
		}
	}
	return cnt;
} 

char KernelFS::doesExist(char* fname) {
	char found = '0';

	for (ClusterNo i = 0; i < INDEX_SIZE; i++) {
		ClusterNo index2AddrNew = index1[i];
		Ind1Entry = i;
		if (index2AddrNew != 0) {

			if (index2Addr != index2AddrNew) {
				//change
				EnterCriticalSection(&KernelFS::cs);
				KernelFS::mountedPart->writeCluster(index2Addr, (char*)index2);
				KernelFS::mountedPart->readCluster(index2AddrNew, (char*)index2);
				index2Addr = index2AddrNew;
				LeaveCriticalSection(&KernelFS::cs);

			}

			for (ClusterNo j = 0; j < INDEX_SIZE; j++)
			{
				ClusterNo headerAddrNew = index2[j];
				Ind2Entry = j;
				if (headerAddrNew != 0) {

					if (headerAddr != headerAddrNew) {
						//change
						EnterCriticalSection(&KernelFS::cs);
						KernelFS::mountedPart->writeCluster(headerAddr, (char*)index2);
						KernelFS::mountedPart->readCluster(headerAddrNew, (char*)header);
						headerAddr = headerAddrNew;
						LeaveCriticalSection(&KernelFS::cs);
					}

					for (ClusterNo k = 0; k < DATA_SIZE; k++)
					{
						headerPointer = k;
						if (memcmp(header[k].name, fname, FNAMELEN) == 0) {
							found = '1';
							return found;
						}
					}
				}
			}
		}
	}
	return found;
}



File* KernelFS::open(char* fname, char mode) {

	EnterCriticalSection(&cs);
	
	char exists = doesExist(fname);

	File* file = nullptr;
	//file->index1Addr = header[headerPointer].ind1;

	switch (mode)
	{
	case 'r':
		if (exists == '0') {
			LeaveCriticalSection(&cs);
			return nullptr;
		}

		//change 
		if (kernelFS->openFileTable.count(header[headerPointer].ind1) != 0 && kernelFS->openFileTable[header[headerPointer].ind1]->writing > 0) {
			SleepConditionVariableCS(&readWrite, &cs, INFINITE);
		}
		

		file = new File();
		
		file->myImpl = new KernelFile(header[headerPointer], READONLY);


		if (kernelFS->openFileTable.count(header[headerPointer].ind1) == 0) {
			OpenFiles* openfile = new OpenFiles();
			openfile->reading++;
			openFileTable.insert(pair<ClusterNo, OpenFiles*>(header[headerPointer].ind1, openfile));
		}
		else {
			kernelFS->openFileTable[header[headerPointer].ind1]->reading++;
		} 
		file->seek(0);
		file->myImpl->mode = READONLY;
		
		LeaveCriticalSection(&cs);
		return file;
		break;
	 case 'w':

		file = new File();
		//pazi
		

		
		if (exists == '1') {
			file->seek(0);
			file->truncate();
		}
		else {

			findFreeEntry();
			
			header[headerPointer].ind1 = bitVector->findFree();
			strcpy(header[headerPointer].name, fname);
			header[headerPointer].fileSize = 0;

			file->myImpl = new KernelFile(header[headerPointer], WRITEONLY);
			file->seek(0);
		}
		if (openFileTable.count(header[headerPointer].ind1) == 0) {
			OpenFiles* openfile = new OpenFiles();
			openfile->writing++;   
			openFileTable.insert(pair <ClusterNo, OpenFiles* > (header[headerPointer].ind1, openfile));
		}
		else {
			SleepConditionVariableCS(&readWrite, &cs, INFINITE);
			openFileTable[header[headerPointer].ind1]->writing++;
			
		}
		file->myImpl->mode = WRITEONLY;
		LeaveCriticalSection(&cs);
		return file;
		break;
	case 'a':
		if (exists == '0') {
			LeaveCriticalSection(&cs);
			return nullptr;
		}

		file = new File();
		file->myImpl = new KernelFile(header[headerPointer], READANDWRITE);


		if (openFileTable.count(header[headerPointer].ind1)  == 0) {
			OpenFiles* openfile = new OpenFiles();
			openfile->writing++;
			openFileTable.insert(pair<ClusterNo, OpenFiles*>(header[headerPointer].ind1, openfile));
		}
		else {
			//TODO 
			SleepConditionVariableCS(&readWrite, &cs, INFINITE);
			openFileTable[header[headerPointer].ind1]->writing++;
		}
		file->myImpl->mode = READANDWRITE;
		file->seek(header[headerPointer].fileSize);

		LeaveCriticalSection(&cs);
		return file;
		break;
	}
	 
}


void KernelFS::findFreeEntry() {


	for (ClusterNo i = 0; i < INDEX_SIZE; i++) {
		ClusterNo index2AddrNew = index1[i];
		Ind1Entry = i;
		if (index2AddrNew != 0) {

			if (index2Addr != index2AddrNew) {
				//change
				EnterCriticalSection(&KernelFS::cs);
				KernelFS::mountedPart->writeCluster(index2Addr, (char*)index2);
				KernelFS::mountedPart->readCluster(index2AddrNew, (char*)index2);
				LeaveCriticalSection(&KernelFS::cs);
				index2Addr = index2AddrNew;
			}

			for (ClusterNo j = 0; j < INDEX_SIZE; j++)
			{
				ClusterNo headerAddrNew = index2[j];
				Ind2Entry = j;
				if (headerAddrNew != 0) {

					if (headerAddr != headerAddrNew) {
						//change
						EnterCriticalSection(&KernelFS::cs);
						KernelFS::mountedPart->writeCluster(headerAddr, (char*)header);
						KernelFS::mountedPart->readCluster(headerAddrNew, (char*)header);
						LeaveCriticalSection(&KernelFS::cs);
						headerAddr = headerAddrNew;

					}

					for (ClusterNo k = 0; k < DATA_SIZE; k++)
					{
						headerPointer = k;
						if (memcmp(header[k].name, "\0\0\0\0\0\0\0", FNAMELEN) == 0) {
							return;
						}
					}
				}
				else {
					EnterCriticalSection(&KernelFS::cs);
					KernelFS::mountedPart->writeCluster(headerAddr, (char*)header);
					LeaveCriticalSection(&KernelFS::cs);

					headerAddr = bitVector->findFree();
					
					memset(header, 0, ClusterSize);
					index2[Ind2Entry] = headerAddr;
					headerPointer = 0;
					return;
				}
			}
		}
		else {

			EnterCriticalSection(&KernelFS::cs);
			KernelFS::mountedPart->writeCluster(index2Addr, (char*)index2);
			LeaveCriticalSection(&KernelFS::cs);

			index2Addr = bitVector->findFree();
			memset(index2, 0, ClusterSize);
			index1[Ind1Entry] = index2Addr;
			Ind2Entry = 0;

			EnterCriticalSection(&KernelFS::cs);
			KernelFS::mountedPart->writeCluster(headerAddr, (char*)header);
			LeaveCriticalSection(&KernelFS::cs);

			headerAddr = bitVector->findFree();
			memset(header, 0, ClusterSize);
			index2[Ind2Entry] = headerAddr;
			headerPointer = 0;
			
			return;
		}
	}

}


char KernelFS::deleteFile(char* fname) {
	EnterCriticalSection(&cs);
	char exists = doesExist(fname);
	if (exists == '1'){
		KernelFile * file = new KernelFile();
		file->seek(0); 
		file->truncate();

		KernelFS::bitVector->reset(header[headerPointer].ind1);
		strcpy(header[headerPointer].name, "\0\0\0\0\0\0\0");
		delete file;
		
		LeaveCriticalSection(&cs);
		return '1';
	}		
	
	LeaveCriticalSection(&cs);
	return '0';
}
 

void KernelFS::load(BytesCnt bytesCnt) {
	EnterCriticalSection(&cs);
	KernelFS::mountedPart->readCluster(index1Addr, (char*)index1);
	

	Ind1Entry = bytesCnt / (INDEX_SIZE * DATA_SIZE);
	index2Addr = index1[Ind1Entry];

	
	KernelFS::mountedPart->readCluster(index2Addr, (char*)index2);


	Ind2Entry = (bytesCnt % (INDEX_SIZE * DATA_SIZE)) / DATA_SIZE;
	headerAddr = index2[Ind2Entry];

	
	KernelFS::mountedPart->readCluster(headerAddr, (char*)header);
	

	headerPointer = (bytesCnt % (INDEX_SIZE * DATA_SIZE)) % DATA_SIZE;
	LeaveCriticalSection(&KernelFS::cs);
}



