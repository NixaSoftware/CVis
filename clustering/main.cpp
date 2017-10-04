#include <iostream>
#include <stdlib.h>
#include "cpartition.h"
#include "clustering.h"
#include "bootstrap.h"
#include "clusteringTypes.h"
#include "sota.h"
#include "kmeans.h"
#include "variability.h"

#include<cfloat>
#include <map>
#include <functional>
#include <iostream>
#include <sstream>
//#include <utility>

namespace fs = boost::filesystem;
using namespace std;



#include "boost/lexical_cast.hpp"

//****************
#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include "boost/filesystem/fstream.hpp"    // ditto
#include "boost/filesystem/convenience.hpp"    // ditto
//****************

void setParameters(std::vector<t_algorithms> &algorithms, t_similarities &similarity,
                   int &minK, int &maxK, int &nShufledFiles, int &nRuns, 
                   std::vector<int> &valuesNN,          
                   int &minStrong, int &maxStrong, int &stepStrong,
                   int &minTopic, int &maxTopic, int &stepTopic,
                   int &minNoise, int &maxNoise, int &stepNoise,
                   int &minMerge, int &maxMerge, int &stepMerge,
                   int &minLabel, int &maxLabel, int &stepLabel,
                   char distanceType, int nPatterns, int algType)
{         
    // cpartition &tp, cpartition &tpsc,
    //**********************
    // Para fazer testes preliminares do snn.  
    //**********************
    
//    int nPatterns = tp.nPatterns();
//    algorithms.clear();
//    algorithms.insert(algorithms.end(), snn_algorithm);
//    similarity = euclidean;
//    valuesNN.clear();
//    valuesNN.insert(valuesNN.end(), (int) ceil(nPatterns * 0.02)); // 2%
//    valuesNN.insert(valuesNN.end(), (int) ceil(nPatterns * 0.05)); // 5%
//    valuesNN.insert(valuesNN.end(), (int) ceil(nPatterns * 0.10)); // 10%
//    valuesNN.insert(valuesNN.end(), (int) ceil(nPatterns * 0.20)); // 20%
//    valuesNN.insert(valuesNN.end(), (int) ceil(nPatterns * 0.30)); // 30%
//    valuesNN.insert(valuesNN.end(), (int) ceil(nPatterns * 0.40)); // 40%
//    // To use the default value for parameter XXX, set the entries like this: 
//    // minXXX = -1, maxXXX = -1, stepXXX = 1 ou 10
//    minStrong = -1; maxStrong = -1; stepStrong = 10;
//    minTopic = 20; maxTopic = 80; stepTopic = 30;
//    minNoise = 0; maxNoise = 0; stepNoise = 10;
//    minMerge = 40; maxMerge = 80; stepMerge = 20;
//    minLabel = 0; maxLabel = 0; stepLabel = 10;


    //**********************
    // Para rodar os experimentos
    //**********************
    
	algorithms.clear();
	switch (algType){
	case 1:
		algorithms.insert(algorithms.end(), singlelink_algorithm);
		algorithms.insert(algorithms.end(), averagelink_algorithm);
		algorithms.insert(algorithms.end(), centroidlink_algorithm);
		algorithms.insert(algorithms.end(), completelink_algorithm);
		algorithms.insert(algorithms.end(), snn_algorithm);
		algorithms.insert(algorithms.end(), kmeans_algorithm);
		break;
	case 2: algorithms.insert(algorithms.end(), singlelink_algorithm); break;
	case 3: algorithms.insert(algorithms.end(), averagelink_algorithm); break;
	case 4: algorithms.insert(algorithms.end(), centroidlink_algorithm);break;
	case 5: algorithms.insert(algorithms.end(), completelink_algorithm);break;
	case 6: algorithms.insert(algorithms.end(), snn_algorithm);break;
	case 7: algorithms.insert(algorithms.end(), kmeans_algorithm); break;
	}




//    algorithms.insert(algorithms.end(), sota_algorithm);
//    algorithms.insert(algorithms.end(), dbscan_algorithm);

    if (distanceType == 'E')
    	similarity = euclidean;
    else
    	similarity = pearson;
                                         
    
     
    // n�mero de arquivos com ordens diferentes dos dados em que serao rodados os algoritmos n�o deterministicos
    nShufledFiles = 30;
//    nShufledFiles = 5;
     
    // n�mero de vezes que o k-means ser� executado para a escolha da melhor parti��o 
    // (parametro do software cluster 3.0)
    // should be 1 to the software run it only once
    nRuns = 30;

    // valores para o parametro NN do algoritmo snn
    valuesNN.clear();
    valuesNN.insert(valuesNN.end(), (int) ceil(nPatterns * 0.02)); // 2%
    if ((int) ceil(nPatterns * 0.05) < 1250)
      valuesNN.insert(valuesNN.end(), (int) ceil(nPatterns * 0.05)); // 5%
    if ((int) ceil(nPatterns * 0.10) < 1250)
      valuesNN.insert(valuesNN.end(), (int) ceil(nPatterns * 0.10)); // 10%
    if ((int) ceil(nPatterns * 0.20) < 1250)
      valuesNN.insert(valuesNN.end(), (int) ceil(nPatterns * 0.20)); // 20%
    if ((int) ceil(nPatterns * 0.30) < 1250)
      valuesNN.insert(valuesNN.end(), (int) ceil(nPatterns * 0.30)); // 30%
    if ((int) ceil(nPatterns * 0.40) < 1250)
      valuesNN.insert(valuesNN.end(), (int) ceil(nPatterns * 0.40)); // 40%

    // To use the default value for parameter XXX, set the entries like this: 
    // minXXX = -1, maxXXX = -1, stepXXX = 1 ou 10
    minStrong = -1; maxStrong = -1; stepStrong = 10;
    minTopic = 20; maxTopic = 80; stepTopic = 30;
    minNoise = 0; maxNoise = 0; stepNoise = 10;
    minMerge = 40; maxMerge = 80; stepMerge = 20;
    minLabel = 0; maxLabel = 0; stepLabel = 10;
}


