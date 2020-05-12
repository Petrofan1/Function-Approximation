#include "header.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    if(parse_command_line(argc, argv) == -1) return -1;

    int n, n_double, /*k,*/ num_ver, num_ver_double;
    int *vector_msr;
    double q;
    double *func, *matrix_msr, *right;
    point *vertices, *cutout_vertices, *x;
    double (*f) (point) = f_0;

    vertices = new point[4];
    cutout_vertices = new point[4];

    for(int i = 0; i < 4; i++)
    {
        vertices[i].x = atof(argv[2*i + 1]);
        vertices[i].y = atof(argv[2*i + 2]);
    }

    if(convexity(vertices) == -1)
    {
        delete [] vertices;
        delete [] cutout_vertices;
        return 0;
    }

    n = atoi(argv[9]);
    q = atof(argv[10]);
    // k = atoi(argv[11]);

    n_double = n*2;
    num_ver = number_of_vertices(n);
    num_ver_double = number_of_vertices(n_double);

    // choose_function(f, k);


    set_vertices_of_cutout(vertices, cutout_vertices, q);

    for(int i = 0; i < 4; i++)
    {
        cout<<cutout_vertices[i].x<<" "<<cutout_vertices[i].y<<endl;
    }

    x = new point[num_ver_double];
    func = new double[num_ver_double];
    fill_f_and_x(f, vertices, cutout_vertices, func, x, n);
    
    Triangle *T = static_cast<Triangle*>(operator new[] (6*sizeof(Triangle)));
    for(int i = 0; i < 3; i++)
    {
        new(T + 2*i) Triangle(vertices[i], vertices[i + 1], cutout_vertices[i], n);
        new(T + 2*i + 1) Triangle(cutout_vertices[i], vertices[i + 1], cutout_vertices[i + 1], n);
    }

    matrix_msr = new double[7*num_ver];
    vector_msr = new int[7*num_ver];
    right = new double[num_ver];
    memset(matrix_msr, 0, 7*num_ver*sizeof(double));
    memset(vector_msr, 0, 7*num_ver*sizeof(int));
    memset(right, 0, num_ver*sizeof(double));
    
    fill_matrix(matrix_msr, vector_msr, x, T, n);

    fill_f_and_x(f, vertices, cutout_vertices, func, x, n_double);
    fill_right_side(right, func, T, n);

    for(int i = 0; i < 7*num_ver; i++)
    {
        cout<<matrix_msr[i]<<" ";
    }
    cout<<"\n"<<endl;
    for(int i = 0; i < 7*num_ver; i++)
    {
        cout<<vector_msr[i]<<" ";
    }
    cout<<"\n"<<endl;
    print_system_msr(matrix_msr, vector_msr, right, n);

    delete[] x;
    delete[] func;
    delete[] right;
    delete T;
    // for(int i = 0; i < 6; i++)
    // {
    //     delete T[i];
    // }
    delete[] matrix_msr;
    delete[] vector_msr; 
    delete[] vertices;
    delete[] cutout_vertices;
    return 0;
}