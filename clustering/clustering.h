
// Observacao: os r�tulos dos clusters s�o n�meros a partir de 1

#ifndef clusteringH
#define clusteringH


#include "sota.h"
#include "kmeans.h"
#include "dbscan.h"
#include "snn.h"
#include "singlelink.h"
#include "averagelink.h"
#include "centroidlink.h"
#include "completelink.h"

#include "cpartition.h"
#include "clusteringTypes.h"
//#include "externalValidation.h"
#include "variability.h"

#include <cstdio> 
#include <cstdlib> 
#include <string>
#include <iostream>
//#include <fstream> // usando a do boost
#include <map>
#include <set>
#include <iterator>

// para incluir as bibliotecas Boost
#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include "boost/filesystem/fstream.hpp"    // ditto
#include "boost/lexical_cast.hpp"

namespace fs = boost::filesystem;


//using std::string;

//using namespace std;

//#define MAX_CLUSTERS    500

//struct cmp
//{
//  bool operator()(string s1, string s2) const
//  {
//    return s1 < s2;
//    //return strcmp(s1, s2) < 0;
//  }
//};


//typedef row * t_membershipMatrix;

class clustering {
    
public:
    clustering();
//    clustering(fs::path);
//    ~clustering();
    clustering(const clustering &c);
    clustering & operator= (const clustering &c);
    cpartition loadPartition(fs::path partitionFile);
    cpartition random(fs::path dataset, fs::path fileResult, int k); // generate a random partition
    cpartition runClustering(fs::path dataset, fs::path expDir, t_algorithms alg, t_similarities sim, std::string parameters);
//    double runExternalValidation(cpartition rP, double alpha, t_indices ind, int B, fs::path bootstrapDir);
    double runVariability(int B, t_indices ind, fs::path bootstrapDir);
    bool isValid();
    double calculateSilhouette(fs::path partitionFileName);

private:     
    bool valid;
    bool runEV;
    fs::path dataset;
    fs::path experimentsDir; 
    fs::path fileResult;
    t_algorithms algorithm;
    t_similarities similarity;
    std::string parameters;
    cpartition p;
    cpartition truePartition;
    
    fs::path datasetSota(fs::path datasetFile);
    cpartition runSota();
    cpartition runKmeans();
    cpartition runSingleLink();
    cpartition runDbscan();
    cpartition runSnn();
    cpartition runAverageLink();
    cpartition runCentroidLink();
    cpartition runCompleteLink();
};

  

#endif // fim unit clustering