// nRuns should be at least 1 
void runKmeans(fs::path datasetBaseName, fs::path baseDirExp,
         t_algorithms alg, t_similarities sim, int minK, int maxK, int nRunsS, int nRuns)
{
    fs::path expDir;
    fs::path expDirAux;
    fs::path dataset;
    fs::path errorFile;

    std::string parameters;
    std::string expDirName;

    dataset = datasetBaseName.branch_path().string() + "/" + fs::basename(datasetBaseName) + ".txt";
    std::map<int, std::map<int, cpartition> > algorithmPartition; // k, f
    std::map<int, std::map<int, fs::path> > files;

    for (int f = 1; f <= nRuns; f++)
    {
        
        for (int k = minK; k <= maxK; k++)
        {
            clustering c;
            //partition algorithmPartition;

            expDirName = fs::basename(dataset) + "-" + strAlgorithms(alg) + "-" 
                         + strSimilarities(sim) + "-k" + boost::lexical_cast<std::string>(k);                     
            expDir = baseDirExp / expDirName;
            if (!exists(expDir))
                fs::create_directory(expDir);
            expDirAux = expDir.string() + "/" + expDir.leaf().string() + "-Run" + boost::lexical_cast<std::string>(f);
            files[k][f] = expDirAux;
            
            fs::path auxName = expDirAux.leaf().string() + ".err";
            errorFile = expDirAux.string() / auxName;

    
            parameters = "-k " + boost::lexical_cast<std::string>(k) + 
                         " -r " + boost::lexical_cast<std::string>(nRunsS);
            
            //std::cout << "parameters" << parameters << std::endl;
            
            algorithmPartition[k][f] = c.runClustering(dataset, expDirAux, alg, sim, parameters);
        }
    }    
    
}



