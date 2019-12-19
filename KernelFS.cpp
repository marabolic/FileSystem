#include "KernelFS.h"
#include "KernelFile.h"
#include "javniTest/testprimer.h"


Partition *KernelFS::mountedPart = nullptr;

KernelFS::KernelFS() {
	mountedPart = nullptr;
}

KernelFS::~KernelFS() {
	delete mountedPart;
}

char KernelFS::mount(Partition* partition) {
	if (mountedPart == nullptr) {
		mountedPart = partition;
		return '1';
	}
	else {
		//TODO: block threads 
		return '0';
	}

}

char KernelFS::unmount() {
	if (mountedPart != nullptr) {
		mountedPart = nullptr;
		return '1';
	}
	else {
		return '0';
	}
}

char KernelFS::format() {
	//if (numOfOpen != 0) {
		//wait
	//}

	section->bv->format();
	//init root
	for (int i = 0; i < ClusterSize / sizeof(int); i++) {
		section->ind1[i] = 0;
	}
	
}

FileCnt KernelFS::readRootDir() {

}

char KernelFS::doesExist(char* fname) {

}

KernelFile* KernelFS::getFile(char* fname) {

}

KernelFile* KernelFS::open(char* fname, char mode) {
	KernelFile * file;
	switch (mode) {
		case 'r': 
			if (!doesExist(fname)) {
				//throw exception
				return nullptr;
			}
			else {
				file = new KernelFile();
				file->isReadOnly = true;
			}
			break;
		case 'w': 
			if (doesExist(fname)) {
				deleteFile(fname);
			}
			file = new KernelFile();
			file->isWriteOnly = true;
			break;
		case 'a':
			if (!doesExist(fname)) {
				//throw exception
				return nullptr;
			}
			file = getFile(fname);
			file->isReadAndWrite = true;
			break;
		default: 
			//throw exception
			return nullptr;
	}
	return file;
}

char KernelFS::deleteFile(char* fname) {

}



void KernelFS::scan() {
	
}