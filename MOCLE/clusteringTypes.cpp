#include "clusteringTypes.h"

t_system runningSystem;
t_exp expType;
t_crossover crossoverType;
std::vector<t_objectives > objType;
int passo;
std::string ga;

std::string strAlgorithms(t_algorithms alg)
{    
    switch(alg) 
    {
        case sota_algorithm:   return "sota"; 
        case kmeans_algorithm:   return "k-means"; 
        case singleLink_algorithm:   return "single-link"; 
        default:   return "undefined algorithm";
    }

}

std::string strSimilarities(t_similarities sim)
{    
    switch(sim) 
    {
        case euclidean:   return "euclidean"; 
        case pearson:   return "pearson"; 
        default:   return "undefined similarity";
    }

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

std::string strObjectives(t_objectives obj)
{    
    switch(obj) 
    {
        case nClusters_obj:   return "_K"; 
        case informationGain_obj:   return "_GAIN"; 
        case connectivity_obj:   return "_CON"; 
        case correctedRand_obj:   return "_CR"; 
        case purity_obj:   return "_PUR"; 
        case deviation_obj:   return "_DEV"; 
        case deviationPearson_obj:   return "_DEVP"; 
        case connectivityPearson_obj:   return "_CONP"; 
        default:   return "undefined index";
    }
}

int randomInt(int lowest, int highest)
{
    //http://www.daniweb.com/tutorials/tutorial1769.html
    int random_integer; 
    int range = (highest - lowest) + 1; 
//    rand();
//    double r = (double)rand();
//    double n = r / ((double)(RAND_MAX)+(double)(1));
//    random_integer = lowest + int ((double)range * n); 
    
    //random_integer = lowest + int ((double)range * ((double)rand() / (double)(RAND_MAX + 1.0))); 
//    random_integer = lowest + int ((double)range * (r / (double)(RAND_MAX + 1.0))); 
    //random_integer = lowest + int(range * rand() / (RAND_MAX + 1.0)); 

      //random_integer = rand() % (highest - lowest + 1) + lowest; 
      random_integer = rand() % range + lowest; 

//      std::cout << "lowest: " << lowest << "  highest: " << highest << std::endl; 
//      std::cout << "range: " << range << std::endl; 
//      std::cout << "r: " << r << " n: " << n << std::endl; 
//      std::cout << " number: " << random_integer << std::endl; 

    return random_integer;
}

