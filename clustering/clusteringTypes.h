#ifndef clusteringTypes_H
#define clusteringTypes_H

#include <string>
#include "boost/lexical_cast.hpp"

#define maxFileLine 300000

typedef enum{mywindows, mylinux} t_system;
extern t_system runningSystem;

typedef enum {sota_algorithm = 0, kmeans_algorithm, singlelink_algorithm, dbscan_algorithm, snn_algorithm, averagelink_algorithm, centroidlink_algorithm, completelink_algorithm, undefined_algorithm} t_algorithms;
typedef enum {euclidean = 0, pearson, undefined_similarity} t_similarities;
typedef enum {correctedRand_index = 0, rand_index, jaccard_index, hubbert_index, folkesMallows_index} t_indices;

std::string strAlgorithms(t_algorithms alg);
std::string strSimilarities(t_similarities sim);
std::string strIndices(t_indices ind);

int randomInt(int lowest, int highest);

//const char *strAlgorithms[] = {"sota", "k-means", "single-link", "undefined"};
//const char *strSimilarities[] = {"euclidean", "pearson", "undefined"};
//const char *strIndices[] = {"corrected rand", "rand", "jaccard", "hubbert", "folkes e mallows"};

//std::string strAlgorithms(t_algorithms alg)
//{    
//    switch(alg) 
//    {
//        case sota_algorithm:   return "sota"; 
//        case kmeans_algorithm:   return "k-means"; 
//        case singleLink_algorithm:   return "single-link"; 
//        default:   return "undefined algorithm";
//    }
//
//}
//
//std::string strSimilarities(t_similarities sim)
//{    
//    switch(sim) 
//    {
//        case euclidean:   return "euclidean"; 
//        case pearson:   return "pearson"; 
//        default:   return "undefined similarity";
//    }
//
//}
//
//std::string strIndices(t_indices ind)
//{    
//    switch(ind) 
//    {
//        case correctedRand_index:   return "corrected rand"; 
//        case rand_index:   return "rand"; 
//        case jaccard_index:   return "jaccard"; 
//        case hubbert_index:   return "hubbert"; 
//        case folkesMallows_index:   return "folkes e mallows"; 
//        default:   return "undefined index";
//    }
//
//}


#endif // clusteringTypes_H
