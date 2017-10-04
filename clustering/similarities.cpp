#include "similarities.h"

double euclideanDistance(std::vector<double> pattern1, std::vector<double> pattern2)
{
    int i;
    double distance = 0;
    
    if (pattern1.size() != pattern2.size())
    {
        std::cout << "Patterns should have the same size." << std::endl;
        return -1;
    }
    
    for (i = 0; i < pattern1.size(); i++)
    {
        distance += (pattern1[i] - pattern2[i]) * (pattern1[i] - pattern2[i]);
    }
    distance = sqrt(distance);
    
    return distance;
}
       
double pearsonDistance(std::vector<double> pattern1, std::vector<double> pattern2)
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
    
    for (i = 0; i < d; i++)
    {
        s1 += (pattern1[i] - average1) * (pattern2[i] - average2);
        s2 += (pattern1[i] - average1) * (pattern1[i] - average1);
        s3 += (pattern2[i] - average2) * (pattern2[i] - average2);
    }

    if (s2 * s3 == 0)
    {
        correlation = 0;
    } 
    else
    {
        correlation = (s1 / (sqrt(s2 * s3)));
    }

    if (correlation < 0)
    	return 1 + correlation;
    return 1 - correlation;
}
       
