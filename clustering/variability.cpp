// Class automatically generated by Dev-C++ New Class wizard
#include <cstring>
#include "variability.h" // class's header file

//using boost::filesystem;    // for ease of tutorial presentation;

namespace fs = boost::filesystem;

using namespace std;


// class constructor
variability::variability()
{
  B = 100; // default is 100 bootstrap samples
  partitionDistance = correctedRand_index;
}

// class destructor
//variability::~variability()
//{
//	// insert your code here
//}

variability::variability(const variability &a)
{
    B = a.B; 
    n = a.n; 
    partitionDistance = a.partitionDistance;
    dataset = a.dataset;
    variabilityDir = a.variabilityDir; 
    bootstrapSampleDir = a.bootstrapSampleDir; 
    randomPartitionsDir = a.randomPartitionsDir; 
    algorithmPartitionsDir = a.algorithmPartitionsDir; 
        
    randomPartitions = a.randomPartitions;
    algorithmPartitions = a.algorithmPartitions;

    algorithm = a.algorithm; 
    similarity = a.similarity;
    parameters = a.parameters;
} 

variability & variability::operator = (const variability &a)
{
    B = a.B; 
    n = a.n; 
    partitionDistance = a.partitionDistance;
    dataset = a.dataset;
    variabilityDir = a.variabilityDir; 
    bootstrapSampleDir = a.bootstrapSampleDir; 
    randomPartitionsDir = a.randomPartitionsDir; 
    algorithmPartitionsDir = a.algorithmPartitionsDir; 
        
    randomPartitions = a.randomPartitions;
    algorithmPartitions = a.algorithmPartitions;

    algorithm = a.algorithm; 
    similarity = a.similarity;
    parameters = a.parameters;
} 

void variability::generateBootstrapSamples()
{
    bootstrap bootstrapSamples(dataset, bootstrapSampleDir, B, n);

}

void variability::generateRandomPartitions(int nClusters)
{
     string command;
     string fileRandomPartition;


     for (int sample = 0; sample < B; sample++)
     {
         fileRandomPartition = randomPartitionsDir.string() + "/" + fs::basename(dataset) + 
                               "RandomPartition" + boost::lexical_cast<string>(sample+1) + ".txt"; // sample + 1 for the names of partitions contains the number from 1 to B
         cpartition p;
         p.randomPartition(dataset, fileRandomPartition, nClusters);
         randomPartitions.insert(randomPartitions.end(), p);
         
     }
}
 
void variability::runAlgorithm()
{
    fs::path fileAlgorithmPartition;
    string fileResultName;
    string bootstrapDataset;

    cpartition p;
    
    switch(algorithm) 
    {
      case sota_algorithm:   
           {
               sota alg(parameters);
               alg.changeSimilarity(similarity);
               for (int sample = 1; sample <= B; sample++)
               {             
                   bootstrapDataset = bootstrapSampleDir.string() + "/" + fs::basename(dataset) + 
                                      "BootstrapSample" + boost::lexical_cast<string>(sample) + ".txt";
                   alg.changeDataset(bootstrapDataset);
                   
                   fileAlgorithmPartition = algorithmPartitionsDir.string() + "/" + fs::basename(dataset) + 
                                            "AlgorithmPartition" + boost::lexical_cast<string>(sample) + ".txt";

                   alg.changeResultFile(fileAlgorithmPartition.string());
                   p = alg.run();
                   
                   p = completePartition(alg, fileAlgorithmPartition);
                   algorithmPartitions.insert(algorithmPartitions.end(), p);                   
               }
           } 
           break;
           
      case kmeans_algorithm:    
           {
               kmeans alg(parameters);
               
               alg.changeSimilarity(similarity);
               for (int sample = 1; sample <= B; sample++)
               {          
                   bootstrapDataset = bootstrapSampleDir.string() + "/" + fs::basename(dataset) + 
                                      "BootstrapSample" + boost::lexical_cast<string>(sample) + ".txt";
                   alg.changeDataset(bootstrapDataset);
 
                   fileAlgorithmPartition = algorithmPartitionsDir.string() + "/" + fs::basename(dataset) + 
                                            "AlgorithmPartition" + boost::lexical_cast<string>(sample) + ".txt";

                   alg.changeResultFile(fileAlgorithmPartition.string());
                   p = alg.run();

                   p = completePartition(alg, fileAlgorithmPartition);
                   algorithmPartitions.insert(algorithmPartitions.end(), p);                                                                          
               }
           } 
           break;
           
      case dbscan_algorithm:   
           {                              
               dbscan alg(parameters);
               alg.changeSimilarity(similarity);
               for (int sample = 1; sample <= B; sample++)
               {
                   bootstrapDataset = bootstrapSampleDir.string() + "/" + fs::basename(dataset) + 
                                      "BootstrapSample" + boost::lexical_cast<string>(sample) + ".txt";
                   alg.changeDataset(bootstrapDataset);
                   
 
                   fileAlgorithmPartition = algorithmPartitionsDir.string() + "/" + fs::basename(dataset) + 
                                            "AlgorithmPartition" + boost::lexical_cast<string>(sample) + ".txt";

                   alg.changeResultFile(fileAlgorithmPartition.string());
                   p = alg.run();
                   
                   p = completePartition(alg, fileAlgorithmPartition);
                   algorithmPartitions.insert(algorithmPartitions.end(), p);                   
               }
           } 
           break;

//      case singleLink_algorithm:    break;
      //      default:   statement sequence; break;
    }
}

