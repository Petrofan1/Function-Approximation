#include <iostream>
#include <cmath> 
#include <cstring>
#include <iomanip>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <QtWidgets/QtWidgets>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QMainWindow>
#include <QPushButton>
#include <unistd.h>
#include <new>

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP
#include "triangle.hpp"
#endif

#define SETW_CONSTANT 11
#define NUMBER_OF_ITERATIONS 50
#define NUMBER_OF_POINTS_FOR_DRAWING 64

// calculations.cpp:
int parse_command_line(int argc, char* argv[]);
int read_area_parameters(char* filename, point *vertices, double *q);
int convexity(point *vertices);
int number_of_vertices(int n);
point diagonals_intersection(point *vertices);
void set_vertices_of_cutout(point *vertices, point *cutout_vertices, double q);
void fill_f_and_x(double (*f) (point), point *vertices, point *cutout_vertices, double *func, point *x, int n);
void fill_x(point *vertices, point *cutout_vertices, point *x, int n);
void fill_f_approx(point *vertices, point *cutout_vertices, double *approx, point *points, double *x, int n);
double find_residual(double (*f) (point), point *vertices, point *cutout_vertices, double *x, int n, point *points);
double value(point p_1, point p_2, point p_3, point p);
void min_max(double *func, int n, double *min, double *max);
double vector_norm(double *vector, int n);

// actions.cpp:
void print_system_msr(double *matrix_msr, int *vector_msr, double *right, int n);

//system.cpp:
void fill_matrix(double *matrix_msr, int *vector_msr, point *x, Triangle* T, int n);
void fill_right_side(double *right, double *f, Triangle* T, int n); 
double right_side_help_func(int A, int B, int C, int D, int E, int F, double *f);
void scalar_product(double *vector_1, double *vector_2, int n, double *scalar_product, int num, int p);
void multiplication(double *matrix_msr, int *vector_msr, double *vector, double *res, int num, int p);
void* solve_system(void* args);
void* system_processing(void *args);

//functions.cpp:
double f_0(point a);
double f_1(point a);
double f_2(point a);
double f_3(point a);
double f_4(point a);
double f_5(point a);
double f_6(point a);
double f_7(point a);
