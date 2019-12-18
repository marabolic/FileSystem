#include "KernelFS.h"



KernelFS::KernelFS()
{
	mountedPart = nullptr;
}

KernelFS::~KernelFS()
{
}

char KernelFS::mount(Partition* partition) {
	if (mountedPart != nullptr) {
		 
	}
}

char KernelFS::unmount() {

}

char KernelFS::format() {

}

FileCnt KernelFS::readRootDir() {

}

char KernelFS::doesExist(char* fname) {

}

File* KernelFS::open(char* fname, char mode) {

}

char KernelFS::deleteFile(char* fname) {

}