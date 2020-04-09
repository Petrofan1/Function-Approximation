#include<iostream>
#include<cstdio>
#include<cmath>
#include<cstring>

int approximation_0(int N, double *x, double *func, double *func_der, double *coef); 
int approximation_1(int N, double *x, double *func, double *func_der, double *coef);
double point_value(int n, int i, double a, double b);
double polynomial_value_0(double x, double a, double b, int N, double *coef);
double polynomial_value_1(double x, double a, double b, int N, double *coef);
double f_0(double x);
double f_1(double x);
double f_2(double x);
double f_3(double x);
double f_4(double x);
double f_5(double x);
double f_6(double x);
double f_der_0(double x);
double f_der_1(double x);
double f_der_2(double x);
double f_der_3(double x);
double f_der_4(double x);
double f_der_5(double x);
double f_der_6(double x);