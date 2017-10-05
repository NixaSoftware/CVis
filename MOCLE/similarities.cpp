#include "similarities.h"

double euclideanDistance(const std::vector<double> &pattern1, const std::vector<double> &pattern2)
{
    int i;
    double distance = 0.0;
    
//    std::cout << "Pattern 1 size: " << pattern1.size() << std::endl;
//    for (int i = 0; i < pattern1.size(); i++)
//    {
//        std::cout << pattern1[i] << " ";
//    }    
//    std::cout << std::endl;

//    std::cout << "Pattern 2 size: " << pattern2.size() << std::endl;
//    for (int i = 0; i < pattern2.size(); i++)
//    {
//        std::cout << pattern2[i] << " ";
//    }    
//    std::cout << std::endl;
        
    if (pattern1.size() != pattern2.size())
    {
        std::cout << "Patterns should have the same size." << std::endl;
        getchar();
        return -1;
    }
    
    for (i = 0; i < pattern1.size(); i++)
    {
        distance += (pattern1[i] - pattern2[i]) * (pattern1[i] - pattern2[i]);
    }
    distance = sqrt(distance);
    
    return distance;
}
       
double pearsonCorrelation(const std::vector<double> &pattern1, const std::vector<double> &pattern2)
{

    int i, d;
    double correlation = 0;
    double average1 = 0, average2 = 0, s1 = 0, s2 = 0, s3 = 0;

    d = pattern1.size();
    if (d != pattern2.size())
    {
        std::cout << "Patterns should have the same size." << std::endl;
        getchar();
        return -1;
    }
    
    // calculate the average of each pattern
    for (i = 0; i < d; i++)
    {
       average1 += pattern1[i];
       average2 += pattern2[i];
    }
    average1 /= d;
    average2 /= d;
    
//    std::cout << "average1: " << average1 << std::endl;
//    std::cout << "average2: " << average2 << std::endl;
    
    for (i = 0; i < d; i++)
    {
        s1 += (pattern1[i] - average1) * (pattern2[i] - average2);
        s2 += (pattern1[i] - average1) * (pattern1[i] - average1);
        s3 += (pattern2[i] - average2) * (pattern2[i] - average2);
    }
//    std::cout << "s1: " << s1  << "        s2: " << s2 << "        s3: " << s3 << std::endl;

    if (s2 * s3 == 0)
    {
        correlation = 0;
    } 
    else
    {
        correlation = (s1 / (sqrt(s2 * s3)));
    }
//    std::cout << "sqrt s2*s3: " << sqrt(s2 * s3) << std::endl;
//    std::cout << "pearson: " << correlation << std::endl;
    return correlation;
}

       
double pearsonDistance(const std::vector<double> &pattern1, const std::vector<double> &pattern2)
{
       

/* Como usado no SOTA e no cluster 3.0 (k-means):
The correlation routine calculates the weighted Pearson distance between two
rows or columns in a matrix. We define the Pearson distance as one minus the
Pearson correlation.
This definition yields a semi-metric: d(a,b) >= 0, and d(a,b) = 0 iff a = b.
but the triangular inequality d(a,b) + d(b,c) >= d(a,c) does not hold
(e.g., choose b = a + c).
*/

    int i, d;
    double correlation = 0;
    double average1 = 0, average2 = 0, s1 = 0, s2 = 0, s3 = 0;

    d = pattern1.size();
    if (d != pattern2.size())
    {
        std::cout << "Patterns should have the same size." << std::endl;
        getchar();
        return -1;
    }
    
    // calculate the average of each pattern
    for (i = 0; i < d; i++)
    {
       average1 += pattern1[i];
       average2 += pattern2[i];
    }
    average1 /= d;
    average2 /= d;
    
//    std::cout << "average1: " << average1 << std::endl;
//    std::cout << "average2: " << average2 << std::endl;
    
    for (i = 0; i < d; i++)
    {
        s1 += (pattern1[i] - average1) * (pattern2[i] - average2);
        s2 += (pattern1[i] - average1) * (pattern1[i] - average1);
        s3 += (pattern2[i] - average2) * (pattern2[i] - average2);
    }
//    std::cout << "s1: " << s1  << "        s2: " << s2 << "        s3: " << s3 << std::endl;

    if (s2 * s3 == 0)
    {
        correlation = 0;
    } 
    else
    {
        correlation = (s1 / (sqrt(s2 * s3)));
    }
//    std::cout << "sqrt s2*s3: " << sqrt(s2 * s3) << std::endl;
//    std::cout << "pearson: " << correlation << std::endl;
    return 1.0 - correlation;
}
       
double cosenoDistance(const std::vector<double> &pattern1, const std::vector<double> &pattern2)
{
    int i;
    double distance = 0, d1 = 0, d2 = 0, d3 = 0;
    
//    std::cout << "Pattern 1 size: " << pattern1.size() << std::endl;
//    for (int i = 0; i < pattern1.size(); i++)
//    {
//        std::cout << pattern1[i] << " ";
//    }    
//    std::cout << std::endl;

//    std::cout << "Pattern 2 size: " << pattern2.size() << std::endl;
//    for (int i = 0; i < pattern2.size(); i++)
//    {
//        std::cout << pattern2[i] << " ";
//    }    
//    std::cout << std::endl;
        
    if (pattern1.size() != pattern2.size())
    {
        std::cout << "Patterns should have the same size." << std::endl;
        getchar();
        return -1;
    }
    
    for (i = 0; i < pattern1.size(); i++)
    {
        d1 += pattern1[i] * pattern2[i];
        d2 += pattern1[i] * pattern1[i];
        d3 += pattern2[i] * pattern2[i];
    }
    distance = d1 / (sqrt(d2 * d3));
    
    return distance;
}       
