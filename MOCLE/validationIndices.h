#include "tPartition.h"

double cr(tPartition &Pe, tPartition &Pr);
double connectivity(tPartition &Pe);
double deviation(tPartition &Pe);
double informationGain(tPartition &Pe, tPartition *truePartition);
double connectivityPearson(tPartition &Pe);
double deviationPearson(tPartition &Pe);
//double silhouette(fs::path dataset, fs::path partitionFileName, t_similarities sim);
double squaredErrorEuclidean(tPartition &Pe);
double squaredErrorPearson(tPartition &Pe);
double squaredErrorCoseno(tPartition &Pe);
