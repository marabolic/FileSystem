#include "OpenFileTable.h"

#include <algorithm>

OpenFileTable::OpenFileTable() {
	numOpen = 0;
}
OpenFileTable::~OpenFileTable() {
	closeAll();
}

bool OpenFileTable::openFile(KernelFile* file) {
	numOpen++;
}
long OpenFileTable::numOfOpen() {
	return numOpen;
}

void OpenFileTable::closeFile(KernelFile* file) {
	fileTable.erase(std::remove(fileTable.begin(), fileTable.end(), file), fileTable.end()); 
	numOpen--;
}