double variability::calculateVariability(std::string fileName, 
                         fs::path experimentsDir, 
                         fs::path bsDir,
                         int nBootstrapSamples,
                         t_algorithms alg, //sota::sota clusteringAlgorithm, ////
                         t_similarities sim, 
                         t_indices ind, 
                         int nClusters,
                         std::string param)
{
	std::string header;
    char line[maxFileLine];
    fs::ifstream file;
    
    double V, Vran, adjustedV;
    validationIndex index;
    
    algorithm = alg;
    similarity = sim;
    partitionDistance = ind;
    parameters = param;
    B = nBootstrapSamples;
    bootstrapSampleDir = bsDir;
      
    dataset = fileName;
    
    variabilityDir = experimentsDir / "Variability";
    fs::create_directory(variabilityDir);

    randomPartitionsDir = variabilityDir;
    randomPartitionsDir /= "randomPartitions";
    fs::create_directory(randomPartitionsDir);
    
    algorithmPartitionsDir = variabilityDir;
    algorithmPartitionsDir /= "algorithmPartitions";
    fs::create_directory(algorithmPartitionsDir);
    
    fs::path fileResultValidation;
    fileResultValidation = experimentsDir / "summaryVariability.txt";
    // carrega dataset
    fs::ofstream res;
    res.open(fileResultValidation);
    if (!res.good()) 
    {
        std::cout << "\nError in the file '" << fileResultValidation.string() << "'\n";
        getchar();
    }

   res << "Variability Summary" << std::endl << std::endl;
   res << "Dataset: " << dataset.string() << std::endl;
   res << "Partition distance: " << strIndices(partitionDistance) << std::endl;
   res << "Number of bootstrap samples: " << B << std::endl;
   res << "Algorithm: " << strAlgorithms(algorithm) << std::endl;
   res << "Similarity: " << strSimilarities(similarity) << std::endl;
   res << "Parameters of the algorithm: " << parameters << std::endl;
    
    // count the number of examples of the dataset
    file.open(dataset);
    if (!file.good()) 
    {
        std::cout << "\nError in the file '" << dataset.string() << std::endl;
        getchar();
    }
    file.getline(line, maxFileLine);
    header = line;
    n = 0;
    while (!file.eof())
    {
        file.getline(line, maxFileLine);
        if (strcmp(line, ""))
        {
            n++;
        }
   }
   file.close();
	
    if (bootstrapSampleDir == "")
    {
        bootstrapSampleDir = variabilityDir;
        bootstrapSampleDir /= "bootstrapSamples";
        fs::create_directory(bootstrapSampleDir);
    
        // generate bootstrap samples
        generateBootstrapSamples(); // tem que ser aqui, pois preciso de n
    }

    string bootstrapFileName1 = bootstrapSampleDir.string() + "/" + fs::basename(dataset) + "BootstrapSample1.txt";
    if (!fs::exists(bootstrapFileName1))
    {
        generateBootstrapSamples();   
    }

   // generate algorithm partitions
   runAlgorithm();
   
   // generate random partitions
   generateRandomPartitions(nClusters);  // ver qual valor de k usar aqui
   
   res << "Number of patterns: " << n << std::endl;
   res << "Number of clusters employed: " << nClusters << std::endl << std::endl;

   V = 0;
   Vran = 0;
   double auxV;
   double cr;
   for (int i = 0; i < B; i++)
   {
       for (int j = i + 1; j < B; j++)
       {
           cr = index.calculate(ind, algorithmPartitions[i], algorithmPartitions[j]);
           auxV = 1 - cr;
           V += auxV;
           res << "cr[" << i << "," << j << "]: " << cr << std::endl;
           cr = index.calculate(ind, randomPartitions[i], randomPartitions[j]);
           auxV = 1 - cr;
           Vran += auxV;
           res << "cr(random)[" << i << "," << j << "]: " << cr << std::endl << std::endl;
       }
   }
   V = V / (B * (B - 1) );
   Vran = Vran / (B * (B - 1) );   
   adjustedV = V / Vran;
   res << std::endl << "V: " << V << std::endl;
   res << "Vran: " << Vran << std::endl;
   res << "adjustedV: " << adjustedV << std::endl;
   
   res.close(); // close the summary file
   
   return adjustedV;

}


