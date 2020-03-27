#include <QPainter>

#include"header.hpp"
double point_value(int n, int i, double a, double b)
{
    return a + i*(b - a)/(n - 1);
}