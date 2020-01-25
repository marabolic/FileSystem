#include "Cache.h"


Cache::Cache(char * name) {
	partition = new Partition(name);
}
Cache::~Cache() {
	delete partition;
}


ClusterNo Cache::getNumOfClusters() const {
	return partition->getNumOfClusters();
}


int Cache::readCluster(ClusterNo clusterNo, char* buffer) {
	return partition->readCluster(clusterNo, buffer);
}
int Cache::writeCluster(ClusterNo clusterNo, const char* buffer) {
	return partition->writeCluster(clusterNo, buffer);
}
