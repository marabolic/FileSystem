#include "KernelFile.h"



KernelFile::KernelFile() {
	isReadAndWrite = false;
	isReadOnly = false;
	isWriteOnly = false;
}

KernelFile::~KernelFile() {

}

char KernelFile::write(BytesCnt, char* buffer) {

}

BytesCnt KernelFile::read(BytesCnt, char* buffer) {

}

char KernelFile::seek(BytesCnt) {

}

BytesCnt KernelFile::filePos() {

}

char KernelFile::eof() {

}

BytesCnt KernelFile::getFileSize() {

}

char KernelFile::truncate() {

}