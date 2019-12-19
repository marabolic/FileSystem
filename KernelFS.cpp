#include "KernelFS.h"


Partition *KernelFS::mountedPart = nullptr;

KernelFS::KernelFS()
{
	mountedPart = nullptr;
}

KernelFS::~KernelFS()
{
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

File* KernelFS::open(char* fname, char mode) {

}

char KernelFS::deleteFile(char* fname) {

}