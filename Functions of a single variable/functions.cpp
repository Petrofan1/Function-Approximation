#include <QPainter>

#include"header.hpp"

double polynomial_value(double x, double *coef, int n) 
{
    double value = 0.0;
    for(int i = 0; i <= n; i++)
    {
        value += coef[i]*pow(x, i);
    }
    return value;
}