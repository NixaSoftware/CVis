#include "validationIndex.h" 


//using namespace std;

//validationIndex::validationIndex()
//{
//}

//validationIndex::~validationIndex()
//{
//}

double validationIndex::calculate(t_indices index, cpartition Pe, cpartition Pr)
{
    double indexValue; 
    switch(index) 
    {
       case correctedRand_index: 
       {
           indexValue = correctedRand(Pe, Pr);
       }
       break;
    }

    return indexValue;
}


double validationIndex::correctedRand(cpartition Pe, cpartition Pr)
{ 
    double CR = 0;
           
    cpartition::t_iterator_Patterns currPe1, currPe2, currPr1, currPr2;

    int contingencyTable[Pe.nClusters()][Pr.nClusters()];
    int somaPe[Pe.nClusters()], somaPr[Pr.nClusters()];

    for (int i = 0; i < Pe.nClusters(); i++)
    {
        for (int j = 0; j < Pr.nClusters(); j++)
        {
            contingencyTable[i][j] = 0;
        }
    }
    
    for (int i = 0; i < Pe.nClusters(); i++)
    {
        somaPe[i] = 0;
    }
    
    for (int j = 0; j < Pr.nClusters(); j++)
    {
        somaPr[j] = 0;
    }
    
    for (currPe1 = Pe.first(); currPe1 != Pe.last(); currPe1++)
    {
        currPr1 = Pr.find((*currPe1).first);
        contingencyTable[(*currPe1).second - 1][(*currPr1).second - 1]++;
    }

    for (int i = 0; i < Pe.nClusters(); i++)
    {
        for (int j = 0; j < Pr.nClusters(); j++)
        {
            somaPe[i] += contingencyTable[i][j];
        }
    }

    for (int j = 0; j < Pr.nClusters(); j++)
    {
        for (int i = 0; i < Pe.nClusters(); i++)
        {
            somaPr[j] += contingencyTable[i][j];
        }
    }

    double T1 = 0;
    for (int i = 0; i < Pe.nClusters(); i++)
    {
        for (int j = 0; j < Pr.nClusters(); j++)
        {
            T1 += (contingencyTable[i][j] * contingencyTable[i][j] - contingencyTable[i][j])/2;
        }
    }
    
    double T2 = 0;
    for (int i = 0; i < Pe.nClusters(); i++)
    {
        T2 += (somaPe[i] * somaPe[i] - somaPe[i]) / 2;
    }
    
    double T3 = 0;
    for (int j = 0; j < Pr.nClusters(); j++)
    {
        T3 += (somaPr[j] * somaPr[j] - somaPr[j]) / 2;
    }
    
    int n = Pr.nPatterns();
    double T4 = (n * n - n) / 2;
      
    double T5 = (T2 * T3) / T4;
    
    CR = (T1 - T5) / ( (T2 + T3) / 2 - T5);    
    
    return CR;
}


// Silhouette
double validationIndex::silhouette(fs::path dataset, fs::path partitionFileName, t_similarities sim)
{       
    fs::path auxName;
    
    auxName = fs::basename(dataset) + "_silhouette.txt";
    fs::path silhoueteFileName = partitionFileName.branch_path() / auxName;
    
    fs::path silhouetteSummaryFileName = partitionFileName.branch_path() / (fs::basename(dataset) + "_silhouette_summary.txt");
    
    std::string command = "perl silhouetteVersao2.pl " + strSimilarities(sim) + " " + dataset.string() + " " + 
              partitionFileName.string() + " " + silhoueteFileName.string();
    system(command.c_str());
    
    fs::ifstream file;
    file.open(silhouetteSummaryFileName);
    if (!file.good()) 
    {
        std::cout << "\nError in the file " << silhouetteSummaryFileName.string() << std::endl;
        getchar();
    }
    
    int cluster;
    double silhouette, averageSilhouette;
    
    file.ignore(maxFileLine, '\n');
    while (!file.eof())
    {
        file >> cluster >> silhouette;
        if (!file.fail())
        {
            if (cluster == 0) averageSilhouette = silhouette;
        }
    }
    file.close();

    return averageSilhouette;
}



//// Homogeneity as in Shamir-2002 - Algorithmic approachs to clustering gene expression data
//double validationIndex::homogeneity(partition Pe, partition Pr)
//{ 
//}
//
//
//// Separation as in Shamir-2002 - Algorithmic approachs to clustering gene expression data
//double validationIndex::separation(partition Pe, partition Pr)
//{ 
//}
