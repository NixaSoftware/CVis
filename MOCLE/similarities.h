#ifndef similarities_H
#define similarities_H

#include <iostream>
#include <vector>
#include <math.h>
#include <boost/lexical_cast.hpp>

double euclideanDistance(const std::vector<double> &pattern1, const std::vector<double> &pattern2);
double pearsonCorrelation(const std::vector<double> &pattern1, const std::vector<double> &pattern2);
double pearsonDistance(const std::vector<double> &pattern1, const std::vector<double> &pattern2); // = 1 - pearsonCorrelation
double cosenoDistance(const std::vector<double> &pattern1, const std::vector<double> &pattern2); // = 1 - pearsonCorrelation

#endif // similarities_H

