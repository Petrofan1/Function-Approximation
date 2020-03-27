/* Newton's interpolation formula with multiple nodes */

#include"header.hpp"

int approximation_0(double f(double), double f_der(double), double a, double b, int N, double *coef)
{
    double temp = 0;
    double delta = (b - a)/(N - 1);
    for(int i = 0; i < 2*N; i += 2)
    {
        temp = f(point_value(N, i/2, a, b));
        coef[i] = temp;
        coef[i + 1] = temp;
    }
    for(int i = 2*N - 2; i >= 0; i--)
    {
        if(i % 2 == 0)
        {
            coef[i + 1] = f_der(point_value(N, i/2, a, b));
        }
        else
        {
            coef[i + 1] = (coef[i + 1] - coef[i])/delta;
        }
    }
    for (int j = 1; j < N; j++)
    {
        for (int i = 2*N - 1; i >= 2*(j + 1) - 1; i -= 2)
        {
            coef[i] = (coef[i] - coef[i - 1]) / (j*delta);
            coef[i - 1] = (coef[i - 1] - coef[i - 2]) / (j*delta);
        }
        for (int i = 2*N - 1; i >= 2*(j + 1) + 1; i -= 2)
        {
            coef[i] = (coef[i] - coef[i - 1]) / (j*delta);
            coef[i - 1] = (coef[i - 1] - coef[i - 2]) / ((j + 1)*delta);
        }
        coef[2*(j + 1)- 1] = (coef[2*(j + 1) - 1] - coef[2*(j + 1)- 2]) / (j* delta);
    }
    for(int i = 0; i < 2*N; i++)
    {
        std::cout<<coef[i]<<std::endl;
    }
    return 0;
}
double polynomial_value_0(double x, double a, double b, int N, double *coef) 
{
    double sum = 0.0;
    double temp = 1.0;
    sum += coef[0];
    for(int i = 1; i < 2*N; i++)
    {
        temp *= (x - point_value(N, (i - 1)/2, a, b));
        sum += temp*coef[i];
    }
    return sum;
}
// for (step = 2; step <= n_; step++)
// {
//     for (i = 2 * n_ - 1; i >= 2 * step - 1; i -= 2)
//     {
//         polynom[i] = (polynom[i] - polynom[i - 1]) / ((step - 1) * delta_x);
//         polynom[i - 1] = (polynom[i - 1] - polynom[i - 2]) / ((step - 1) * delta_x);
//     }
//     for (i = 2 * n_ - 1; i >= 2 * step + 1; i -= 2)
//     {
//         polynom[i] = (polynom[i] - polynom[i - 1]) / ((step - 1) * delta_x);
//         polynom[i - 1] = (polynom[i - 1] - polynom[i - 2]) / (step * delta_x);
//     }
//     polynom[2 * step - 1] = (polynom[2 * step - 1] - polynom[2 * step - 2]) / ((step - 1) * delta_x);
// }
