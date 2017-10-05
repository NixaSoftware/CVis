#include "validationIndices.h"

double cr(tPartition &Pe, tPartition &Pr)
{ 
    double CR = 0;
           
    cCluster::tPatternsIt patItPe, patItPr;

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
//    std::cout << "Numero clusters Pe: " << Pe.nClusters() << std::endl;
//    std::cout << "Numero clusters Pr: " << Pr.nClusters() << std::endl;
//    std::cout << "Numero padroes Pe: " << Pe.nPatterns() << std::endl;
//    std::cout << "Numero padroes Pr: " << Pr.nPatterns() << std::endl;
//
//    getchar();
    
//    for (currPe1 = Pe.first(); currPe1 != Pe.last(); currPe1++)
//    {
//        currPr1 = Pr.find((*currPe1).first);
//        contingencyTable[(*currPe1).second - 1][(*currPr1).second - 1]++;
//    }

   // Parte nova:
   for (int i = 0; i < Pe.nClusters(); i++)
   {
      for (int j = 0; j < Pr.nClusters(); j++)
      {
         for (patItPe = Pe.clusters[i].patterns.begin(); patItPe != Pe.clusters[i].patterns.end(); patItPe++)
         {
            patItPr = Pr.clusters[j].patterns.find(*patItPe);
            if (patItPr != Pr.clusters[j].patterns.end())
               contingencyTable[i][j]++;
         }
      }
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
    
//    std::cout << "Partition de tamanho: " << n << std::endl;
//    Pr.printPartition();
//    std::cout << "N: " << n << std::endl;
    
    double T5 = (T2 * T3) / T4;
    
    CR = (T1 - T5) / ( (T2 + T3) / 2 - T5);
    
//    std::cout << "T1 = " << T1 << std::endl;
//    std::cout << "T2 = " << T2 << std::endl;
//    std::cout << "T3 = " << T3 << std::endl;
//    std::cout << "T4 = " << T4 << std::endl;
//    std::cout << "T5 = " << T5 << std::endl << std::endl;
//    std::cout << "CR = " << CR << std::endl << std::endl;
//    
//    std::cout << "T1 - T5 = " << T1 - T5 << std::endl;
//    std::cout << "T2 + T3 = " << T2 + T3 << std::endl;
//    std::cout << "T2 * T3 = " << T2 * T3 << std::endl << std::endl;
//    
//   std::cout << "Contingency table: " << std::endl;
//    for (int i = 0; i < Pe.nClusters(); i++)
//    {
//        for (int j = 0; j < Pr.nClusters(); j++)
//        {
//            std::cout << contingencyTable[i][j] << " ";
//        }
//        std::cout << std::endl;
//    }
//    
//    std::cout << std::endl << std::endl;
//    
//    std::cout << "ni.: " << std::endl;
//    
//    for (int i = 0; i < Pe.nClusters(); i++)
//    {
//        std::cout << somaPe[i] << " ";
//    }
//    std::cout << std::endl;
//
//    std::cout << "n.j: " << std::endl;    
//    for (int j = 0; j < Pr.nClusters(); j++)
//    {
//        std::cout << somaPr[j] << " ";
//    }
//    std::cout << std::endl << std::endl;     
   
    return CR;
}

double connectivity(tPartition &Pe)
{ 
    double conn = 0;
    int nPat = Pe.nPatterns();
    float nn = ceil (nPat * 0.05); // number of nearest neighbors is 5% of the size of the dataset
    int nNearestNeighbors = int(nn);
    //std::cout << "nn: " << nNearestNeighbors << std::endl;
  
    //std::cout << "Pattern " << (*it1).patternNumber << std::endl;
	//std::cout << " j-esimo vizinho " << "1/j" << "somaNN" << "conn" << std::endl;
    for (tPatterns::tPatternsDescriptionIt it1 = Pe.patterns->patternsDescription.begin();
         it1 != Pe.patterns->patternsDescription.end(); it1++)
    {
		//std::cout << "Pattern " << (*it1).patternNumber;
        double somaNN = 0;
        for (int j = 0; j < nNearestNeighbors; j++)
        {
            if (Pe.clusterOf((*it1).patternNumber) !=  
                Pe.clusterOf(Pe.patterns->nnList[(*it1).patternNumber][j]))
                somaNN += 1.0/(j+1); // j + 1 porque j � 0 para o 1o vizinho... 
			//std::cout << " " << j + 1;
			//std::cout << " " << 1.0/(j+1) << " s" << somaNN;
        }    
        conn += somaNN;
		//std::cout << " c " << conn << std::endl;
    }    
   
	//std::cout << "Connectivity: " << conn << std::endl << std::endl;
	//getchar();
   return conn; 
}

// this objective is the overall deviation of a partitioning, and 
// measure the cluster compactness (Handl-2004, Multiobjective clustering with 
// automatic determination of the number of clusters
double deviation(tPartition &Pe)
{ 
    
    double dev = 0;
    std::map<int, std::vector<double> > *centroids;
    std::map<int, std::vector<double> >::iterator centroidsIt;
    centroids = Pe.getCentroids();
    for (tPartition::tClustersIt itClu = Pe.clusters.begin(); 
         itClu != Pe.clusters.end(); itClu++)
    {
        //std::cout << "Cluster: " << (*itClu).clusterLabel << std::endl;
        int nAttributes = Pe.patterns->nAttributes();
      
        // calculate the summed distances for each pattern of a cluster
        for (cCluster::tPatternsIt itPat = (*itClu).patterns.begin();
             itPat != (*itClu).patterns.end(); itPat++)
        {
            //std::vector<double> pat = Pe.patterns.patternValues(*itPat);
            //centroids[(*itClu).clusterLabel];
            
            centroidsIt = centroids->find((*itClu).clusterLabel);
            double euclidean = euclideanDistance(Pe.patterns->patternValues(*itPat), (*centroidsIt).second);
            //double euclidean = euclideanDistance(Pe.patterns->patternValues(*itPat), centroids[(*itClu).clusterLabel].second);
            //std::cout << "Pattern: " << (*itPat) << " - distance: " << euclidean << std::endl;
            dev += euclidean;
        }    
    }    

   //std::cout << "dev: " << dev << std::endl << std::endl;
   //getchar();
   return dev; 

//    double dev = 0;
//    std::map<int, std::vector<double> > centroid;
//    for (tPartition::tClustersIt itClu = Pe.clusters.begin(); 
//         itClu != Pe.clusters.end(); itClu++)
//    {
//        //std::cout << "Cluster: " << (*itClu).clusterLabel << std::endl;
//        int nAttributes = Pe.patterns->nAttributes();
//        // calculate the centroid of cluster itClu
//        centroid[(*itClu).clusterLabel].resize(nAttributes);
//        //std::cout << "centroide: ";
//        for (int a = 0; a < nAttributes; a++)
//        {
//            
//            //std::cout << "Cluster: " << (*itClu).clusterLabel << std::endl << "Patterns: ";
////            getchar();
//            //centroid[(*itClu).clusterLabel].insert(centroid[(*itClu).clusterLabel].end(), 0);
//            centroid[(*itClu).clusterLabel][a] = 0;
//            for (cluster::tPatternsIt itPat = (*itClu).patterns.begin();
//                 itPat != (*itClu).patterns.end(); itPat++)
//            {
//                centroid[(*itClu).clusterLabel][a] += Pe.patterns->patternValues(*itPat)[a];
//                //std::cout << (*itPat) << " " << centroid[(*itClu).clusterLabel][a] << std::endl;
//            }    
//            centroid[(*itClu).clusterLabel][a] /= (*itClu).nPatterns();
//            //std::cout << " " << centroid[(*itClu).clusterLabel][a];
//        }    
//        //std::cout << std::endl;
//        
//        //getchar();
//        // calculate the summed distances for each pattern of a cluster
//        for (cluster::tPatternsIt itPat = (*itClu).patterns.begin();
//             itPat != (*itClu).patterns.end(); itPat++)
//        {
//            //std::vector<double> pat = Pe.patterns.patternValues(*itPat);
//            double euclidean = euclideanDistance(Pe.patterns->patternValues(*itPat), centroid[(*itClu).clusterLabel]);
//            //std::cout << "Pattern: " << (*itPat) << " - distance: " << euclidean << std::endl;
//            dev += euclidean;
//        }    
//    }    
//
//   //std::cout << "dev: " << dev << std::endl << std::endl;
//   //getchar();
//   return dev; 
}

double informationGain(tPartition &Pe, tPartition *truePartition)
{ 

  double s1 = 0, s2 = 0, s3 = 0;
  int nPat = truePartition->nPatterns();
  
//  std::cout << "nPat: " << nPat << std::endl;
  
  for (int i = 0; i < truePartition->nClusters(); i++)
  {
      double pc = (double)truePartition->clusters[i].nPatterns() / nPat;
//      std::cout << "Pc: " << pc << std::endl;
      if (pc != 0) s1 += pc * log2(pc);
  }
//  std::cout << "S1: " << s1 << std::endl;
  
  for (int j = 0; j < Pe.nClusters(); j++)
  {
      int nElemCluster = Pe.clusters[j].nPatterns();
      double pk = (double)nElemCluster / nPat;
      
//      std::cout << "pk: " << pk << std::endl;
      
      s2 = 0;
      for (int i = 0; i < truePartition->nClusters(); i++)
      {
          cCluster::tPatternsIt it = truePartition->clusters[i].patterns.begin();
          
//          std::cout << truePartition.patterns.patternClass(*it) << std::endl;
          double pck = (double)Pe.nPatternsClass(truePartition->patterns->patternClass(*it), Pe.clusters[j]) / nElemCluster;
//          std::cout << "Pck: " << pck << std::endl;
          if (pck != 0) s2 += pck * log2(pck);
      }
//      std::cout << "S2: " << s2 << std::endl;

      s3 += pk * s2;
  }
//  std::cout << "S3: " << s3 << std::endl;

   double gain = - s1 + s3;
   
//   std::cout << "Gain: " << gain << std::endl;
//   getchar();
   return gain; 
}


double connectivityPearson(tPartition &Pe)
{ 
    double conn = 0;
    int nPat = Pe.nPatterns();
    float nn = ceil (nPat * 0.05); // number of nearest neighbors is 5% of the size of the dataset
    int nNearestNeighbors = int(nn);
    //std::cout << "nn: " << nNearestNeighbors << std::endl;
  
//    std::cout << "Pattern " << (*it1).patternNumber << std::endl;
    //std::cout << " j-esimo vizinho " << "1/j" << "somaNN" << "conn" << std::endl;
    for (tPatterns::tPatternsDescriptionIt it1 = Pe.patterns->patternsDescription.begin();
         it1 != Pe.patterns->patternsDescription.end(); it1++)
    {
        //std::cout << "Pattern " << (*it1).patternNumber;
        double somaNN = 0;
        for (int j = 0; j < nNearestNeighbors; j++)
        {
            if (Pe.clusterOf((*it1).patternNumber) !=  
                Pe.clusterOf(Pe.patterns->nnListPearson[(*it1).patternNumber][j]))
                somaNN += 1.0/(j+1); // j + 1 porque j � 0 para o 1o vizinho... 
            //std::cout << " " << j + 1;
            //std::cout << " " << 1.0/(j+1) << " s" << somaNN;
        }    
        conn += somaNN;
        //std::cout << " c " << conn << std::endl;
    }    
//   std::cout << "Connectivity: " << conn << std::endl << std::endl;
//   getchar();
   return conn; 
}


double deviationPearson(tPartition &Pe)
{   
    double dev = 0;
    std::map<int, std::vector<double> > *centroids;
    std::map<int, std::vector<double> >::iterator centroidsIt;
    centroids = Pe.getCentroids();
    for (tPartition::tClustersIt itClu = Pe.clusters.begin(); 
         itClu != Pe.clusters.end(); itClu++)
    {
        //std::cout << "Cluster: " << (*itClu).clusterLabel << std::endl;
        int nAttributes = Pe.patterns->nAttributes();
      
        // calculate the summed distances for each pattern of a cluster
        for (cCluster::tPatternsIt itPat = (*itClu).patterns.begin();
             itPat != (*itClu).patterns.end(); itPat++)
        {
            //std::vector<double> pat = Pe.patterns.patternValues(*itPat);
            //centroids[(*itClu).clusterLabel];
            
            centroidsIt = centroids->find((*itClu).clusterLabel);
            double pearson = pearsonCorrelation(Pe.patterns->patternValues(*itPat), (*centroidsIt).second);
            //double euclidean = euclideanDistance(Pe.patterns->patternValues(*itPat), centroids[(*itClu).clusterLabel].second);
            //std::cout << "Pattern: " << (*itPat) << " - distance: " << euclidean << std::endl;
            dev += pearson;
        }    
    }    

   //std::cout << "dev: " << dev << std::endl << std::endl;
   //getchar();
   return dev; 
   
//    double dev = 0;
//    std::map<int, std::vector<double> > centroid;
//    for (tPartition::tClustersIt itClu = Pe.clusters.begin(); 
//         itClu != Pe.clusters.end(); itClu++)
//    {
//        //std::cout << "Cluster: " << (*itClu).clusterLabel << std::endl;
//        int nAttributes = Pe.patterns->nAttributes();
//        // calculate the centroid of cluster itClu
//        centroid[(*itClu).clusterLabel].resize(nAttributes);
//        //std::cout << "centroide: ";
//        for (int a = 0; a < nAttributes; a++)
//        {
//            
//            //std::cout << "Cluster: " << (*itClu).clusterLabel << std::endl << "Patterns: ";
////            getchar();
//            //centroid[(*itClu).clusterLabel].insert(centroid[(*itClu).clusterLabel].end(), 0);
//            centroid[(*itClu).clusterLabel][a] = 0;
//            for (cluster::tPatternsIt itPat = (*itClu).patterns.begin();
//                 itPat != (*itClu).patterns.end(); itPat++)
//            {
//                centroid[(*itClu).clusterLabel][a] += Pe.patterns->patternValues(*itPat)[a];
//                //std::cout << (*itPat) << " " << centroid[(*itClu).clusterLabel][a] << std::endl;
//            }    
//            centroid[(*itClu).clusterLabel][a] /= (*itClu).nPatterns();
//            //std::cout << " " << centroid[(*itClu).clusterLabel][a];
//        }    
//        //std::cout << std::endl;
//        
//        //getchar();
//        // calculate the summed distances for each pattern of a cluster
//        for (cluster::tPatternsIt itPat = (*itClu).patterns.begin();
//             itPat != (*itClu).patterns.end(); itPat++)
//        {
//            //std::vector<double> pat = Pe.patterns.patternValues(*itPat);
//            double pearson = pearsonCorrelation(Pe.patterns->patternValues(*itPat), centroid[(*itClu).clusterLabel]);
//            //std::cout << "Pattern: " << (*itPat) << " - distance: " << euclidean << std::endl;
//            dev += pearson;
//        }    
//    }    
//   //std::cout << "dev: " << dev << std::endl << std::endl;
//   //getchar();
//   return dev; 
}

/*
// Silhouette
double silhouette(fs::path dataset, fs::path partitionFileName, t_similarities sim)
{       
    fs::path auxName;
    
    auxName = fs::basename(dataset) + "_" + fs::basename(partitionFileName) + "_silhouette.txt";
    fs::path dirSilhouette = partitionFileName.branch_path().string() + "_silhouette";
    if (!exists(dirSilhouette))
        fs::create_directory(dirSilhouette);
    fs::path silhoueteFileName = dirSilhouette / auxName;
    
    fs::path silhouetteSummaryFileName = dirSilhouette / (fs::basename(dataset) + "_" + fs::basename(partitionFileName) + "_silhouette_summary.txt");
    
    std::string command = "perl calculaSilhouette.pl " + strSimilarities(sim) + " " + dataset.string() + " " + 
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
*/

//// Silhouette
//double validationIndex::silhouette(fs::path dataset, fs::path partitionFileName, t_similarities sim)
//{       
//    fs::path auxName;
//    
//    auxName = fs::basename(dataset) + "_silhouette.txt";
//    fs::path silhoueteFileName = partitionFileName.branch_path() / auxName;
//    
//    fs::path silhouetteSummaryFileName = partitionFileName.branch_path() / (fs::basename(dataset) + "_silhouette_summary.txt");
//    
//    std::string command = "perl silhouetteVersao2.pl " + strSimilarities(sim) + " " + dataset.string() + " " + 
//              partitionFileName.string() + " " + silhoueteFileName.string();
//    system(command.c_str());
//    
//    fs::ifstream file;
//    file.open(silhouetteSummaryFileName);
//    if (!file.good()) 
//    {
//        std::cout << "\nError in the file " << silhouetteSummaryFileName.string() << std::endl;
//        getchar();
//    }
//    
//    int cluster;
//    double silhouette, averageSilhouette;
//    
//    file.ignore(maxFileLine, '\n');
//    while (!file.eof())
//    {
//        file >> cluster >> silhouette;
//        if (!file.fail())
//        {
//            if (cluster == 0) averageSilhouette = silhouette;
//            //std::cout << "c: " << cluster << "\ts: " << silhouette << std::endl;
//        }
//    }
//    file.close();
//
//    return averageSilhouette;
//}

// erro quadratico com distancia Euclideana - criterio do k-means
double squaredErrorEuclidean(tPartition &Pe)
{ 
    
    double dev = 0;
    std::map<int, std::vector<double> > *centroids;
    std::map<int, std::vector<double> >::iterator centroidsIt;
    centroids = Pe.getCentroids();
    for (tPartition::tClustersIt itClu = Pe.clusters.begin(); 
         itClu != Pe.clusters.end(); itClu++)
    {
        //std::cout << "Cluster: " << (*itClu).clusterLabel << std::endl;
        int nAttributes = Pe.patterns->nAttributes();
      
        // calculate the summed distances for each pattern of a cluster
        for (cCluster::tPatternsIt itPat = (*itClu).patterns.begin();
             itPat != (*itClu).patterns.end(); itPat++)
        {
            //std::vector<double> pat = Pe.patterns.patternValues(*itPat);
            //centroids[(*itClu).clusterLabel];
            
            centroidsIt = centroids->find((*itClu).clusterLabel);
            double euclidean = euclideanDistance(Pe.patterns->patternValues(*itPat), (*centroidsIt).second);
            //double euclidean = euclideanDistance(Pe.patterns->patternValues(*itPat), centroids[(*itClu).clusterLabel].second);
            //std::cout << "Pattern: " << (*itPat) << " - distance: " << euclidean << std::endl;
            dev += euclidean*euclidean;
        }    
    }    

   //std::cout << "dev: " << dev << std::endl << std::endl;
   //getchar();
   return dev; 

//    double dev = 0;
//    std::map<int, std::vector<double> > centroid;
//    for (tPartition::tClustersIt itClu = Pe.clusters.begin(); 
//         itClu != Pe.clusters.end(); itClu++)
//    {
//        //std::cout << "Cluster: " << (*itClu).clusterLabel << std::endl;
//        int nAttributes = Pe.patterns->nAttributes();
//        // calculate the centroid of cluster itClu
//        centroid[(*itClu).clusterLabel].resize(nAttributes);
//        //std::cout << "centroide: ";
//        for (int a = 0; a < nAttributes; a++)
//        {
//            
//            //std::cout << "Cluster: " << (*itClu).clusterLabel << std::endl << "Patterns: ";
////            getchar();
//            //centroid[(*itClu).clusterLabel].insert(centroid[(*itClu).clusterLabel].end(), 0);
//            centroid[(*itClu).clusterLabel][a] = 0;
//            for (cluster::tPatternsIt itPat = (*itClu).patterns.begin();
//                 itPat != (*itClu).patterns.end(); itPat++)
//            {
//                centroid[(*itClu).clusterLabel][a] += Pe.patterns->patternValues(*itPat)[a];
//                //std::cout << (*itPat) << " " << centroid[(*itClu).clusterLabel][a] << std::endl;
//            }    
//            centroid[(*itClu).clusterLabel][a] /= (*itClu).nPatterns();
//            //std::cout << " " << centroid[(*itClu).clusterLabel][a];
//        }    
//        //std::cout << std::endl;
//        
//        //getchar();
//        // calculate the summed distances for each pattern of a cluster
//        for (cluster::tPatternsIt itPat = (*itClu).patterns.begin();
//             itPat != (*itClu).patterns.end(); itPat++)
//        {
//            //std::vector<double> pat = Pe.patterns.patternValues(*itPat);
//            double euclidean = euclideanDistance(Pe.patterns->patternValues(*itPat), centroid[(*itClu).clusterLabel]);
//            //std::cout << "Pattern: " << (*itPat) << " - distance: " << euclidean << std::endl;
//            dev += euclidean;
//        }    
//    }    
//
//   //std::cout << "dev: " << dev << std::endl << std::endl;
//   //getchar();
//   return dev; 
}


// erro quadratico com correla��o de Pearson - criterio do k-means
double squaredErrorPearson(tPartition &Pe)
{ 
    
    double dev = 0;
    std::map<int, std::vector<double> > *centroids;
    std::map<int, std::vector<double> >::iterator centroidsIt;
    centroids = Pe.getCentroids();
    for (tPartition::tClustersIt itClu = Pe.clusters.begin(); 
         itClu != Pe.clusters.end(); itClu++)
    {
        //std::cout << "Cluster: " << (*itClu).clusterLabel << std::endl;
        int nAttributes = Pe.patterns->nAttributes();
      
        // calculate the summed distances for each pattern of a cluster
        for (cCluster::tPatternsIt itPat = (*itClu).patterns.begin();
             itPat != (*itClu).patterns.end(); itPat++)
        {
            //std::vector<double> pat = Pe.patterns.patternValues(*itPat);
            //centroids[(*itClu).clusterLabel];
            
            centroidsIt = centroids->find((*itClu).clusterLabel);
            double pearson = pearsonCorrelation(Pe.patterns->patternValues(*itPat), (*centroidsIt).second);
            //double euclidean = euclideanDistance(Pe.patterns->patternValues(*itPat), centroids[(*itClu).clusterLabel].second);
            //std::cout << "Pattern: " << (*itPat) << " - distance: " << euclidean << std::endl;
            dev += pearson*pearson;
        }    
    }    

   //std::cout << "dev: " << dev << std::endl << std::endl;
   //getchar();
   return dev; 
}

// erro quadratico com distancia coseno - criterio do k-means
double squaredErrorCoseno(tPartition &Pe)
{ 
    
    double dev = 0;
    std::map<int, std::vector<double> > *centroids;
    std::map<int, std::vector<double> >::iterator centroidsIt;
    centroids = Pe.getCentroids();
    for (tPartition::tClustersIt itClu = Pe.clusters.begin(); 
         itClu != Pe.clusters.end(); itClu++)
    {
        //std::cout << "Cluster: " << (*itClu).clusterLabel << std::endl;
        int nAttributes = Pe.patterns->nAttributes();
      
        // calculate the summed distances for each pattern of a cluster
        for (cCluster::tPatternsIt itPat = (*itClu).patterns.begin();
             itPat != (*itClu).patterns.end(); itPat++)
        {
            //std::vector<double> pat = Pe.patterns.patternValues(*itPat);
            //centroids[(*itClu).clusterLabel];
            
            centroidsIt = centroids->find((*itClu).clusterLabel);
            double coseno = cosenoDistance(Pe.patterns->patternValues(*itPat), (*centroidsIt).second);
            //double euclidean = euclideanDistance(Pe.patterns->patternValues(*itPat), centroids[(*itClu).clusterLabel].second);
            //std::cout << "Pattern: " << (*itPat) << " - distance: " << euclidean << std::endl;
            dev += coseno * coseno;
        }    
    }    

   //std::cout << "dev: " << dev << std::endl << std::endl;
   //getchar();
   return dev; 
}