cpartition variability::completePartition(kmeans alg, fs::path fileAlgorithmPartition)
{
    fs::ifstream file;
    char line[maxFileLine];
    string line2, identifier, s;
    vector<string> header;
    fs::path fileAlgorithmPartitionCompleted;

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep("\t ");
    std::map<std::string, vector<double>, cmp> pat;

    // carrega dataset
    file.open(dataset);
    if (!file.good()) 
    {
        std::cout << "\nError in the file '" << dataset.string() << "'\n";
        getchar();
    }
    
    file.getline(line, maxFileLine);
    line2 = line;
    tokenizer tokens(line2, sep);
    for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
    {
        header.insert(header.end(), *tok_iter);
    }

    while (!file.eof())
    {
        file.getline(line, maxFileLine);
        line2 = line;
        tokens.assign(line2, sep);

        for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
        {
            if (tok_iter == tokens.begin())
            {
               identifier = *tok_iter;
            }
            else
            {
                s = *tok_iter; 
                pat[identifier].insert(pat[identifier].end(), atof(s.c_str())); 
            }            
        }
   }
   file.close();
   
   kmeans::t_centroids centroids = alg.getCentroids();

   std::map<std::string, std::vector<double>, cmp>::iterator patIterator;        
   cpartition::t_iterator_Patterns partitionIterator;
   std::set<int> clusterLabels;
   std::set<int>::iterator c;
   cpartition p;
   p =  alg.getPartition();
   clusterLabels = p.getClustersLabels();
   double distance;
   
   // encontra cluster mais proximo e associa o padr�o a esse cluster
   for (patIterator = pat.begin(); patIterator != pat.end(); patIterator++)    
   {
       // se o padr�o n�o pertence a parti��o, ele deve ser associado ao cluster com centro mais proximo
       
       int closestCluster = 0;
       double closestDistance = 1000000;
       
       //verifica se o padr�o ja esta em uma parti��o
       cpartition::t_Patterns clustersPatterns = p.getClustersPatterns();
       partitionIterator = clustersPatterns.find((*patIterator).first);
       if (partitionIterator == clustersPatterns.end()) 
       {
          map <int, double> distancesToCentroids;
          map <int, double>::iterator distanceIterator;
          for (c = clusterLabels.begin(); c != clusterLabels.end(); c++)
          {
              switch(similarity) 
              {
                  case euclidean:  distancesToCentroids[*c] = euclideanDistance(centroids[*c], (*patIterator).second);
                  break;
           
                  case pearson: distancesToCentroids[*c] = pearsonDistance(centroids[*c], (*patIterator).second); 
                  break;
               }
           }
           
           for (distanceIterator = distancesToCentroids.begin(); distanceIterator != distancesToCentroids.end(); distanceIterator++)
           {
               //distanceToCentroids
               if ((*distanceIterator).second < closestDistance)
               {
                   closestDistance = (*distanceIterator).second;
                   closestCluster = (*distanceIterator).first;
               }
           }
           p.patternInsert((*patIterator).first, closestCluster);        
           fileAlgorithmPartitionCompleted = algorithmPartitionsDir.string() + "/" + fs::basename(fileAlgorithmPartition) + "completed" + ".txt";
           p.printPartition(fileAlgorithmPartitionCompleted);
       }
   }
   
   return p;
}

