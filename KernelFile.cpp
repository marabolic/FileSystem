#include "KernelFile.h"
#include "KernelFS.h"




KernelFile::KernelFile(HeaderFields header) {
	index1Addr = header.ind1;
	fileSize = header.fileSize;
	cursor = 0;
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
		if (KernelFS::openFileTable[index1Addr]->reading == 0) {
			WakeAllConditionVariable(&KernelFS::readWrite);
			KernelFS::openFileTable.erase(index1Addr);
			delete KernelFS::openFileTable[index1Addr];
		}
	} 
	else { //mode = w or mode = a 
		KernelFS::openFileTable[index1Addr]->writing--;
		WakeAllConditionVariable(&KernelFS::readWrite);
		KernelFS::openFileTable.erase(index1Addr);
		delete KernelFS::openFileTable[index1Addr];
	}

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
}

BytesCnt KernelFile::read(BytesCnt bytesCnt, char* buffer) { //done
	for (BytesCnt i = 0; i < bytesCnt; i++) {
		if (!eof())
			buffer[i] = readByte();
	}

	return bytesCnt; 


}

char KernelFile::seek(BytesCnt bytesCnt) {
	load(bytesCnt);
	cursor = bytesCnt;
	
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

	for (ClusterNo i = Ind2Entry + 1; i < INDEX_SIZE; i++)
	{
		KernelFS::bitVector->reset(index2[i]);
		index2[i] = 0;
	}

	for (ClusterNo i = Ind1Entry + 1; i < INDEX_SIZE; i++)
	{
		KernelFS::bitVector->reset(index1[i]);
		index1[i] = 0;
	}

	fileSize = cursor;

}



void KernelFile::load(BytesCnt bytesCnt) {

	if (index1 == nullptr) {
		EnterCriticalSection(&KernelFS::cs);
		KernelFS::mountedPart->readCluster(index1Addr, (char*)index1);
		LeaveCriticalSection(&KernelFS::cs);
	}

	if (Ind2Entry == INDEX_SIZE - 1) { //optimization

		Ind1Entry = bytesCnt / (INDEX_SIZE * ClusterSize);
		index2Addr = index1[Ind1Entry];

		EnterCriticalSection(&KernelFS::cs);
		KernelFS::mountedPart->readCluster(index2Addr, (char*)index2);
		LeaveCriticalSection(&KernelFS::cs);
	}

	if (dataBytePointer == ClusterSize - 1) { //optimization

		Ind2Entry = (bytesCnt % (INDEX_SIZE * ClusterSize)) / ClusterSize;
		dataAddr = index2[Ind2Entry];

		EnterCriticalSection(&KernelFS::cs);
		KernelFS::mountedPart->readCluster(dataAddr, (char*)data);
		LeaveCriticalSection(&KernelFS::cs);
	}

	dataBytePointer = (bytesCnt % (INDEX_SIZE * ClusterSize)) % ClusterSize;

}

void KernelFile::writeByte(char* buff, int i) {
	data[dataBytePointer] = buff[i];
	if (dataBytePointer == ClusterSize - 1) {

		EnterCriticalSection(&KernelFS::cs);
		KernelFS::mountedPart->writeCluster(dataAddr, (char*)data);
		LeaveCriticalSection(&KernelFS::cs);

		KernelFS::bitVector->reset(dataAddr);
		
		if (Ind2Entry == INDEX_SIZE - 1) {
			EnterCriticalSection(&KernelFS::cs);
			KernelFS::mountedPart->writeCluster(index2Addr, (char*)index2);
			LeaveCriticalSection(&KernelFS::cs);

			KernelFS::bitVector->reset(index2Addr);
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

	ClusterNo freeData = KernelFS::bitVector->findFree();


	EnterCriticalSection(&KernelFS::cs);
	KernelFS::mountedPart->writeCluster(dataAddr, (char*)data);
	KernelFS::mountedPart->readCluster(freeData, (char*)data);
	LeaveCriticalSection(&KernelFS::cs);


	if (Ind2Entry == INDEX_SIZE - 1) {

		ClusterNo freeIndex = KernelFS::bitVector->findFree();

		EnterCriticalSection(&KernelFS::cs);
		KernelFS::mountedPart->writeCluster(index2Addr, (char*)index2);
		KernelFS::mountedPart->readCluster(freeIndex, (char*)index2); //TODO optimize memset
		LeaveCriticalSection(&KernelFS::cs);
	}

	seek(cursor + 1);
	fileSize++;
	
}


