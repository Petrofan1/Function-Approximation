/* Piecewise interpolation by cubic Bessel polynomials */

#include"header.hpp"
double d(int i, int N, double a, double b, double f(double))
{
    double x, y, z, diff_z_x, diff_y_x, diff_z_y;
    if(i != 0 && i != N - 1)
    {
        x = point_value(N, i - 1, a, b);
        y = point_value(N, i, a, b);
        z = point_value(N, i + 1, a, b);
        diff_y_x = y - x;
        diff_z_x = z - x;
        diff_z_y = z - y;
        return (diff_z_y*((f(y) - f(x))/diff_y_x) + diff_y_x*((f(z) - f(y))/diff_z_y))/diff_z_x;
    }
    else if(i == 0)
    {
        x = a;
        y = a + (b - a)/(N - 1);
        return 0.5*(3*((f(y) - f(x))/(y - x)) - d(1, N, a, b, f));
    }
    else if(i == N - 1)
    {
        x = b - (b - a)/(N - 1);
        y = b;
        return 0.5*(3*((f(y) - f(x))/(y - x)) - d(N - 2, N, a, b, f));
    }
    return 0;
}
int approximation_1(double f(double), double (double), double a, double b, int N, double *coef)
{
    double f_diff, d_x, d_y, delta, x, y;
    for(int i = 0; i < N - 1; i++)
    {
        x = point_value(N, i, a, b);
        y = point_value(N, i + 1, a, b);
        delta = (b - a)/(N - 1);
        d_x = d(i, N, a, b, f);
        d_y = d(i + 1, N, a, b, f);
        f_diff = (f(y) - f(x))/delta;
        coef[i*4] = f(x);
        coef[i*4 + 1] = d_x;
        coef[i*4 + 2] = (3*f_diff - 2*d_x - d_y)/delta;
        coef[i*4 + 3] = (d_x + d_y - 2*f_diff)/(delta*delta);
    }
    return 0;
}
double polynomial_value_1(double x, double a, double b, int N, double *coef) 
{
    int i = 0;
    double diff;
    double delta = (b - a)/(N - 1);
    while(a + i*delta <= x) i++;
    i--;
    diff = x - (a + i*delta);
    if(fabs(b - x) < 1e-10)
    {
        i = N - 2;
    }
    return coef[i*4] + diff*coef[i*4 + 1] + diff*diff*diff*coef[i*4 + 2] + diff*diff*diff*coef[i*4 + 3];
}