void runSota(fs::path datasetBaseName, fs::path baseDirExp,
             t_algorithms alg, t_similarities sim, int minK, int maxK, int nShufledFiles)
{
    //std::vector<clustering> clusterings;
    fs::path expDir;
    fs::path dataset;
    cpartition algorithmPartition;
    
    clustering c;
    std::string parameters;
    std::string expDirName;


    if (nShufledFiles == 0)
    {
        dataset = datasetBaseName.branch_path().string() + "/" + fs::basename(datasetBaseName) + ".txt";

        for (int k = minK; k <= maxK; k++)
        {
            clustering c;
            cpartition algorithmPartition;
    
            expDirName = fs::basename(dataset) + "-" + strAlgorithms(alg) + "-" 
                         + strSimilarities(sim) + "-k" + boost::lexical_cast<std::string>(k);                     
            expDir = baseDirExp;// / expDirName;
    
            parameters = " -cn " + boost::lexical_cast<std::string>(k-1); // k - 1 as cn is the number of cicles and
    
            algorithmPartition = c.runClustering(dataset, expDir, alg, sim, parameters);
    
        }    
    }    

    for (int f = 1; f <= nShufledFiles; f++)
    {
        clustering c;
        cpartition algorithmPartition;

        dataset = datasetBaseName.branch_path().string() + "/" + fs::basename(datasetBaseName) + 
                  "-f" + boost::lexical_cast<std::string>(f) + ".txt";

        for (int k = minK; k <= maxK; k++)
        {
    
            expDirName = fs::basename(dataset) + "-" + strAlgorithms(alg) + "-" 
                         + strSimilarities(sim) + "-k" + boost::lexical_cast<std::string>(k);                     
            expDir = baseDirExp / expDirName;
    
            parameters = " -cn " + boost::lexical_cast<std::string>(k-1); // k - 1 as cn is the number of cicles and
    
            algorithmPartition = c.runClustering(dataset, expDir, alg, sim, parameters);
    
        }
    }
}

