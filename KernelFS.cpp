#include "KernelFS.h"
#include "OpenFileTable.h"
#include "KernelFile.h"
#include "javniTest/testprimer.h"



Partition * KernelFS::mountedPart = nullptr;
CONDITION_VARIABLE ConditionVar;
CritSection* KernelFS::cs = cs = nullptr;


KernelFS::KernelFS() {
	mountedPart = nullptr;
}

KernelFS::~KernelFS() {
	delete mountedPart;
	delete cs;
}

char KernelFS::mount(Partition * partition) {
	if (mountedPart == nullptr) {
		cs->enter();
		if (mountedPart == nullptr) {
			mountedPart = partition;
			openFiles = new OpenFileTable();
			//open root dir 
			
		}
		cs->exit();
		return '1';
	}
	else {
		return '0';
	}
}

char KernelFS::unmount() {

	if (mountedPart != nullptr) {
		cs->enter();
		mountedPart = nullptr;
		openFiles->closeAll();
		cs->exit();
		return '1';
	}
	else {
		return '0';
	}
}

char KernelFS::format() {
	if (openFiles->numOfOpen() != 0) {
		//wait
	}

	bitVector.format();
	//init root
	for (ClusterNo i = 0; i <INDEX_SIZE; i++) {
		index1[i] = 0;
	}

	
}

FileCnt KernelFS::readRootDir() {

}

char KernelFS::doesExist(char* fname) {

}

File* KernelFS::getFile(char* fname) {

}

File* KernelFS::open(char* fname, char mode) {
	//u index1Addr upisem iz Headera

	File * file = nullptr;
	KernelFile* kfile = nullptr;
	switch (mode) {
		case 'r': 
			if (!doesExist(fname)) {
				//throw exception
				return nullptr;
			}
			else {
				kfile = new KernelFile();
				kfile->mode = READONLY;
			}
			break;
		case 'w': 
			if (doesExist(fname)) {
				deleteFile(fname);
			}
			kfile = new KernelFile();
			kfile->mode = WRITEONLY;
			break;
		case 'a':
			if (!doesExist(fname)) {
				//throw exception
				return nullptr;
			}
			file = getFile(fname);
			kfile->mode = READANDWRITE;
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
	for (ClusterNo ind1 = 0; ind1 < INDEX_SIZE; ind1++) {
		if (index1[ind1] == 0) {
			continue;
		}
		for (ClusterNo ind2 = 0; ind2 < INDEX_SIZE; ind2++) {

		}
	}

}