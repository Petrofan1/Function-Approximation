/* Piecewise interpolation by cubic Bessel polynomials */

#include"header.hpp"

double d(int i, int N, double *x, double *func)
{
    double h;
    h = (x[N - 1] - x[0])/(N - 1);   
    if(i != 0 && i != N - 1)
    {
        return(func[i + 1] - func[i - 1])/(2*h);
    }
    else if(i == 0)
    {
        return (N > 3)? d(1, N, x, func) - (d(2, N, x, func) - d(1, N, x, func)): d(1, N, x, func);
    }   
    else if(i == N - 1)
    {
       return (N > 3)? d(N - 2, N, x, func) - (d(N - 3, N, x, func) - d(N - 2, N, x, func)): d(N - 2, N, x, func);
    }
    return 0;
}
int approximation_1(int N, double *x, double *func, double*, double *coef)
{

    double f_diff, d_x, d_y, delta, x_temp, y_temp;
    for(int i = 0; i < N - 1; i++)
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
        // std::cout<<"coef[i] = "<<std::scientific<<coef[4*i]<<" coef[i + 1] = "<<coef[4*i + 1]<<" coef[i + 2] = "<<coef[4*i + 2]<<" coef[i + 3] = "<<coef[4*i + 3]<<std::endl;
    }
    return 0;
}
double polynomial_value_1(double x, double a, double b, int N, double *coef) 
{
    double diff, delta = (b - a)/(N - 1);
    int i = (x - a)/delta;
    if(i >= N - 1) i = N - 2;
    // if(i >= N - 2) i = N - 3;
    // if(i == 0) i = 1;
    diff = x - (a + i*delta);
    return coef[i*4] + diff*coef[i*4 + 1] + diff*diff*coef[i*4 + 2] + diff*diff*diff*coef[i*4 + 3];
}