/* Piecewise interpolation by cubic Bessel polynomials */

#include"header.hpp"
double d(int i, int N, double *x, double *func)
{
    double x_temp, y_temp, z_temp, diff_z_x, diff_y_x, diff_z_y;
    if(i != 0 && i != N - 1)
    {
        x_temp = x[i - 1];
        y_temp = x[i];
        z_temp = x[i + 1];
        diff_y_x = y_temp - x_temp;
        diff_z_x = z_temp - x_temp;
        diff_z_y = z_temp - y_temp;
        return (diff_z_y*((func[i] - func[i - 1])/diff_y_x) + diff_y_x*((func[i + 1] - func[i])/diff_z_y))/diff_z_x;
    }
    else if(i == 0)
    {
        return d(1, N, x, func);
    }   
    else if(i == N - 1)
    {
       return d(N - 2, N, x, func);
    }
    return 0;
}
int approximation_1(int N, double *x, double *func, double*, double *coef)
{

    double f_diff, d_x, d_y, delta, x_temp, y_temp;
    for(int i = 0; i < N; i++)
    {
        x_temp = x[i];
        y_temp = x[i + 1];
        delta = y_temp - x_temp;
        d_x = d(i, N, x, func);
        d_y = d(i + 1, N, x, func);
        f_diff = (func[i + 1] - func[i])/delta;
        coef[i*4] = func[i];
        coef[i*4 + 1] = d_x;
        coef[i*4 + 2] = (3*f_diff - 2*d_x - d_y)/delta;
        coef[i*4 + 3] = (d_x + d_y - 2*f_diff)/(delta*delta);
    }
    return 0;
}
double polynomial_value_1(double x, double a, double b, int N, double *coef) 
{
    double diff, delta = (b - a)/(N - 1);
    int i = (x - a)/delta;
    diff = x - (a + i*delta);
    return coef[i*4] + diff*coef[i*4 + 1] + diff*diff*diff*coef[i*4 + 2] + diff*diff*diff*coef[i*4 + 3];
}