#include "KernelFile.h"
#include "KernelFS.h"
#include <math.h>



KernelFile::KernelFile(HeaderFields headerAddr) {
	index1Addr = headerAddr.ind1;
	fileSize = headerAddr.fileSize;
	cursor = 0;
	dataBytePointer = 0;
	index2Set = 0;
	dataSet = 0;
	
	if (mode == READONLY) {
		EnterCriticalSection(&KernelFS::cs);
		KernelFS::mountedPart->readCluster(index1Addr, (char*)index1);
		LeaveCriticalSection(&KernelFS::cs);
	}
	else {
		memset(index1, 0, ClusterSize);
	}
	
}

KernelFile::KernelFile() {
	cursor = 0;
}



KernelFile::~KernelFile() {

	EnterCriticalSection(&KernelFS::cs);
	if (mode != READONLY) {
		KernelFS::mountedPart->writeCluster(dataAddr, (char*)data);
		KernelFS::mountedPart->writeCluster(index2Addr, (char*)index2);
		KernelFS::mountedPart->writeCluster(index1Addr, (char*)index1);
	}


	if (mode == READONLY) {
		KernelFS::openFileTable[index1Addr]->reading--;
		if (KernelFS::kernelFS->openFileTable[index1Addr]->reading == 0) {
			WakeConditionVariable(&KernelFS::readWrite);
			delete KernelFS::kernelFS->openFileTable[index1Addr];
			KernelFS::kernelFS->openFileTable.erase(index1Addr);
			
		}
	} 
	else { //mode = w or mode = a 
		OpenFiles * p = KernelFS::kernelFS->openFileTable[index1Addr];
		KernelFS::kernelFS->openFileTable[index1Addr]->writing--;
		WakeConditionVariable(&KernelFS::readWrite);
		delete KernelFS::kernelFS->openFileTable[index1Addr];
		KernelFS::kernelFS->openFileTable.erase(index1Addr);
	}

	index2Set = 0;
	dataSet = 0;

	LeaveCriticalSection(&KernelFS::cs);

} 

  

char KernelFile::write(BytesCnt bytesCnt, char* buffer) {
	for (BytesCnt i = 0; i < bytesCnt; i++) {
		if (eof()) {
			if (canExtend()) {
				extend();
			}
			else {
				return '0';
			}
		} 
		writeByte(buffer, i); 
	}
	return '1';
}

BytesCnt KernelFile::read(BytesCnt bytesCnt, char* buffer) { //done
	for (BytesCnt i = 0; i < bytesCnt; i++) {
		if (!eof())
			buffer[i] = readByte();
	}
	return bytesCnt; 
}

char KernelFile::seek(BytesCnt bytesCnt) {
	if (bytesCnt < 0)
		return '0';
	load(bytesCnt);
	cursor = bytesCnt;
	return '1';
}

BytesCnt KernelFile::filePos() {
	return cursor;
}

char KernelFile::eof() {
	return cursor == getFileSize();
}

BytesCnt KernelFile::getFileSize() { 
	return fileSize; 
}

char KernelFile::truncate() {

	for (ClusterNo i = Ind2Entry + 1; i < ceil((double)( fileSize - ClusterSize * INDEX_SIZE * Ind1Entry) / ClusterSize); i++)
	{
		KernelFS::bitVector->reset(index2[i]);
		index2[i] = 0;
	}

	for (ClusterNo i = Ind1Entry + 1; i < ceil( (double) fileSize / (INDEX_SIZE * ClusterSize)); i++)
	{
		KernelFS::bitVector->reset(index1[i]);
		index1[i] = 0;
	}


	if (cursor == 0) {
		KernelFS::bitVector->reset(index2[0]);
		index2[0] = 0;
		KernelFS::bitVector->reset(index1[0]);
		index1[0] = 0;

		
	}

	fileSize = cursor;
	return '1';
}



void KernelFile::load(BytesCnt bytesCnt) {

	ClusterNo Ind1EntryNew = bytesCnt / (INDEX_SIZE * ClusterSize);
	ClusterNo Ind2EntryNew = (bytesCnt % (INDEX_SIZE * ClusterSize)) / ClusterSize;
	dataBytePointer = (bytesCnt % (INDEX_SIZE * ClusterSize)) % ClusterSize;

	if (Ind1Entry != Ind1EntryNew || index2Set == 0) { //optimization

		index2Set = 1;
		Ind1Entry = bytesCnt / (INDEX_SIZE * ClusterSize);
		index2Addr = index1[Ind1Entry];

		EnterCriticalSection(&KernelFS::cs);
		KernelFS::mountedPart->readCluster(index2Addr, (char*)index2);
		LeaveCriticalSection(&KernelFS::cs);
	}

	if (Ind2Entry != Ind2EntryNew || dataSet == 0) { //optimization

		dataSet = 1;
		Ind2Entry = (bytesCnt % (INDEX_SIZE * ClusterSize)) / ClusterSize;
		dataAddr = index2[Ind2Entry];

		EnterCriticalSection(&KernelFS::cs);
		KernelFS::mountedPart->readCluster(dataAddr, (char*)data);
		LeaveCriticalSection(&KernelFS::cs);
	}



}

void KernelFile::writeByte(char* buff, int i) {
	data[dataBytePointer] = buff[i];
	if (dataBytePointer == ClusterSize - 1) {

		EnterCriticalSection(&KernelFS::cs);
		KernelFS::mountedPart->writeCluster(dataAddr, (char*)data);
		LeaveCriticalSection(&KernelFS::cs);
		
		if (Ind2Entry == INDEX_SIZE - 1) {
			EnterCriticalSection(&KernelFS::cs);
			KernelFS::mountedPart->writeCluster(index2Addr, (char*)index2);
			LeaveCriticalSection(&KernelFS::cs);
		}
	}
	seek(cursor + 1);
}

char KernelFile::readByte() {
	char c = data[dataBytePointer];
	seek(cursor + 1); 
	return c;
}

bool KernelFile::canExtend()
{
	if (fileSize < UINT_MAX)
		return true;
	return false;
}

void KernelFile::extend() {

	if (fileSize % ClusterSize == 0) {
		ClusterNo freeData = KernelFS::bitVector->findFree();

		EnterCriticalSection(&KernelFS::cs);
		if (fileSize != 0) {
			KernelFS::mountedPart->writeCluster(dataAddr, (char*)data);
		}
		memset(data, 0, ClusterSize);
		dataAddr = freeData;
		
		LeaveCriticalSection(&KernelFS::cs);



		if (fileSize % (ClusterSize * INDEX_SIZE) == 0) {

			ClusterNo freeIndex = KernelFS::bitVector->findFree();

			EnterCriticalSection(&KernelFS::cs);
			if (fileSize != 0) {
				KernelFS::mountedPart->writeCluster(index2Addr, (char*)index2);
			}
			memset(index2, 0, INDEX_SIZE);
			index2Addr = freeIndex;
			index1[Ind1Entry] = index2Addr;
			LeaveCriticalSection(&KernelFS::cs);
		}

		index2[Ind2Entry] = dataAddr;
	}
	fileSize++;
	
}


