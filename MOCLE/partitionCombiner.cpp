#include <cstdlib>
#include <iostream>

#include "combiner.h"
#include "tPatterns.h"
#include "tPartition.h"
//#include "TPartitionSolution.h"
//#include "tPartition.h"


#include <boost/filesystem.hpp>


namespace fs = boost::filesystem;
using namespace std;

//typedef enum{mywindows, mylinux} t_system;
//t_system runningSystem;



std::string experiment;


void run(fs::path expDir, fs::path dirInitialPop, fs::path dataset, tPartition *tp)
{
    NumberOfObjectives = 8;
    combiner myCombiner(dataset, expDir, dirInitialPop, tp);
    int nGenerations = 50;
    double mutationProbability = 0;
    myCombiner.runCombiner(nGenerations, mutationProbability); // nGenerations, mutationProbability
}

int main(int argc, char **argv)
{
    passo = 0;
//    srand ( time(NULL) * clock() ); 
    srand((unsigned)time(0));
    
//    runningSystem = mywindows;
    runningSystem = mylinux;


	if (argc < 4)
	{
		std::cout << "Usage: ./mocle <Crossover Type: 1- mcla or 2 - bipartite> <Dataset> <Initial Population Directory> <Results Directory> <True Partition (can be a fake one if not known)> \n" << std::endl;
		return -1;
	}

	int crossoverTypeName = atoi(argv[1]);
	string datasetName = argv[2];
	string dirInitialPopName = argv[3];
	string expDirName = argv[4];
	string truePartitionName = argv[5];// if using correctedRand_obj objective, one should provide the true partition for the run method
	                               // can be a fake one if not known

	cout << crossoverTypeName<< endl
			<<datasetName << endl
			<<dirInitialPopName << endl
			<< expDirName<< endl
			<<truePartitionName << endl;
	if (crossoverTypeName == 1)
		crossoverType = mcla;
	else if (crossoverTypeName == 2)
		crossoverType = bipartite;
	else {
		cout << "Invalid crossover";
		return -1;
	}

    objType.insert(objType.end(), deviation_obj);
    objType.insert(objType.end(), connectivity_obj);
    //objType.insert(objType.end(), nClusters_obj);
    //objType.insert(objType.end(), informationGain_obj);
    //objType.insert(objType.end(), correctedRand_obj); // if using correctedRand_obj objective, one should provide the true partition for the run method
    //objType.insert(objType.end(), purity_obj);
//    objType.insert(objType.end(), deviationPearson_obj);
//    objType.insert(objType.end(), connectivityPearson_obj);


    fs::path dataset = datasetName;
    fs::path expDir = expDirName;
    fs::path dirInitialPop = dirInitialPopName;
    fs::path truePartition = truePartitionName;

    if (!exists(expDir))
            fs::create_directory(expDir);

    tPartition tp(dataset, truePartition); // if using correctedRand_obj objective, one should provide and load the true partition for the run method
    run(expDir, dirInitialPop, dataset, &tp);

	
    return EXIT_SUCCESS;
}
