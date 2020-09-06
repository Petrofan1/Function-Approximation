#include"header.hpp"

double f_0(point)
{
    return 1;
}

double f_1(point a)
{
    return a.x;
}

double f_2(point a)
{
    return a.y;
}

double f_3(point a)
{
    return a.x + a.y;
}

double f_4(point a)
{
    return sqrt(a.x*a.x + a.y*a.y);
}

double f_5(point a)
{
    return a.x*a.x + a.y*a.y;
}

double f_6(point a)
{
    return exp(a.x*a.x - a.y*a.y);
}

double f_7(point a)
{
    return 1/(25*(a.x*a.x + a.y*a.y) + 1);
}
