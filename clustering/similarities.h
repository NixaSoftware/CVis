#ifndef similarities_H
#define similarities_H

#include <iostream>
#include <vector>
#include <math.h>
#include "boost/lexical_cast.hpp"

double euclideanDistance(std::vector<double> pattern1, std::vector<double> pattern2);
double pearsonDistance(std::vector<double> pattern1, std::vector<double> pattern2);

#endif // similarities_H

