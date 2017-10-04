// Class automatically generated by Dev-C++ New Class wizard

#ifndef DBSCAN_H
#define DBSCAN_H

#include <string>
#include <vector>
#include "cpartition.h"
#include "clusteringTypes.h"
#include "boost/tokenizer.hpp"
#include "boost/lexical_cast.hpp"

#include <iostream>

// object function that run the SOTA algorithm
class dbscan
{
	public:
        //typedef std::map<int, std::vector<double> > t_cells;
        // class constructor
		dbscan(std::string datasetName, std::string fileResultName, t_similarities sim,
           double aux_eps, int aux_minpts);
		dbscan(std::string datasetName, std::string fileResultName, t_similarities sim);
        dbscan();
		dbscan(std::string datasetName, std::string fileResultName, t_similarities sim, std::string parameters);
        dbscan(std::string parameters);
		// class destructor
		//~dbscan();
		dbscan(const dbscan &a);
		dbscan & operator = (const dbscan &a);

		cpartition run();
		void changeDataset(std::string datasetName);
		void changeResultFile(std::string fileResultName);
		void changeSimilarity(t_similarities sim);
        void changeEps(double aux_eps);      
        void changeMinpts(int aux_minpts); 
        
		cpartition getPartition();
		//t_cells getCells();

	private:
        std::string dataset;
        std::string fileResult;
        t_similarities similarity; // "euclidean", "correlation", "square"
                                // "offset", "spearman", "jackknife"

        double eps;
        int minpts;

        cpartition p;
              
        static const double epsDefault = 0;
        static const int minptsDefault = 0;
};

#endif // DBSCAN_H