cpartition variability::completePartition(sota alg, fs::path fileAlgorithmPartition)
{
    fs::ifstream file;
    fs::ifstream file2,file3;
    char line[maxFileLine];
    string line2, identifier, s;
    vector<string> header;
    fs::path fileAlgorithmPartitionCompleted;

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep("\t ");
    std::map<std::string, vector<double>, cmp> pat;

    // carrega dataset
    file.open(dataset);
    if (!file.good()) 
    {
        std::cout << "\nError in the file '" << dataset.string() << "'\n";
        getchar();
    }   
    file.getline(line, maxFileLine);
    line2 = line;
    tokenizer tokens(line2, sep);
    for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
    {
        header.insert(header.end(), *tok_iter);
    }
    while (!file.eof())
    {
        file.getline(line, maxFileLine);
        line2 = line;
        tokens.assign(line2, sep);
        for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
        {
            if (tok_iter == tokens.begin())
            {
               identifier = *tok_iter;
            }
            else
            {
                s = *tok_iter; 
                pat[identifier].insert(pat[identifier].end(), atof(s.c_str())); 
            }
        }
    }
    file.close();
        
    sota::t_cells clustersCenters = alg.getCells();

    std::map<std::string, std::vector<double>, cmp>::iterator patIterator;        
    cpartition::t_iterator_Patterns partitionIterator;
    std::set<int> clusterLabels;
    std::set<int>::iterator c;
    cpartition p;
    p =  alg.getPartition();
    clusterLabels = p.getClustersLabels();
    double distance;

   // find the closest cluster and assign the pattern to this cluster
   for (patIterator = pat.begin(); patIterator != pat.end(); patIterator++)    
   {
       // se o padr�o n�o pertence a parti��o, ele deve ser associado ao cluster com centro mais proximo
       // if the pattern don�t belong to a partition, it should be assigned to the cluster with the closest center
       int closestCluster = 0;
       double closestDistance = 1000000;
       
       //verifica se o padr�o ja esta em uma parti��o
       cpartition::t_Patterns clustersPatterns = p.getClustersPatterns();
       partitionIterator = clustersPatterns.find((*patIterator).first);
       if (partitionIterator == clustersPatterns.end()) 
       {
          map <int, double> distancesToCentroids;
          map <int, double>::iterator distanceIterator;
          for (c = clusterLabels.begin(); c != clusterLabels.end(); c++)
          {
              switch(similarity) 
              {
                  case euclidean:  distancesToCentroids[*c] = euclideanDistance(clustersCenters[*c], (*patIterator).second);
                  break;
           
                  case pearson: distancesToCentroids[*c] = pearsonDistance(clustersCenters[*c], (*patIterator).second); 
                  break;
               }
           }
           
           for (distanceIterator = distancesToCentroids.begin(); distanceIterator != distancesToCentroids.end(); distanceIterator++)
           {
               //distanceToCentroids
               if ((*distanceIterator).second < closestDistance)
               {
                   closestDistance = (*distanceIterator).second;
                   closestCluster = (*distanceIterator).first;
               }
           }
           p.patternInsert((*patIterator).first, closestCluster);        
           fileAlgorithmPartitionCompleted = algorithmPartitionsDir.string() + "/" + fs::basename(fileAlgorithmPartition) + "completed" + ".txt";
           p.printPartition(fileAlgorithmPartitionCompleted);
       }
   }
   
   return p;
}

