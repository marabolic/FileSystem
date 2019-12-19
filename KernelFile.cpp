#include "KernelFile.h"



KernelFile::KernelFile() {
	isReadAndWrite = false;
	isReadOnly = false;
	isWriteOnly = false;
}

KernelFile::~KernelFile() {

}

char KernelFile::write(BytesCnt bytesCnt, char* buffer) {

}

BytesCnt KernelFile::read(BytesCnt bytesCnt, char* buffer) {

}

char KernelFile::seek(BytesCnt bytesCnt) {

}

BytesCnt KernelFile::filePos() {

}

char KernelFile::eof() {

}

BytesCnt KernelFile::getFileSize() {

}

char KernelFile::truncate() {

}