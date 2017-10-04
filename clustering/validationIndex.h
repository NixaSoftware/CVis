#ifndef validationIndex_H
#define validationIndex_H

#include <iostream>
#include <vector>

// para incluir as bibliotecas Boost
#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include "boost/filesystem/fstream.hpp"    // ditto
#include "boost/filesystem/convenience.hpp"    // ditto
#include <iostream>                        // for std::cout
#include "boost/tokenizer.hpp"
#include "boost/lexical_cast.hpp"


#include <iterator>
#include "cpartition.h"
#include "clusteringTypes.h"

namespace fs = boost::filesystem;

class validationIndex 
{
public: 
    //validationIndex();
    //~validationIndex();
    double calculate(t_indices index, cpartition Pe, cpartition Pr);
    double silhouette(fs::path dataset, fs::path partitionFileName, t_similarities sim);
private:
    double correctedRand(cpartition Pe, cpartition Pr);
};

#endif // validationIndex_H
