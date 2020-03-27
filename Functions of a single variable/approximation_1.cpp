/* Piecewise interpolation by cubic Bessel polynomials */

#include"header.hpp"

int approximation_1(double f(double), double f_der(double), double a, double b, int n, double *coef)
{
    for(int i = 0; i < n + 1; i++)
    {
        coef[i] = 1;
    }
    return 0;
}
double polynomial_value_1(double x, double a, double b, int N, double *coef) 
{
    return x;
}