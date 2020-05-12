#include <iostream>
#include <cmath> 
#include <cstring>
#include <iomanip>
#include "triangle.hpp"
#define PRINT_CONSTANT 10
#define SETW_CONSTANT 13

int parse_command_line(int argc, char* argv[]);
int convexity(point *vertices);
int number_of_vertices(int n);
void set_vertices_of_cutout(point *vertices, point *cutout_vertices, double q);
void fill_f_and_x(double (*f) (point), point *vertices, point *cutout_vertices, double *func, point *x, int n);
void fill_matrix(double *matrix_msr, int *vector_msr, point *x, Triangle* T, int n);
void fill_right_side(double *right, double *f, Triangle* T, int n); 
double right_side_help_func(int A, int B, int C, int D, int E, int F, double *f);

double f_0(point a);

void choose_function(double (*f) (point), int k);
void print_system_msr(double *matrix_msr, int *vector_msr, double *right, int n);