// To use the default value for parameter XXX, set the entries like this:
// minXXX = -1 
// maxXXX = -1
// stepXXX = 1 ou 10
void runSnn(fs::path datasetBaseName, fs::path baseDirExp,
         t_algorithms alg, t_similarities sim, 
         std::vector<int> valuesNN,
         int minStrong, int maxStrong, int stepStrong,
         int minTopic, int maxTopic, int stepTopic,
         int minNoise, int maxNoise, int stepNoise,
         int minMerge, int maxMerge, int stepMerge,
         int minLabel, int maxLabel, int stepLabel)
{
    //std::vector<clustering> clusterings;
    fs::path expDir;
    fs::path dataset;
    
    //partition algorithmPartition;
    
    std::string parameters;
    std::string expDirName;
    
    dataset = datasetBaseName.branch_path().string() + "/" + fs::basename(datasetBaseName) + ".txt";

    parameters = "";
    float strong, topic, noise, merge, label;
              
//    for (int NN = minNN; NN <= maxNN; NN += stepNN)
    for (std::vector<int>::iterator it = valuesNN.begin(); it != valuesNN.end(); it++)
    {
        int NN = *it;
        for (int intstrong = minStrong; intstrong <= maxStrong; intstrong += stepStrong)
        {
            for (int inttopic = minTopic; inttopic <= maxTopic; inttopic += stepTopic)
            {
                for (int intnoise = minNoise; intnoise <= maxNoise; intnoise += stepNoise)
                {
                    for (int intmerge = minMerge; intmerge <= maxMerge; intmerge += stepMerge)
                    {
                        for (int intlabel = minLabel; intlabel <= maxLabel; intlabel += stepLabel)
                        {
                            clustering c; 
                            cpartition algorithmPartition;                            
                            expDirName = fs::basename(dataset) + "-" + strAlgorithms(alg) + "-" 
                                         + strSimilarities(sim);
                                         
                            parameters = "";

                            // para imprimir com precisao 2
                            std::stringstream ss;
                            ss.precision(2);

                            parameters += " -NN " + boost::lexical_cast<std::string>(NN); 
                            expDirName += "-NN" + boost::lexical_cast<std::string>(NN);
 
                            if (minStrong != -1)
                            {
                                strong = (float)intstrong/100;
                                ss.str("");
                                ss << strong;
                                parameters += " -strong " + ss.str();
                                expDirName += "-s" + ss.str();
                                //parameters += " -strong " + boost::lexical_cast<std::string>(strong);
                                //expDirName += "-strong" + boost::lexical_cast<std::string>(strong);
                            }    
    
                            if (minTopic != -1)
                            {
                                topic = (float)inttopic/100;
                                ss.str("");
                                ss << topic;
                                parameters += " -topic " + ss.str();
                                expDirName += "-t" + ss.str();
                                //parameters += " -topic " + boost::lexical_cast<std::string>(topic);
                                //expDirName += "-topic" + boost::lexical_cast<std::string>(topic);
                            }    
                                
                            if (minNoise != -1)
                            {
                                noise = (float)intnoise/100;
                                ss.str("");
                                ss << noise;
                                parameters += " -noise " + ss.str();
                                expDirName += "-n" + ss.str();
                                //parameters += " -noise " + boost::lexical_cast<std::string>(noise);
                                //expDirName += "-noise" + boost::lexical_cast<std::string>(noise);
                            }
                                                                
                            if (minMerge != -1)
                            {
                                merge = (float)intmerge/100;
                                ss.str("");
                                ss << merge;
                                parameters += " -merge " + ss.str();
                                expDirName += "-m" + ss.str();
                                //parameters += " -merge " + boost::lexical_cast<std::string>(merge);
                                //expDirName += "-merge" + boost::lexical_cast<std::string>(merge);
                            }    
                                
                            if (minLabel != -1)
                            {
                                label = (float)intlabel/100;
                                ss.str("");
                                ss << label;
                                //parameters += " -label " + ss.str();
                                //expDirName += "-label" + ss.str();
                                //parameters += " -label " + boost::lexical_cast<std::string>(label);
                                //expDirName += "-label" + boost::lexical_cast<std::string>(label);
                            }
                                                     
                            expDir = baseDirExp / expDirName;

                            algorithmPartition = c.runClustering(dataset, expDir, alg, sim, parameters);
     
                        }
                    }
                }
            }
        }
    }                    
}

void runDbscan(fs::path datasetBaseName, fs::path baseDirExp, 
               t_algorithms alg, t_similarities sim, 
               double minEps, double maxEps, double stepEps, 
               int minMinpts, int maxMinpts, int stepMinpts)
               
                
{
    //std::vector<clustering> clusterings;
    fs::path expDir;
    fs::path dataset;
    cpartition algorithmPartition;
    
    clustering c;
    std::string parameters;
    std::string expDirName;

    dataset = datasetBaseName.branch_path().string() + "/" + fs::basename(datasetBaseName) + ".txt";
    
    for (double eps = minEps; eps <= maxEps; eps += stepEps)
    {
        for (int minpts = minMinpts; minpts <= maxMinpts; minpts += stepMinpts)        
        {
            clustering c;
            cpartition algorithmPartition;
    
            std::string epsStr = boost::lexical_cast<std::string>(eps);
            std::string minptsStr = boost::lexical_cast<std::string>(minpts);
        
            expDirName = fs::basename(dataset) + "-" + strAlgorithms(alg) + "-" 
                         + strSimilarities(sim) + "_eps" + epsStr + "_minpts" + minptsStr;                     
            expDir = baseDirExp / expDirName;

            parameters = " -eps " + epsStr + " -minpts " + minptsStr;

            algorithmPartition = c.runClustering(dataset, expDir, alg, sim, parameters);

        }
    }
}

