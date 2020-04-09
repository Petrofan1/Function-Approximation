#include"header.hpp"

// k = 0:
double f_0(double x)
{
    return 1;
}
double f_der_0(double)
{
    return 0;
}

// k = 1:
double f_1(double x)
{
    return x;
}
double f_der_1(double x)
{
    return 1;
}

// k = 2:
double f_2(double x)
{
    return x*x;
}
double f_der_2(double x)
{
    return 2*x;
}

// k = 3:
double f_3(double x)
{
    return x*x*x;
}
double f_der_3(double x)
{
    return 3*x*x;
}

// k = 4:
double f_4(double x)
{
    return x*x*x*x;
}
double f_der_4(double x)
{
    return 4*x*x*x;
}

// k = 5:
double f_5(double x)
{
    return exp(x);
}
double f_der_5(double x)
{
    return exp(x);
}

// k = 6:
double f_6(double x)
{
    return 1/(25*x*x + 1);
}
double f_der_6(double x)
{
    return ((-1)*50*x)/(pow(25*x*x + 1, 2));
}