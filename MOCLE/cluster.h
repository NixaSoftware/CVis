/*
 * cluster.h
 *
 *  Created on: 13/02/2013
 *      Author: katti
 */

#ifndef CCLUSTER_H_
#define CCLUSTER_H_

#include "tPatterns.h"
#include "clusteringTypes.h"

#include <iostream>
#include <stdlib.h>
#include <set>
#include <map>
#include <vector>
#include <iterator>
#include <functional>


class cCluster {
public:
   std::set<int> patterns;
   typedef std::set<int>::iterator tPatternsIt;
   //tPatternsIt patternsIt;
   int clusterLabel;

   cCluster();
   cCluster(const cCluster &clu);

   int nPatterns();
   bool insertPattern(int pat);
   bool removePattern(int pat);
   int randomPattern();
   void showCluster();
   // calculate the centroid as the average of the points in the cluster
   //std::vector<double> centroid();
};
#endif /* CCLUSTER_H_ */