// precisa fazer ...
cpartition variability::completePartition(dbscan alg, fs::path fileAlgorithmPartition)
{
//    fs::ifstream file;
//    fs::ifstream file2,file3;
//    char line[maxFileLine];
//    string line2, identifier, s;
//    vector<string> header;
//    fs::path fileAlgorithmPartitionCompleted;
//
//    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
//    boost::char_separator<char> sep("\t ");
//    std::map<std::string, vector<double>, cmp> pat;
//
//    // carrega dataset
//    file.open(dataset);
//    if (!file.good()) 
//    {
//        std::cout << "\nError in the file '" << dataset.string() << "'\n";
//        getchar();
//    }   
//    file.getline(line, maxFileLine);
//    line2 = line;
//    tokenizer tokens(line2, sep);
//    for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
//    {
//        //std::cout << *tok_iter << std::endl;
//        header.insert(header.end(), *tok_iter);
//    }
//    //header = line;
//    while (!file.eof())
//    {
//        file.getline(line, maxFileLine);
//        line2 = line;
//        tokens.assign(line2, sep);
//        for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
//        {
//            if (tok_iter == tokens.begin())
//            {
//               identifier = *tok_iter;
//               //std::cout << "ID: " << identifier << std::endl;
//            }
//            else
//            {
//                s = *tok_iter; 
//                //std::cout << s << std::endl;
//                pat[identifier].insert(pat[identifier].end(), atof(s.c_str())); 
//            }
//        }
//    }
//    file.close();
//        
//    sota::t_cells clustersCenters = alg.getCells();
//
//// imprime os centros dos clusters (c�lulas no sota)
////        std::cout << "Centros: " << std::endl; 
////        std::map<int, vector<double> >::iterator centerIterator;        
////        for (centerIterator = clustersCenters.begin(); 
////        centerIterator != clustersCenters.end(); 
////        centerIterator++)    
////        {
////            std::cout << "First " << (*centerIterator).first << std::endl;
////            for (vector<double>::iterator v = (*centerIterator).second.begin(); v != (*centerIterator).second.end(); ++v)
////            { 
////                std::cout << "Second " << *v << std::endl; 
////            }
////        }
//
//    std::map<std::string, std::vector<double>, cmp>::iterator patIterator;        
//    partition::t_iterator_Patterns partitionIterator;
//    std::set<int> clusterLabels;
//    std::set<int>::iterator c;
//    partition::partition p;
//    p =  alg.getPartition();
//    clusterLabels = p.getClustersLabels();
//    double distance;
//
//// imprime os rotulos dos clusters
////    std::set<int>::iterator lab;
////    for (lab = clusterLabels.begin(); lab != clusterLabels.end(); lab++)    
////    {
////        std::cout << *lab << std::endl;
////    }
//   
//   // finde the closest cluster and assign the pattern to this cluster
//   for (patIterator = pat.begin(); patIterator != pat.end(); patIterator++)    
//   {
//       // se o padr�o n�o pertence a parti��o, ele deve ser associado ao cluster com centro mais proximo
//       // if the pattern don�t belong to a partition, it should be assigned to the cluster with the closest center
//       int closestCluster = 0;
//       double closestDistance = 1000000;
//       
//       //verifica se o padr�o ja esta em uma parti��o
//       partition::t_Patterns clustersPatterns = p.getClustersPatterns();
//       partitionIterator = clustersPatterns.find((*patIterator).first);
//       if (partitionIterator == clustersPatterns.end()) 
//       {
//          map <int, double> distancesToCentroids;
//          map <int, double>::iterator distanceIterator;
//          for (c = clusterLabels.begin(); c != clusterLabels.end(); c++)
//          {
//              //std::cout << "Cluster: " << *c << std::endl;
//              switch(similarity) 
//              {
//                  case euclidean:  distancesToCentroids[*c] = euclideanDistance(clustersCenters[*c], (*patIterator).second);
//                  break;
//           
//                  case pearson: distancesToCentroids[*c] = pearsonDistance(clustersCenters[*c], (*patIterator).second); 
//                  break;
//               }
//           }
//           
//           for (distanceIterator = distancesToCentroids.begin(); distanceIterator != distancesToCentroids.end(); distanceIterator++)
//           {
//               //distanceToCentroids
//               if ((*distanceIterator).second < closestDistance)
//               {
//                   closestDistance = (*distanceIterator).second;
//                   closestCluster = (*distanceIterator).first;
//               }
//               //std::cout << "distances: " << (*distanceIterator).first << " - " << (*distanceIterator).second << std::endl;
//           }
//           //std::cout << "closest distance: " << (*patIterator).first << " - " << closestCluster << " - " << closestDistance << std::endl;
//           p.patternInsert((*patIterator).first, closestCluster);        
//           fileAlgorithmPartitionCompleted = algorithmPartitionsDir.string() + "/" + fs::basename(fileAlgorithmPartition) + "completed" + ".txt";
//                                             //algorithmPartitionsDir.string() + "/" + fs::basename(dataset) + 
//                                             //"AlgorithmPartitionCompleted" + sampleStr + ".txt";           
//           p.printPartition(fileAlgorithmPartitionCompleted);
//       }
//   }
//   
//   return p;
}
