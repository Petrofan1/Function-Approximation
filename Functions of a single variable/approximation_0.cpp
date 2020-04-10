/* Newton's interpolation formula with multiple nodes */

#include"header.hpp"

int approximation_0(int N, double *x, double *func, double *func_der, double *coef)
{
    double temp = 0;
    double delta = x[1] - x[0];
    for(int i = 0; i < 2*N; i += 2)
    {
        temp = func[i/2];
        coef[i] = temp;
        coef[i + 1] = temp;
    }
    for(int i = 2*N - 2; i >= 0; i--)
    {
        if(i % 2 == 0)
        {
            coef[i + 1] = func_der[i/2];
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
    return 0;
}
double polynomial_value_0(double x, double a, double b, int N, double *coef) 
{
    double sum = 0.0;
    double temp = 1.0;
    sum += coef[0];
    for(int i = 1; i < 2*N; i++)
    {
        temp *= (x - (a + ((i - 1)/2)*(b - a)/(N - 1)));
        sum += temp*coef[i];
    }
    return sum;
}
