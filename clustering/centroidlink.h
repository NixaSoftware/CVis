// Class automatically generated by Dev-C++ New Class wizard

#ifndef centroidlink_H
#define centroidlink_H

#include <string>
#include "cpartition.h"
#include "clusteringTypes.h"
#include <iostream>
#include <vector>

// para incluir as bibliotecas Boost
#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include "boost/filesystem/fstream.hpp"    // ditto
#include "boost/filesystem/convenience.hpp"    // ditto
#include <iostream>                        // for std::cout
#include "boost/tokenizer.hpp"
#include "boost/lexical_cast.hpp"

//struct cmpInt
//{
//  bool operator()(int s1, int s2) const
//  {
//    return s1 < s2;
//    //return strcmp(s1, s2) < 0;
//  }
//};

// Run k-means clustering with the software cluster-1.27
class centroidlink
{
	public:
        //typedef std::map<int, std::vector<double> > t_centroids;   
        // class constructor
		centroidlink(std::string datasetName, std::string fileResultName,
                   t_similarities sim, int k);
		centroidlink();
		centroidlink(std::string datasetName, std::string fileResultName, t_similarities sim, std::string parametes);
		centroidlink(std::string parametes);
		// class destructor
		//~centroidlink();
		centroidlink(const centroidlink &a);
		centroidlink & operator = (const centroidlink &a);

		cpartition run();
        void changeDataset(std::string datasetName);
		void changeResultFile(std::string fileResultName);
		void changeSimilarity(t_similarities sim);
		void changeK(int value);
		cpartition getPartition();
		//t_centroids getCentroids();


	private:
        fs::path dataset;
        std::string fileResult;
        t_similarities similarity; 
                 // similarity metric to be employed
                 // 0: No gene clustering
                 // 1: Uncentered correlation
                 // 2: Pearson correlation
                 // 3: Uncentered correlation, absolute value
                 // 4: Pearson correlation, absolute value
                 // 5: Spearman's rank correlation
                 // 6: Kendall's tau
                 // 7: Euclidean distance
                 // 8: Harmonically summed Euclidean distance
                 // 9: City-block distance
                 // (default: 1)
        int k; 
        cpartition p;
        //t_centroids centroids;
        
        static const int kDefault = 2;
};

#endif // centroidlink_H

