#ifndef clusteringTypes_H
#define clusteringTypes_H

#include <vector>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

#define maxFileLine 300000
#define firstPatternNumber 1

typedef enum{mywindows, mylinux} t_system;
extern t_system runningSystem;

typedef enum{semisupervised, semisupervised2, unsupervised} t_exp;
extern t_exp expType;

typedef enum{bipartite, mcla, diff, edge} t_crossover;
extern t_crossover crossoverType;


typedef enum{nClusters_obj=0, informationGain_obj, connectivity_obj, 
              correctedRand_obj, purity_obj, deviation_obj, 
              deviationPearson_obj, connectivityPearson_obj} t_objectives;
extern std::vector<t_objectives > objType ;

extern int passo;
extern std::string ga;

typedef enum {sota_algorithm = 0, kmeans_algorithm, singleLink_algorithm, undefined_algorithm} t_algorithms; 
typedef enum {euclidean = 0, pearson, undefined_similarity} t_similarities;
typedef enum {correctedRand_index = 0, rand_index, jaccard_index, hubbert_index, folkesMallows_index} t_indices;

std::string strAlgorithms(t_algorithms alg);
std::string strSimilarities(t_similarities sim);
std::string strIndices(t_indices ind);
std::string strObjectives(t_objectives obj);

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
