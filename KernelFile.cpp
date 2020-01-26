#include "KernelFile.h"
#include "KernelFS.h"




KernelFile::KernelFile() {
	KernelFS::openFiles->openFile(this);

}

KernelFile::~KernelFile() {
	KernelFS::openFiles->closeFile(this);
}

 

char KernelFile::write(BytesCnt bytesCnt, char* buffer) {
	
	//upisujem bajt po bajt
	//dok ne dodjem do kraja klastera

	//upisem klaster
	//KernelFS::mountedPart->writeCluster();

	//uzmem sledeci ako postoji i upisem u njega
	//ako ne postoji, alociram novi -----------> alokacija nadjem slobodan bit u bit vektoru i postavim na 1, i indeks drugog nivoa i prvog azuriram
	//upisem sve na disk 
	
}

BytesCnt KernelFile::read(BytesCnt bytesCnt, char* buffer) {
	//prepisivanje


}

char KernelFile::seek(BytesCnt bytesCnt) {
	load(bytesCnt);
	//pronadjem bajt po modulu 2048
	cursor = bytesCnt % ClusterSize;
	
}

BytesCnt KernelFile::filePos() {
	return cursor;
}

char KernelFile::eof() {

}

BytesCnt KernelFile::getFileSize() { 

}

char KernelFile::truncate() {
	//deallocate sve klastere sa podacima
	//i sve indekse2
	//a ulaze indeksa1 postaviti na 0
}



void KernelFile::load(BytesCnt bytesCnt) {
	KernelFS::mountedPart->readCluster(index1Addr, (char*)index1);
	int entry = bytesCnt / ClusterSize;
	KernelFS::mountedPart->readCluster(index1[entry], (char*)index2);
	index2Addr = index1[entry];
	entry = (512 * 512) / bytesCnt;
	KernelFS::mountedPart->readCluster(index2[entry], (char*)data);
	dataAddr = index2[entry];
}

bool KernelFile::writeByte(char* c) {

}

bool KernelFile::readByte(char* c) {

}

ClusterNo KernelFile::allocate() {
	//find first empty in bitVector and return entry address
}

void KernelFile::deallocate(ClusterNo clusterNo) {
	//remove entry from bitVector
}
