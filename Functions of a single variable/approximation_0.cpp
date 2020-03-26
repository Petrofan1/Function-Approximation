/* Newton's interpolation formula with multiple nodes */

#include"header.hpp"

int approximation_0(double a, double b, int n, double *coef)
{
    for(int i = 0; i < n + 1; i++)
    {
        coef[i] = 1;
    }
    return 0;
}