#ifndef HEADER_HPP
#define HEADER_HPP
#include "header.hpp"
#endif
#include "widget.h"
using namespace std;

void Widget::choose_function()
{
    switch(k)
    {
        case 0:
            f = f_0;
            func_name = "k = 0: f(x, y) = 1";
            break;
        case 1:
            f = f_1;
            func_name = "k = 1: f(x, y) = x";
            break;
        case 2:
            f = f_2;
            func_name = "k = 2: f(x, y) = y";
            break;
        case 3:
            f = f_3;
            func_name = "k = 3: f(x, y) = x + y";
            break;
        case 4:
            f = f_4;
            func_name = "k = 4: f(x, y) = √(x² + y²)";
            break;
        case 5: 
            f = f_5;
            func_name = "k = 5: f(x, y) = x² + y²";
            break;
        case 6:
            f = f_6;
            func_name = "k = 6: f(x, y) = exp(x² - y²)";
            break;
        case 7:
            f = f_7;
            func_name = "k = 7: f(x, y) = 1/(25(x² + y²) + 1)";
            break;
        default:
            f = f_0;
            func_name = "k = 0: f(x) = 1";
            break;
    }
}

void Widget::check_result()
{
    if (is_ready == 1)
    {
        timer.stop();
        residual = find_residual(f, vertices, cutout_vertices, x, n, points);
        fill_f_and_x(f, vertices, cutout_vertices, func_draw, points_draw, n_grid);
        min_max(func_draw, n_grid, &min_draw, &max_draw);
        fill_f_approx(vertices, cutout_vertices, approx, points_draw, x, n);
        min_max(approx, n_grid, &min_approx, &max_approx);
        choose_point_of_view();
        paintGL();
    }
}

void Widget::change_function()
{
    if(is_ready == 0) return;
    k = (k + 1) % 8;
    choose_function();
    delete[] points;
    delete[] func;
    delete[] x;
    calculate_solution();
    timer.start(100);
}

void Widget::calculate_solution()
{
    is_ready = 0;
    int n_double = n*2;
    int num_ver = number_of_vertices(n);
    int num_ver_double = number_of_vertices(n_double);
    int n_draw = number_of_vertices(NUMBER_OF_POINTS_FOR_DRAWING);
    
    points = new point[(num_ver_double > n_draw)? num_ver_double: n_draw];
    func = new double[(num_ver_double > n_draw)? num_ver_double: n_draw];
    x = new double[num_ver];
    
    memset(x, 0, num_ver*sizeof(double));

    thr* th;
    th = new thr[1];

    th[0].f = f;
    th[0].x = x;
    th[0].vertices = vertices;
    th[0].cutout_vertices = cutout_vertices;
    th[0].points = points;
    th[0].func = func;
    th[0].n = n;
    th[0].p = p;
    th[0].p_error = p_error;
    th[0].max = &max;
    th[0].min = &min;
    th[0].eps = eps;
    th[0].threads = threads;
    th[0].barrier = &barrier;
    th[0].is_ready = &is_ready;
    th[0].th = th;

    if(pthread_create(&(threads[1]), NULL, system_processing, th + 0) != 0)
    {
        cout<<"Cannot create thread"<<endl;
        delete[] points;
        delete[] func;
        delete[] approx;
        delete[] x;
        throw -1;
    }
}

void Widget::increase_n()
{
    if(is_ready == 0) return;
    n = n*2;
    delete[] points;
    delete[] func;
    delete[] x;
    calculate_solution();
    timer.start(100);

}

void Widget::increase_p()
{
    if(is_ready == 0) return;
    p_error++;
    delete[] points;
    delete[] func;
    delete[] x;
    calculate_solution();
    timer.start(100);


}
void Widget::reduce_p()
{
    if(is_ready == 0) return;
    p_error--;
    delete[] points;
    delete[] func;
    delete[] x;
    calculate_solution();
    timer.start(100);
}

void Widget::reduce_n()
{
    if(is_ready == 0) return;
    n = (n/2 < 2)? 2: n/2;
    delete[] points;
    delete[] func;
    delete[] x;
    calculate_solution();
    timer.start(100);
}

void print_system_msr(double *matrix_msr, int *vector_msr, double *right, int n)
{
    cout<<"(Scalar products matrix | Right side of system):\n"<<endl;
    int num_ver = number_of_vertices(n);
    int j, num_elem, search, counter = num_ver + 1;
    for(int i = 0; i < num_ver; i++)
    {
        if(i == 0) cout<<"/";
        else if(i == num_ver - 1) cout<<"\\";
        else cout<<"|";
        
        num_elem = vector_msr[i + 1] - vector_msr[i];
        j = 0;
        search = vector_msr[counter];
        while(num_elem != 0)
        {
            while(j != search && j != i)
            {
                cout<<setw(SETW_CONSTANT)<<" - ";
                if(j > num_ver) exit(0);
                j++;
            }
            if(j == search)
            {
                cout<<setw(SETW_CONSTANT)<<matrix_msr[counter];
                counter++;
                search = vector_msr[counter];
                num_elem--;
                j++;
                continue;
            }
            else
            {
                cout<<setw(SETW_CONSTANT)<<matrix_msr[i];
                j++;
                continue;
            }
        }
        for(int k = j; k < num_ver; k++)
        {
            if(k == i) cout<<setw(SETW_CONSTANT)<<matrix_msr[i]; 
            else cout<<setw(SETW_CONSTANT)<<" - ";
        }
        cout<<setw(SETW_CONSTANT)<<" | ";
        cout<<setw(SETW_CONSTANT)<<right[i];
        if(i == 0) cout<<"\\";
        else if(i == num_ver - 1) cout<<"/";
        else cout<<"|";
        cout<<endl;
    }
}