void runAllLink(fs::path datasetBaseName, fs::path baseDirExp,
                   t_algorithms alg, t_similarities sim, int minK, int maxK)
{
    //std::vector<clustering> clusterings;
    fs::path expDir;
    fs::path dataset;
    cpartition algorithmPartition;
    
    clustering c;
    std::string parameters;
    std::string expDirName;

    dataset = datasetBaseName.branch_path().string() + "/" + fs::basename(datasetBaseName) + ".txt";

    for (int k = minK; k <= maxK; k++)
    {
        clustering c;
        cpartition algorithmPartition;

        expDirName = fs::basename(dataset) + "-" + strAlgorithms(alg) + "-" 
                     + strSimilarities(sim) + "-k" + boost::lexical_cast<std::string>(k);                     
        expDir = baseDirExp / expDirName;

        parameters = " -cut " + boost::lexical_cast<std::string>(k);

        algorithmPartition = c.runClustering(dataset, expDir, alg, sim, parameters);
    }
}

void runAlgorithms(fs::path dataset, fs::path expDir, 
                   char distanceType, int minK, int maxK, int nPatterns, int algType)
{   
    std::vector<t_algorithms> algorithms;
    t_similarities similarity;
    std::vector<int> valuesNN;
    int nShufledFiles; int nRuns;
    int minStrong; int maxStrong; int stepStrong;
    int minTopic; int maxTopic; int stepTopic;
    int minNoise; int maxNoise; int stepNoise;
    int minMerge; int maxMerge; int stepMerge;
    int minLabel; int maxLabel; int stepLabel;

    fs::create_directory(expDir);
    
    setParameters(algorithms, similarity, minK, maxK, nShufledFiles, nRuns, valuesNN,
                  minStrong, maxStrong, stepStrong, minTopic, maxTopic, stepTopic,
                  minNoise, maxNoise, stepNoise, minMerge, maxMerge, stepMerge,
                  minLabel, maxLabel, stepLabel, distanceType, nPatterns, algType);

    std::cout << "Running clustering for dataset: " << fs::basename(dataset) << std::endl;

    for (std::vector<t_algorithms>::iterator algorithmIt = algorithms.begin();
         algorithmIt != algorithms.end(); algorithmIt++)
    {
         switch(*algorithmIt) 
         {
               case sota_algorithm:   
               {
                   std::cout << "Running sota..." << std::endl;            
                   runSota(dataset, expDir, *algorithmIt, similarity, minK, maxK, nShufledFiles);
                   break;
               }    
               case kmeans_algorithm:         
               {
                   std::cout << "Running k-means..." << std::endl;
                   runKmeans(dataset, expDir, *algorithmIt, similarity, minK, maxK, nRuns, nShufledFiles);
                   break;
               }    
               case singlelink_algorithm:    
               {
                   std::cout << "Running single-link..." << std::endl;            
                   runAllLink(dataset, expDir, *algorithmIt, similarity, minK, maxK);
                   break;
               }    
               case dbscan_algorithm:         
               {
                   std::cout << "Not implemented yet." << std::endl;
                   break;
               }    
               case snn_algorithm:            
               {
                   std::cout << "Running snn..." << std::endl;
                   runSnn(dataset, expDir, *algorithmIt, similarity, valuesNN, // NN
                          minStrong, maxStrong, stepStrong, // Strong
                          minTopic, maxTopic, stepTopic, // Topic
                          minNoise, maxNoise, stepNoise, // Noise
                          minMerge, maxMerge, stepMerge, // Merge
                          minLabel, maxLabel, stepLabel); // Label
                   break;
               }  
               case averagelink_algorithm:    
               {
                   std::cout << "Running average-link..." << std::endl;            
                   runAllLink(dataset, expDir, *algorithmIt, similarity, minK, maxK);
                   break;
               }
               case centroidlink_algorithm:
               {
                   std::cout << "Running centroid-link..." << std::endl;
                   runAllLink(dataset, expDir, *algorithmIt, similarity, minK, maxK);
                   break;
               }
               case completelink_algorithm:
               {
                   std::cout << "Running complete-link..." << std::endl;
                   runAllLink(dataset, expDir, *algorithmIt, similarity, minK, maxK);
                   break;
               }

         }
   }
}

