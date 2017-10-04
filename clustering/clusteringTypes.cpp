#include "clusteringTypes.h"

t_system runningSystem;

std::string strAlgorithms(t_algorithms alg)
{    
    switch(alg) 
    {
        case sota_algorithm:   return "ST"; 
        case kmeans_algorithm:   return "KM"; 
        case singlelink_algorithm:   return "SL";
        case dbscan_algorithm: return "DBS"; 
        case snn_algorithm: return "SNN"; 
        case averagelink_algorithm:   return "AL";
        case centroidlink_algorithm:   return "CeL";
        case completelink_algorithm:   return "CoL";
        default:   return "UNDEF";
    }

//    switch(alg) 
//    {
//        case sota_algorithm:   return "sota"; 
//        case kmeans_algorithm:   return "k-means"; 
//        case singlelink_algorithm:   return "single-link";
//        case dbscan_algorithm: return "dbscan"; 
//        case snn_algorithm: return "snn"; 
//        case averagelink_algorithm:   return "average-link";
//        default:   return "undefined algorithm";
//    }
}

std::string strSimilarities(t_similarities sim)
{  
    switch(sim) 
    {
        case euclidean:   return "E"; 
        case pearson:   return "P"; 
        default:   return "UNDEF";
    }
  
//    switch(sim) 
//    {
//        case euclidean:   return "euclidean"; 
//        case pearson:   return "pearson"; 
//        default:   return "undefined similarity";
//    }

}

std::string strIndices(t_indices ind)
{    
    switch(ind) 
    {
        case correctedRand_index:   return "corrected rand"; 
        case rand_index:   return "rand"; 
        case jaccard_index:   return "jaccard"; 
        case hubbert_index:   return "hubbert"; 
        case folkesMallows_index:   return "folkes e mallows"; 
        default:   return "undefined index";
    }

}

int randomInt(int lowest, int highest)
{
    int random_integer; 
    int range = (highest - lowest) + 1; 
    random_integer = lowest + int (range * rand() / (RAND_MAX + 1.0)); 
    return random_integer;
}

