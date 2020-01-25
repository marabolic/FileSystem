#include "KernelFile.h"
#include "KernelFS.h"


static ClusterNo allocate() {
	 
}

static void deallocate(ClusterNo clusterNo) {
	
}


KernelFile::KernelFile() {

}

KernelFile::~KernelFile() {

}

HeaderFields KernelFile::load(BytesCnt bytesCnt, char* buffer) {
	KernelFS::mountedPart->readCluster(index1Addr, (char*)index1);
	int entry = bytesCnt / ClusterSize;
	KernelFS::mountedPart->readCluster(index1[entry], (char*)index2);
	entry = (512 * 512) / bytesCnt;
	KernelFS::mountedPart->readCluster(index2[entry], (char*)data);
}

char KernelFile::write(BytesCnt bytesCnt, char* buffer) {
	//ucitam klastere u memoriju
	load(bytesCnt, buffer);
	//postavim kursor na prvi/poslednji bajt
	
	//upisujem bajt po bajt
	//dok ne dodjem do kraja klastera
	//upisem klaster
	//uzmem sledeci ako postoji i upisem u njega
	//ako ne postoji, alociram novi -----------> alokacija nadjem slobodan bit u bit vektoru i postavim na 1, i indeks drugog nivoa i prvog azuriram
	//upisem sve na disk 
	
}

BytesCnt KernelFile::read(BytesCnt bytesCnt, char* buffer) {
	load(bytesCnt, buffer);
	//pronadjem bajt po modulu 2048
	byte cursor = bytesCnt % ClusterSize; 
	//prepisivanje


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
	//deallocate sve klastere sa podacima
	//i sve indekse2
	//a ulaze indeksa1 postaviti na 0
}