int nClusters(fs::path partitionFileName)
{
    fs::ifstream file;

    std::string id;
    int clu;

    std::set <int> clusters;
    std::set <int>::iterator clustersIt;
    file.open(partitionFileName);
    if (!file.good())
    {
        std::cout << "\nError in the file '" << partitionFileName.string() << "'\n";
        getchar();
        exit(-1);
    }
    while (!file.eof())
    {
        file >> id >> clu;
        if (!file.fail())
        {
            clustersIt = clusters.find(clu);
            if (clustersIt == clusters.end())
                clusters.insert(clu);
        }
    }
    file.close();
    return clusters.size();
}


void obtemTodasParticoes(fs::path dataset, fs::path dirIndividual, fs::path dirAllPartitions, int minK, int maxK)
{
	string alg = "SNN";
	string algkm = "KM";
	string::size_type pos;
	fs::path partitionFileName;
	fs::path newPartitionFileName;


	if (!exists(dirAllPartitions))
		fs::create_directory(dirAllPartitions);

	if (!exists(dirIndividual))
	{
		std::cout << "directory of individual experiments not found: " << dirIndividual.string() << std::endl;
	}

	fs::directory_iterator endDir;
	for (fs::directory_iterator dirIt(dirIndividual); dirIt != endDir; dirIt++)
	{
		//d::cout << (*dirIt).string() << std::endl;
		if (is_directory(*dirIt))
		{
			//std::cout << (*dirIt).path().filename().string() << std::endl;

			pos = (*dirIt).path().filename().string().find(algkm,0);
			//      std::cout << pos << " " << string::npos << std::endl;
			if (pos == string::npos)
			{
				partitionFileName = dirIndividual.string() + "/" + (*dirIt).path().filename().string() + "/" + (*dirIt).path().filename().string() + ".clu";
				newPartitionFileName = dirAllPartitions.string() + "/" + partitionFileName.leaf().string();
				//std::cout << "partitionFileName " << partitionFileName.string() << std::endl;
				//std::cout << "newPartitionFileName " << newPartitionFileName.string() << std::endl;
				std::string name = partitionFileName.leaf().string();
				int nClu = nClusters(partitionFileName);
				if ((nClu >= minK) && (nClu <= maxK))
				{
					copy_file(partitionFileName, newPartitionFileName);
				}
				//d::cout << "arq copiado: " << newPartitionFileName.string() << std::endl;
			}
			else {
				// KM - escolher o de menor erro
				//std::cout << (*dirIt).path().filename().string() << std::endl;
				int n = 0;
				float e, eMenor = FLT_MAX;
				fs::path errorFile;
				fs::directory_iterator endDirKM;
				for (fs::directory_iterator dirIt2(*dirIt); dirIt2 != endDirKM; dirIt2++){
					//std::cout << (*dirIt2).path().filename().string() << std::endl;

					fs::path auxName = (*dirIt2).path().filename().string() + ".err";
					errorFile = (*dirIt2) / auxName;
					//cout << errorFile << endl;

					fs::ifstream err;
					err.open(errorFile);
					if (!err.good())
					{
						std::cout << "\nError in the file '" << errorFile.string() << "'\n";
						getchar();
					}
					err >> n >> e;
					if (e < eMenor){
						eMenor = e;
						partitionFileName = (*dirIt2).path().string() + "/" + (*dirIt2).path().leaf().string() + ".clu";
						newPartitionFileName = dirAllPartitions.string() + "/" + partitionFileName.leaf().string();
						//std::cout << "partitionFileName: " << partitionFileName.string() << std::endl;
						//std::cout << "newPartitionFileName " << newPartitionFileName.string() << std::endl;
					}
					//            std::cout << e << std::endl;
					err.close();
				}
				copy_file(partitionFileName, newPartitionFileName);
			}
		}
	}
}

