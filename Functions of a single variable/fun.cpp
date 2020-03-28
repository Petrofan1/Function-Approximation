#include"header.hpp"

double f_0(double x)
{
    return x;
}
double f_der_0(double)
{
    return 1;
}

double f_1(double x)
{
    return x*x;
}
double f_der_1(double x)
{
    return 2*x;
}

double f_2(double x)
{
    return x*x*x;
}
double f_der_2(double x)
{
    return 3*x*x;
}

double f_3(double x)
{
    return exp(x);
}
double f_der_3(double x)
{
    return exp(x);
}

double f_4(double x)
{
    return cos(x);
}
double f_der_4(double x)
{
    return -sin(x);
}