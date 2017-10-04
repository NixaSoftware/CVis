#include "clustering.h"
#include "validationIndex.h"

//extern "C" {
//#include "cluster.h"
//##include "data.h"
//}

using std::string;

using namespace std;

clustering::clustering() 
{
    dataset = "";
    experimentsDir = ""; 
    fileResult = "";
    algorithm = undefined_algorithm;
    similarity = undefined_similarity; 
    parameters = "";
    valid = false;
    runEV = false;
}

//clustering::clustering(fs::path fileName, t_algorithms alg, t_similarities sim) 
//{
//    //p.loadPartition(fileName.string());
//    algorithm = alg;
//    similarity = sim;    
//    parameters = "";
//}


//clustering::~clustering() 
//{
//}


clustering::clustering(const clustering &c)
{
    valid = c.valid;
    runEV = c.runEV;
    dataset = c.dataset;
    experimentsDir = c.experimentsDir; 
    fileResult = c.fileResult;
    algorithm = c.algorithm;
    similarity = c.similarity;
    parameters = c.parameters;
    p = c.p;
    truePartition = c.truePartition;
}

clustering & clustering::operator= (const clustering &c)
{
    valid = c.valid;
    runEV = c.runEV;
    dataset = c.dataset;
    experimentsDir = c.experimentsDir; 
    fileResult = c.fileResult;
    algorithm = c.algorithm;
    similarity = c.similarity;
    parameters = c.parameters;
    p = c.p;
    truePartition = c.truePartition;
}

cpartition clustering::loadPartition(fs::path partitionFile)
{
    p.loadPartition(partitionFile);
    return p;
}

                     
cpartition clustering::runClustering(fs::path datasetFile, fs::path expDir, t_algorithms alg, t_similarities sim, string param)
{

    dataset = datasetFile;
    experimentsDir = expDir,     
    algorithm = alg;
    similarity = sim; 
    parameters = param;

    fs::create_directory(experimentsDir);
    
    //fs::path resultDir(experimentsDir / "results"); // TR01
    fs::path resultDir(experimentsDir);
    //std::cout << resultDir.string() << std::endl;
    fs::create_directory(resultDir);

    //fs::path auxName(fs::basename(dataset) + "Result.txt"); // TR01
//    fs::path auxName(experimentsDir.leaf() + "-Result"); 
    fs::path auxName(experimentsDir.leaf()); 
    fileResult = resultDir.string() / auxName;
    
//    std::cout << fileResult.string() << std::endl;
//    getchar();   


   switch(algorithm) 
   {
      case sota_algorithm:   
           {
               // if the algorithm is sota, the file is changed to the apropriated format
               // all experiments should be run with this new file
               //dataset = datasetSota(datasetFile);
               dataset = datasetFile.branch_path().string() + "/" + fs::basename(datasetFile) + "SOTA.txt";//datasetSota(datasetFile);
               p = runSota(); break;
           }
      case kmeans_algorithm:         p = runKmeans(); break;
      case singlelink_algorithm:     p = runSingleLink(); break;
      case dbscan_algorithm:         p = runDbscan(); break;
      case snn_algorithm:            p = runSnn(); break;
      case averagelink_algorithm:     p = runAverageLink(); break;
      case centroidlink_algorithm:     p = runCentroidLink(); break;
      case completelink_algorithm:     p = runCompleteLink(); break;
    }

   return p;
}

fs::path clustering::datasetSota(fs::path datasetFile)
{
    char line[maxFileLine];
    fs::ofstream res;
    fs::ifstream file;
    
    fs::path sotaFileName;
    
    string auxName = fs::basename(datasetFile) + "SOTA.txt";
    sotaFileName = experimentsDir / auxName; //datasetFile.branch_path() / auxName;
    
    file.open(datasetFile);
    res.open(sotaFileName);
    if (!res.good()) 
    {
        std::cout << "\nError in the file '" << sotaFileName.string() << "'\n";
        getchar();
    }
    if (!file.good()) 
    {
        std::cout << "\nError in the file '" << datasetFile.string() << "'\n";
        getchar();
    }

    res << "#";
   
    while (!file.eof())
    {
        file.getline(line, maxFileLine);
        {
            res << line << std::endl;
        }
    }
    file.close();
    res.close();

    return sotaFileName;
}

cpartition clustering::runSota()
{
    sota alg(dataset.string(), fileResult.string(), similarity, parameters);
    p = alg.run();
    return p;
}

cpartition clustering::runKmeans()
{
    kmeans alg(dataset.string(), fileResult.string(), similarity, parameters);
    p = alg.run();
    return p;
}

cpartition clustering::runSingleLink()
{
    singlelink alg(dataset.string(), fileResult.string(), similarity, parameters);
    p = alg.run();
    return p;
}

cpartition clustering::runDbscan()
{
    dbscan alg(dataset.string(), fileResult.string(), similarity, parameters);
    p = alg.run();
    return p;
}

cpartition clustering::runSnn()
{
    snn alg(dataset.string(), fileResult.string(), similarity, parameters);
    p = alg.run();
    return p;
}

cpartition clustering::runAverageLink()
{
    averagelink alg(dataset.string(), fileResult.string(), similarity, parameters);
    p = alg.run();
    return p;
}

cpartition clustering::runCentroidLink()
{
    centroidlink alg(dataset.string(), fileResult.string(), similarity, parameters);
    p = alg.run();
    return p;
}

cpartition clustering::runCompleteLink()
{
    completelink alg(dataset.string(), fileResult.string(), similarity, parameters);
    p = alg.run();
    return p;
}

cpartition clustering::random(fs::path dataset, fs::path fileResult, int k)
{
    algorithm = undefined_algorithm;
    similarity = undefined_similarity; 
    parameters = "";

    p.randomPartition(dataset, fileResult, k);
    algorithm = undefined_algorithm;
    similarity = undefined_similarity;
    return p;

}
/*
double clustering::runExternalValidation(cpartition rP, double alpha, t_indices ind, int B, fs::path bootstrapDir)
{
    if (algorithm == undefined_algorithm)
    {
        std::cout << "You should run an algorithm first." << std::endl;
        getchar();
        return -10;
    }
    
    externalValidation val;
    valid = val.validate(dataset, experimentsDir, bootstrapDir, rP, p, alpha, ind, B, algorithm, similarity, parameters);
    double indValue = val.indexValue();
    runEV = true;
    
    return indValue;
    
}*/

double clustering::runVariability(int B, t_indices ind, fs::path bootstrapDir)
{
    if (algorithm == undefined_algorithm)
    {
        std::cout << "You should run an algorithm first." << std::endl;
        getchar();
        return -10;
    }
    
    variability var;
    double variabilityValue = var.calculateVariability(dataset.string(), experimentsDir, bootstrapDir, B, algorithm, similarity, ind, p.nClusters(), parameters);

    return variabilityValue;
}

bool clustering::isValid()
{
    if (!runEV)
    {
        std::cout << "You should run external validation first." << std::endl;
        getchar();
    }
    return valid;
}

double clustering::calculateSilhouette(fs::path partitionFileName)
{
    double averageSilhouette;
    if (algorithm == undefined_algorithm)
    {
        std::cout << "You should run an algorithm first." << std::endl;
        getchar();
        return -10;
    }
    validationIndex index;
    averageSilhouette = index.silhouette(dataset, partitionFileName, similarity);
    
    return averageSilhouette;
}

