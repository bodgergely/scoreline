#include "random.h"
#include <iostream>

using namespace std;

const double max_random = (double)RAND_MAX;

double random_uniform()
{
   unsigned r = rand();
   return (double)r / max_random;
}


double random_exponential(double Lambda)
{
    return (-1.0 * log(random_uniform()) ) / Lambda;
}


