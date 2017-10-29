#include "random.h"
#include <iostream>

using namespace std;

std::random_device rd;
std::mt19937 e2(rd());
std::uniform_real_distribution<> dist(0, 1);

double random_uniform()
{
   return dist(e2);
}


double random_exponential(double Lambda)
{
    return (-1.0 * log(random_uniform()) ) / Lambda;
}


