/*
 * cluster.cpp
 *
 *  Created on: 13/02/2013
 *      Author: katti
 */



#include "cluster.h" // class's header file


cCluster::cCluster()
{
    patterns.clear();
    clusterLabel = 0;
}

cCluster::cCluster(const cCluster &clu)
{
   patterns = clu.patterns;
   clusterLabel = clu.clusterLabel;
}

int cCluster::nPatterns()
{
   return patterns.size();
}

bool cCluster::insertPattern(int pat)
{
   tPatternsIt patternsIt = patterns.find(pat);
   if (patternsIt != patterns.end())
   {
      std::cout << "Pattern " << pat << " already belongs to cluster " << clusterLabel << std::endl;
      return false;
   }
   else
   {
      patterns.insert(pat);
      return true;
   }
}

bool cCluster::removePattern(int pat)
{
   tPatternsIt patternsIt = patterns.find(pat);
   if (patternsIt == patterns.end())
   {
      std::cout << "Pattern " << pat << " does not belongs to cluster " << clusterLabel << std::endl;
      return false;
   }
   else
   {
      patterns.erase(pat);
   }
   return true;
}

// randomly choose a pattern of the cluster
int cCluster::randomPattern()
{
   int max, pos, pat;
   max = patterns.size(); // valor maximo para deslocar o iterator e ainda estar apontando para
                          // um elemento dentro do conjunto
   pos = randomInt(0, max - 1);

   tPatternsIt patternsIt = patterns.begin();
   advance(patternsIt, pos);
//   std::cout << "random pattern " << *patternsIt << std::endl;
   return *patternsIt;
}

void cCluster::showCluster()
{
    std::cout << "Cluster " << clusterLabel << ": ";
    for (tPatternsIt patternsIt = patterns.begin(); patternsIt != patterns.end(); patternsIt++)
    {
        std::cout << *patternsIt << "  ";
    }

//    std::cout << "Cluster: " << clusterLabel << std::endl;
//    std::cout << "Patterns: ";
//    for (patternsIt = patterns.begin(); patternsIt != patterns.end(); patternsIt++)
//    {
//        std::cout << *patternsIt << "  ";
//    }
//    std::cout << std::endl;
}
