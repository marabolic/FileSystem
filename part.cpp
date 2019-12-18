#include "particija-VS2017/part.h"
#include "PartitionImpl.h"

Partition::Partition(char *) {

}
ClusterNo Partition::getNumOfClusters() const {
	return myImpl->getNumOfClusters();
}

int Partition::readCluster(ClusterNo clusterNo, char *buffer) {
	return myImpl->readCluster(clusterNo, buffer);
}
int Partition::writeCluster(ClusterNo clusterNo, const char *buffer) {
	return myImpl->writeCluster(clusterNo, buffer);
}

Partition::~Partition() {

}