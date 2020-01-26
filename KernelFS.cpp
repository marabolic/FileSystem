#include "KernelFS.h"
#include "OpenFileTable.h"
#include "KernelFile.h"
#include "javniTest/testprimer.h"



Partition * KernelFS::mountedPart = nullptr;
CONDITION_VARIABLE ConditionVar;
CritSection* KernelFS::cs = nullptr;


KernelFS::KernelFS() {
	mountedPart = nullptr;
}

KernelFS::~KernelFS() {
	
}

char KernelFS::mount(Partition * partition) {
	if (mountedPart == nullptr) {
		cs = new CritSection();
		cs->enter();
		if (mountedPart == nullptr) {
			mountedPart = partition;
			openFiles = new OpenFileTable();
			//open root dir
			//bit vector init
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
		delete openFiles;
		//bitvector write
		//close rootDir
		delete mountedPart;
		cs->exit();
		delete cs;
		return '1';
	}
	else {
		return '0';
	}
}

char KernelFS::format() {
	cs->enter();
	if (openFiles->numOfOpen() != 0) {
		//wait
	}
	bitVector.format();
	//set index of root to null 
	for (ClusterNo i = 0; i <INDEX_SIZE; i++) {
		index1[i] = 0;
	}
	cs->exit();
}

FileCnt KernelFS::readRootDir() {
	FileCnt cnt = 0;
	HeaderFields hf;
	rootDir->seek(0);
	while (!rootDir->eof()) {
		rootDir->read(sizeof(HeaderFields), (char*)&hf);
		if (hf.name != 0) { 
			cnt++;
		}
	}

	return cnt;
}

char KernelFS::doesExist(char* fname) {
	FileCnt cnt = 0;
	HeaderFields hf;
	rootDir->seek(0);
	while (!rootDir->eof()) {
		rootDir->read(sizeof(HeaderFields), (char*)& hf);
		if (hf.name == fname){
			return '1';
		}
	}
						
				
	return '0';
}

File * KernelFS::getFile(char* fname, HeaderFields * hf) {
	File* f;
	rootDir->seek(0);
	while (!rootDir->eof()) {
		rootDir->read(sizeof(HeaderFields), (char*)& hf);
		if (hf->name == fname) {
			//todo
			return f;
		}
	}
	return nullptr;
}

File* KernelFS::open(char* fname, char mode) {
	//u index1Addr upisem iz Headera

	File* file = new File();
	KernelFile* kfile = file->myImpl;
	ClusterNo newCluster;
	switch (mode) {
		case 'r': 
			if (!doesExist(fname)) {
				//throw exception
				return nullptr;
			}
			else {
				kfile->mode = READONLY;
				kfile->seek(0);
			}
			break;
		case 'w': 
			if (doesExist(fname)) {
				deleteFile(fname);
			}
			//alloc - set bitvector
			newCluster = KernelFile::allocate();

			//set addr in header
			//write in root dir - find first empty space - write(sizeof(HeaderFields), space)
			kfile = new KernelFile();
			kfile->mode = WRITEONLY;
			break;
		case 'a':
			if (!doesExist(fname)) {
				//throw exception
				return nullptr;
			}
			kfile->mode = READANDWRITE;
			HeaderFields * hf;
			file = getFile(fname, hf);
			file->seek(file->getFileSize());
			break;
		default: 
			//throw exception
			return nullptr;
	}
	return file;
}

char KernelFS::deleteFile(char* fname) {
	FileCnt cnt = 0;
	HeaderFields * hf;
	rootDir->seek(0);
	while (!rootDir->eof()) {
		rootDir->read(sizeof(HeaderFields), (char*)& hf);
		if (hf->name == fname) {
			//hf->name = 0;
		}
	}
	return 0;
				
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