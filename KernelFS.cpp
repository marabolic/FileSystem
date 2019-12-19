#include "KernelFS.h"
#include "KernelFile.h"


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
}

FileCnt KernelFS::readRootDir() {

}

char KernelFS::doesExist(char* fname) {

}

KernelFile* getFile(char* fname) {

}

KernelFile* KernelFS::open(char* fname, char mode) {
	switch (mode) {
		case 'r': 
			if (!doesExist(fname)) {
				//throw exception
				return nullptr;
			}
			else {
				KernelFile * file = new File();
				file->isReadOnly = true;
			}
			break;
		case 'w': 
			if (doesExist(fname)) {
				deleteFile(fname);
			}
			KernelFile* file = new File();
			file->iswriteOnly = true;
			break;
		case 'a':
			if (!doesExist(fname)) {
				//throw exception
				return nullptr;
			}
			KernelFile * file = getFile(fname);
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