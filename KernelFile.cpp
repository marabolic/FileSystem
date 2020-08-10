#include "KernelFile.h"
#include "KernelFS.h"




KernelFile::KernelFile() {
	cursor = 0;
}

KernelFile::~KernelFile() {

}

 

char KernelFile::write(BytesCnt bytesCnt, char* buffer) {
	for (BytesCnt i = 0; i < bytesCnt; i++) {
		if (! eof())
			writeByte(buffer, i);
		else {
			if (canExtend()) {
				extend();
			}
		}
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
	
}



void KernelFile::load(BytesCnt bytesCnt) {
	EnterCriticalSection(&KernelFS::cs);
	KernelFS::mountedPart->readCluster(index1Addr, (char*)index1);
	LeaveCriticalSection(&KernelFS::cs);

	Ind1Entry = bytesCnt / (INDEX_SIZE * ClusterSize);
	index2Addr = index1[Ind1Entry];

	EnterCriticalSection(&KernelFS::cs);
	KernelFS::mountedPart->readCluster(index2Addr, (char*)index2);
	LeaveCriticalSection(&KernelFS::cs);

	Ind2Entry = (bytesCnt % (INDEX_SIZE * ClusterSize)) / ClusterSize;
	dataAddr = index2[Ind2Entry];

	EnterCriticalSection(&KernelFS::cs);
	KernelFS::mountedPart->readCluster(dataAddr, (char*)data);
	LeaveCriticalSection(&KernelFS::cs);

	dataBytePointer = (bytesCnt % (INDEX_SIZE * ClusterSize)) % ClusterSize;

}

void KernelFile::writeByte(char* buff, int i) {
	data[dataBytePointer] = buff[i];
	cursor++;
	seek(cursor);
}

char KernelFile::readByte() {
	char c = data[dataBytePointer];
	cursor++;
	seek(cursor); 
	return c;
}

bool KernelFile::canExtend()
{
	if (fileSize < UINT_MAX)
		return true;
	return false;
}

void KernelFile::extend() {
	 
}

ClusterNo KernelFile::allocate() {
	//find first empty in bitVector and return entry address
}

void KernelFile::deallocate(ClusterNo clusterNo) {
	//remove entry from bitVector
}
