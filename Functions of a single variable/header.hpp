#include<stdio.h>
#include<cmath>
#include<cstring>

double f(double x);
double polynomial_value(double x, double *coef, int n);
int approximation_0(double a, double b, int n, double *coef); 
int approximation_1(double a, double b, int n, double *coef);