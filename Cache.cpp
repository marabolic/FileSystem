#include "Cache.h"


Cache::Cache(Partition * partition) {
	this->partition = partition;
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
