#include "KernelFile.h"
#include "KernelFS.h"


ClusterNo KernelFile::allocate() {
	 
}

void KernelFile::deallocate(ClusterNo clusterNo) {
	
}


KernelFile::KernelFile() {
	//add to openfiletable

}

KernelFile::~KernelFile() {
	//delete from openfiletable
}

void KernelFile::load(BytesCnt bytesCnt){
	KernelFS::mountedPart->readCluster(index1Addr, (char*)index1);
	int entry = bytesCnt / ClusterSize;
	KernelFS::mountedPart->readCluster(index1[entry], (char*)index2);
	index2Addr = index1[entry];
	entry = (512 * 512) / bytesCnt;
	KernelFS::mountedPart->readCluster(index2[entry], (char*)data);
	dataAddr = index2[entry];
}


char KernelFile::write(BytesCnt bytesCnt, char* buffer) {
	seek(bytesCnt);
	
	//upisujem bajt po bajt
	//dok ne dodjem do kraja klastera

	//upisem klaster
	//KernelFS::mountedPart->writeCluster();

	//uzmem sledeci ako postoji i upisem u njega
	//ako ne postoji, alociram novi -----------> alokacija nadjem slobodan bit u bit vektoru i postavim na 1, i indeks drugog nivoa i prvog azuriram
	//upisem sve na disk 
	
}

BytesCnt KernelFile::read(BytesCnt bytesCnt, char* buffer) {
	seek(0);
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