int main(int argc, char **argv)
{
	srand ( time(NULL) );

	fs::path dataset;
	fs::path expDir;
	char distanceType;
	int minK, maxK;
	int nPatterns;
	int algType;

	runningSystem = mylinux;

	if (argc < 6)
	{
		std::cout << "Usage: ./clustering <E or P> <numObjects> <minK> <maxK> <dataset> <exp dir> <0-get part, 1-all, 2-SL, 3- AL, 4- CeL, 5-CoL, 6-SNN, 7-KM>\n" << std::endl;
		return -1;
	}
	distanceType = argv[1][0];
	nPatterns = atoi(argv[2]);
	minK = atoi(argv[3]);
	maxK = atoi(argv[4]);
	dataset = argv[5];
	expDir = argv[6];
	algType = atoi(argv[7]);

	if (!exists(expDir))
		fs::create_directory(expDir);

	fs::path dirDS = expDir/"indivAlgs";
	if (!exists(dirDS))
		fs::create_directory(dirDS);

	fs::path dirPart = expDir/"allPart";
	if (!exists(dirPart))
		fs::create_directory(dirPart);

	std::cout << "dataset: " << dataset << std::endl;
	std::cout << "expDir: " << expDir << std::endl;
	std::cout << "Results of algorithms in: " << dirDS << std::endl;
	std::cout << "All partitions in: " << dirPart << std::endl;

	if (algType != 0) {
		runAlgorithms(dataset, dirDS, distanceType, minK, maxK, nPatterns, algType);
	}

	std::cout << "Copying partitions to algorithmsPartitions directory..." << std::endl;

	//copy all files in a directory
	fs::path dirInitialPar = expDir.parent_path().string() + "/" + dataset.stem().string() + "-algPartitions";
	
	obtemTodasParticoes(dataset, dirDS, dirPart, minK, maxK);

	std::cout << "Done." << std::endl;
	return 0;
}

/*
int main(int argc, char **argv)
{
	srand ( time(NULL) );

	fs::path dataset;
	fs::path expDir;
	char distanceType;
	int minK, maxK;
	int nPatterns;
	int algType;

	runningSystem = mylinux;

	if (argc < 6)
	{
		std::cout << "Usage: ./clustering <E or P> <numObjects> <minK> <maxK> <dataset> <exp dir> <0-get part, 1-all, 2-SL, 3- AL, 4- CeL, 5-CoL, 6-SNN, 7-KM>\n" << std::endl;
		return -1;
	}
	distanceType = argv[1][0];
	nPatterns = atoi(argv[2]);
	minK = atoi(argv[3]);
	maxK = atoi(argv[4]);
	dataset = argv[5];
	expDir = argv[6];
	algType = atoi(argv[7]);

	std::cout << "dataset: " << dataset << std::endl;
	std::cout << "expDir: " << expDir << std::endl;

	fs::path dirDS = expDir;
	if (!exists(dirDS))
		fs::create_directory(dirDS);

	if (algType != 0) {
		runAlgorithms(dataset, expDir, distanceType, minK, maxK, nPatterns, algType);
	}

	std::cout << "Copying partitions to algorithmsPartitions directory..." << std::endl;

	//copy all files in a directory
	//fs::path dirInitialPar = expDir.parent_path().string() + "/" + dataset.stem().string() + "-algPartitions";
	fs::path dirInitialPar = expDir.parent_path().string() + "/" + expDir.stem().string() + "-algPartitions";
	std::cout << "dataset: " << dataset << std::endl;
	std::cout << "expDir: " << expDir << std::endl;

	obtemTodasParticoes(dataset, expDir, dirInitialPar, minK, maxK);

	std::cout << "Done." << std::endl;
	return 0;
}
*/

