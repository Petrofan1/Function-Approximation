#include "header.hpp"
#include <cfloat>
using namespace std;

point diagonals_intersection(point *vertices)
{
    point temp;
    double a = vertices[0].x; 
    double b = vertices[0].y; 
    double c = vertices[1].x; 
    double d = vertices[1].y;
    double j = vertices[2].x;
    double f = vertices[2].y;
    double g = vertices[3].x; 
    double h = vertices[3].y; 
    temp.x = (a*(d - f)*g + a*c*(f - h) - d*g*j + b*(-c + g)*j + c*h*j)/(-(b - f)*(c - g) + (d - h)*(a - j));
    temp.y = (a*f*(d - h) + (b - f)*(d*g - c*h) + b*(-d + h)*j)/(-(b - f)*(c - g) + (d - h)*(a - j));
    return temp;
}

int parse_command_line(int argc, char* argv[])
{
    if(argc == 1)
    {
        cout<<"Usage: "<<argv[0]<<" filename.txt n_x n_y k eps p\n"<<endl;
        return 2;
    }
    if(argc < 7)
    {
        cout<<"\nToo few arguments\n"<<endl;
        cout<<"Usage: "<<argv[0]<<" filename.txt n_x n_y k eps p\n"<<endl;
        return -1;
    }
    if(fopen(argv[1], "r") == NULL)
    {
        cout<<"\nCannot open file\n"<<endl;
        return -1;
    }
    if(atoi(argv[2]) != atoi(argv[3]))
    {
        cout<<"Usage: "<<argv[0]<<" filename.txt n_x n_y k eps p"<<endl;
        cout<<"\nParametn_x and n_y must be the same\n"<<endl;
        return -1;
    }
    if(atoi(argv[4]) < 0 || atoi(argv[4]) > 7)
    {
        cout<<"Usage: "<<argv[0]<<" filename.txt n_x n_y k eps p"<<endl;
        cout<<"\nNumber of function k takes values from 0 to 7\n"<<endl;
        return -1;
    }
    if(atoi(argv[6]) < 1)
    {
        cout<<"Usage: "<<argv[0]<<" filename.txt n_x n_y k eps p"<<endl;
        cout<<"\nNumber of threads should be integer greater than 1\n"<<endl;
        return -1;
    }
    return 0;
}

int read_area_parameters(char *filename, point *vertices, double *q)
{
    char temp;
    FILE *f;
    f = fopen((filename == 0)? "area.txt": filename, "r");
    for(int i = 0; i < 4;)
    {
        if(fscanf(f, "%lf", &vertices[i].x) == 1 && fscanf(f, "%lf", &vertices[i].y) == 1)
        { 
            i++;
            continue;
        }
        temp = fgetc(f);
        if(temp == '#')
        {
            while(temp != '\n' && temp != EOF) 
            {
                temp = fgetc(f);
            }
            continue;
        }
        else
        {
            cout<<"\nIncorrect data in file "<<filename<<"\n"<<endl;
            return -1;
        } 
    }
    for(int i = 0; i < 1;)
    {
        if(fscanf(f, "%lf", q) == 1)
        {
            i++;
            continue;
        }
        temp = fgetc(f);
        if(temp == '#')
        {
            while(temp != '\n' && temp != EOF) temp = fgetc(f);
            continue;
        }
        else
        {
            cout<<"\nIncorrect data in file "<<filename<<"\n"<<endl;
            return -1;
        } 
    }
    fclose(f);
    return 0;
}


int convexity(point *vertices)
{
    double temp, turn;
    turn = (vertices[1].x - vertices[0].x)*(vertices[2].y - vertices[1].y) - (vertices[1].y - vertices[0].y)*(vertices[2].x - vertices[1].x);
    for(int i = 1; i < 4; i++)
    {
        temp = (vertices[(i + 1) % 4].x - vertices[i % 4].x)*(vertices[(i + 2) % 4].y - vertices[(i + 1) % 4].y) - (vertices[(i + 1) % 4].y - vertices[i % 4].y)*(vertices[(i + 2) % 4].x - vertices[(i + 1) % 4].x);
        if(turn*temp <= 0)
        {
            cout<<"\nQuadrangle is not convex\n"<<endl;
            return -1;
        }
    }
    return 0;
}

int number_of_vertices(int n)
{
    return (3*(n + 1) - 2)*(n + 1);
}

void set_vertices_of_cutout(point *vertices, point *cutout_vertices, double q)
{
    cutout_vertices[0].x = (vertices[0].x + vertices[3].x)/2 + q*(vertices[0].x - vertices[3].x)/2;
    cutout_vertices[0].y = (vertices[0].y + vertices[3].y)/2 + q*(vertices[0].y - vertices[3].y)/2;
    cutout_vertices[3].x = (vertices[0].x + vertices[3].x)/2 + q*(vertices[3].x - vertices[0].x)/2;
    cutout_vertices[3].y = (vertices[0].y + vertices[3].y)/2 + q*(vertices[3].y - vertices[0].y)/2;
    cutout_vertices[1].x = (vertices[0].x + vertices[3].x)/2 + q*(vertices[0].x - vertices[3].x)/2 + (vertices[1].x - vertices[0].x)*q;
    cutout_vertices[1].y = (vertices[0].y + vertices[3].y)/2 + q*(vertices[0].y - vertices[3].y)/2 + (vertices[1].y - vertices[0].y)*q;
    cutout_vertices[2].x = (vertices[0].x + vertices[3].x)/2 + q*(vertices[3].x - vertices[0].x)/2 + (vertices[2].x - vertices[3].x)*q;;
    cutout_vertices[2].y = (vertices[0].y + vertices[3].y)/2 + q*(vertices[3].y - vertices[0].y)/2 + (vertices[2].y - vertices[3].y)*q;
}

void fill_f_and_x(double (*f) (point), point *vertices, point *cutout_vertices, double *func, point *x, int n)
{
    point A, B, C, D;
    int counter = 0;
    for(int i = 0; i < 3; i++)
    {   
        A = vertices[i];
        B = vertices[i + 1];
        C = cutout_vertices[i];
        D = cutout_vertices[i + 1];
        for(int j = 0; j < n; j++)
        {
            for(int l = 0; l < n - j; l++)
            {
                x[counter].x = A.x + (B.x - A.x)*j/n + (C.x - A.x)*l/n;
                x[counter].y = A.y + (B.y - A.y)*j/n + (C.y - A.y)*l/n;
                func[counter] = f(x[counter]);
                counter++;
            }
            for(int l = 0; l < j + 1; l++)
            {
                x[counter].x = C.x + (B.x - C.x)*j/n + (D.x - B.x)*l/n;
                x[counter].y = C.y + (B.y - C.y)*j/n + (D.y - B.y)*l/n;
                func[counter] = f(x[counter]);
                counter++;
            }
        }
    }
    for(int i = 0; i < n + 1; i++)
    {
        x[counter].x = B.x + (D.x - B.x)*i/n;
        x[counter].y = B.y + (D.y - B.y)*i/n;
        func[counter] = f(x[counter]);
        counter++;
    }
}

void fill_x(point *vertices, point *cutout_vertices, point *x, int n)
{
    point A, B, C, D;
    int counter = 0;
    for(int i = 0; i < 3; i++)
    {   
        A = vertices[i];
        B = vertices[i + 1];
        C = cutout_vertices[i];
        D = cutout_vertices[i + 1];
        for(int j = 0; j < n; j++)
        {
            for(int l = 0; l < n - j; l++)
            {
                x[counter].x = A.x + (B.x - A.x)*j/n + (C.x - A.x)*l/n;
                x[counter].y = A.y + (B.y - A.y)*j/n + (C.y - A.y)*l/n;
                counter++;
            }
            for(int l = 0; l < j + 1; l++)
            {
                x[counter].x = C.x + (B.x - C.x)*j/n + (D.x - B.x)*l/n;
                x[counter].y = C.y + (B.y - C.y)*j/n + (D.y - B.y)*l/n;
                counter++;
            }
        }
    }
    for(int i = 0; i < n + 1; i++)
    {
        x[counter].x = B.x + (D.x - B.x)*i/n;
        x[counter].y = B.y + (D.y - B.y)*i/n;
        counter++;
    }
}

double value(point p_1, point p_2, point p_3, point p)
{
    double eps = 1e-15;
    double a = 0,b = 0,c = 0, d = 0;
    d = p_2.y*(p_3.x - p_1.x) + p_3.y*(p_1.x - p_2.x) + p_1.y*(p_2.x - p_3.x);

    if(fabs(p_1.y - p_2.y) < eps && fabs(p_1.x - p_2.x) > eps && fabs(p_1.y - p_3.y) > eps) 
    {
        a = 1/(p_1.x - p_2.x);
        b = ((p_3.x - p_2.x)*a)/(p_1.y - p_3.y);
        c = (-p_3.x*p_1.y + p_2.x*p_3.y)*a/(p_1.y - p_3.y);
        return a*p.x + b*p.y + c;
    } 
    if(fabs(d) > eps && fabs(p_1.y - p_2.y) > eps)
    {
        a = (p_3.y - p_2.y)/d;
        b = (p_2.x - p_3.x)/d;
        c = (p_3.x*p_2.y - p_2.x*p_3.y)/d;
        return a*p.x + b*p.y + c;
    } 
    else 
    {
        cout<<"Something goes wrong"<<endl;
        return 0;
    }
}

void fill_f_approx(point *vertices, point *cutout_vertices, double *approx, point *points, double *x, int n)
{
    int n_grid = NUMBER_OF_POINTS_FOR_DRAWING;
    int j_real, left, left_real, trapeze, trapeze_real, lt, lt_real, ltj, ltj_real, l_real;
    point p_1, p_2, p_3, p;
    point vec_x, vec_y;
    for(int i = 0; i < 3; i++)
    {
        trapeze = i*n_grid*(n_grid + 1);
        trapeze_real = i*n*(n + 1);

        vec_y = (vertices[i + 1] - vertices[i])/n;
        vec_x = (cutout_vertices[i] - vertices[i])/n;
        for(int l = 0; l < n_grid; l++)
        {
            l_real = (l*n/n_grid);
            left = l*(n_grid + 1);
            left_real = l_real*(n + 1);
            lt = left + trapeze;
            lt_real = trapeze_real + left_real;
            for(int j = 0; j < n_grid - l; j++)
            {
                j_real = j*n/n_grid;
                ltj_real = lt_real + j_real;
                ltj = lt + j;
                if((double)(j*n/n_grid) - j_real <= (double)(l*n/n_grid) - l_real)
                {
                    p_1 = vertices[i] + vec_y*((int)(l_real)) + vec_x*((int)j_real);
                    p_2 = vertices[i] + vec_y*((int)(l_real + 1)) + vec_x*((int)j_real);
                    p_3 = vertices[i] + vec_y*((int)l_real) + vec_x*((int)(j_real + 1));
                    p = points[ltj];
                    approx[ltj] = value(p_1, p_2, p_3, p)*x[ltj_real] + value(p_2, p_3, p_1, p)*x[ltj_real + n + 1] + value(p_3, p_1, p_2, p)*x[ltj_real + 1];
                } 
                else
                {
                    p_1 = vertices[i] + vec_y*(l_real + 1) + vec_x*(j_real + 1);
                    p_3 = vertices[i] + vec_y*(l_real + 1) + vec_x*j_real;
                    p_2 = vertices[i] + vec_y*l_real + vec_x*(j_real + 1);
                    p = points[ltj];
                    approx[ltj] = value(p_1, p_2, p_3, p)*x[ltj_real + n + 2] + value(p_2, p_3, p_1, p)*x[ltj_real + 1] + value(p_3, p_1, p_2, p)*x[ltj_real + n + 1];
                } 
            }
        }
        vec_x = (cutout_vertices[i + 1] - cutout_vertices[i])/n;
        vec_y = (vertices[i + 1] - cutout_vertices[i])/n;
        for(int l = 0; l < n_grid; l++)
        {
            l_real = l*n/n_grid;
            left = n_grid*(l + 1);
            left_real = n*(l_real + 1);
            lt = left + trapeze;
            lt_real = trapeze_real + left_real;

            for(int j = 0; j < (n_grid - l)*(n_grid + 1); j += n_grid + 1)
            {
                int j_local = j/(n_grid + 1);
                int j_local_real = j_local*n/n_grid;
                j_real = j_local_real*(n + 1);
                ltj_real = lt_real + j_real;
                ltj = lt + j;

                if((double)(j_local*n/n_grid) - j_local_real <= (double)(l*n/n_grid) - l_real)
                {
                    p_1 = cutout_vertices[i] + vec_y*(l_real) + vec_x*(j_local_real);
                    p_2 = cutout_vertices[i] + vec_y*(l_real + 1) + vec_x*(j_local_real);
                    p_3 = cutout_vertices[i] + vec_y*(l_real) + vec_x*(j_local_real + 1);
                    p = points[ltj];
                    approx[ltj] = value(p_1, p_2, p_3, p)*x[ltj_real] + value(p_2, p_3, p_1, p)*x[ltj_real + n] + value(p_3, p_1, p_2, p)*x[ltj_real + n + 1];
                } 
                else
                {
                    p_1 = cutout_vertices[i] + vec_y*(l_real + 1) + vec_x*(j_local_real + 1);
                    p_3 = cutout_vertices[i] + vec_y*(l_real + 1) + vec_x*(j_local_real);
                    p_2 = cutout_vertices[i] + vec_y*(l_real) + vec_x*(j_local_real + 1);
                    p = points[ltj];
                    approx[ltj] = value(p_1, p_2, p_3, p)*x[ltj_real + 2*n + 1] + value(p_2, p_3, p_1, p)*x[ltj_real + n + 1] + value(p_3, p_1, p_2, p)*x[ltj_real + n] ;
                } 
                
            }
            if(i == 2)
            {
                int j = (n_grid - l)*(n_grid + 1);
                ltj_real = lt_real + j_real;
                ltj = lt + j;
                p_1 = cutout_vertices[i] + vec_y*(l_real) + vec_x*(j_real/(n + 1));
                p_2 = cutout_vertices[i] + vec_y*(l_real + 1) + vec_x*(j_real/(n + 1));
                p_3 = cutout_vertices[i] + vec_y*(l_real) + vec_x*((j_real/(n + 1) + 1));
                p = points[ltj];
                approx[ltj] = value(p_1, p_2, p_3, p)*x[ltj_real] + value(p_2, p_3, p_1, p)*x[ltj_real + n] + value(p_3, p_1, p_2, p)*x[ltj_real + n + 1];
            }
        }
        if(i == 2)
        {
            trapeze = (i + 1)*n_grid*(n_grid + 1);
            trapeze_real = (i + 1)*n*(n + 1);
            approx[trapeze] = x[trapeze_real];            
        }
    }
}

void min_max(double *func, int n, double *min, double *max)
{
    *max = DBL_MIN;
    *min = DBL_MAX;
    int num_ver = number_of_vertices(n);
    for(int i = 0; i < num_ver; i++)
    {
        *max = (*max > func[i])? *max: func[i];
        *min = (*min < func[i])? *min: func[i]; 
    }
}

double vector_norm(double *vector, int n)
{
    double max = 0;
    for(int i = 0; i < n; i++)
    {
        max = (fabs(vector[i]) > max)? fabs(vector[i]): max;
    }
    return max;
}

double find_residual(double (*f) (point), point *vertices, point *cutout_vertices, double *x, int n, point *points)
{
    int n_grid = n/2;
    fill_x(vertices, cutout_vertices, points, n_grid);
    int j_real = 0, left = 0, left_real = 0, trapeze = 0, trapeze_real = 0, lt = 0, lt_real = 0, ltj = 0, ltj_real = 0, l_real = 0;
    double resid = 0, temp_res, func_val;
    point p_1, p_2, p_3, p;
    point vec_x, vec_y;
    for(int i = 0; i < 3; i++)
    {
        trapeze = i*n_grid*(n_grid + 1);
        trapeze_real = i*n*(n + 1);

        vec_y = (vertices[i + 1] - vertices[i])/n;
        vec_x = (cutout_vertices[i] - vertices[i])/n;
        for(int l = 0; l < n_grid; l++)
        {
            l_real = (l*n/n_grid);
            left = l*(n_grid + 1);
            left_real = l_real*(n + 1);
            lt = left + trapeze;
            lt_real = trapeze_real + left_real;
            for(int j = 0; j < n_grid - l; j++)
            {
                j_real = j*n/n_grid;
                ltj_real = lt_real + j_real;
                ltj = lt + j;
                if((double)(j*n/n_grid) - j_real <= (double)(l*n/n_grid) - l_real)
                {
                    p_1 = vertices[i] + vec_y*(l_real) + vec_x*j_real;
                    p_2 = vertices[i] + vec_y*(l_real + 1) + vec_x*j_real;
                    p_3 = vertices[i] + vec_y*(l_real) + vec_x*(j_real + 1);
                    p = points[ltj];
                    temp_res = value(p_1, p_2, p_3, p)*x[ltj_real] + value(p_2, p_3, p_1, p)*x[ltj_real + n + 1] + value(p_3, p_1, p_2, p)*x[ltj_real + 1];
                    func_val = fabs(f(p) - temp_res);
                    resid = (resid > func_val)? resid: func_val;
                } 
                else
                {
                    p_1 = vertices[i] + vec_y*(l_real + 1) + vec_x*(j_real + 1);
                    p_3 = vertices[i] + vec_y*(l_real + 1) + vec_x*j_real;
                    p_2 = vertices[i] + vec_y*l_real + vec_x*(j_real + 1);
                    p = points[ltj];
                    temp_res = value(p_1, p_2, p_3, p)*x[ltj_real + n + 2] + value(p_2, p_3, p_1, p)*x[ltj_real + 1] + value(p_3, p_1, p_2, p)*x[ltj_real + n + 1];
                    func_val = fabs(f(p) - temp_res);
                    resid = (resid > func_val)? resid: func_val;
                } 
            }
        }

        vec_x = (cutout_vertices[i + 1] - cutout_vertices[i])/n;
        vec_y = (vertices[i + 1] - cutout_vertices[i])/n;
        for(int l = 0; l < n_grid; l++)
        {
            l_real = l*n/n_grid;
            left = n_grid*(l + 1);
            left_real = n*(l_real + 1);
            lt = left + trapeze;
            lt_real = trapeze_real + left_real;

            for(int j = 0; j < (n_grid - l)*(n_grid + 1); j += n_grid + 1)
            {
                int j_local = j/(n_grid + 1);
                int j_local_real = j_local*n/n_grid;
                j_real = j_local_real*(n + 1);
                ltj_real = lt_real + j_real;
                ltj = lt + j;

                if((double)(j_local*n/n_grid) - j_local_real <= (double)(l*n/n_grid) - l_real)
                {
                    p_1 = cutout_vertices[i] + vec_y*(l_real) + vec_x*(j_local_real);
                    p_2 = cutout_vertices[i] + vec_y*(l_real + 1) + vec_x*(j_local_real);
                    p_3 = cutout_vertices[i] + vec_y*(l_real) + vec_x*(j_local_real + 1);
                    p = points[ltj];
                    temp_res = value(p_1, p_2, p_3, p)*x[ltj_real] + value(p_2, p_3, p_1, p)*x[ltj_real + n] + value(p_3, p_1, p_2, p)*x[ltj_real + n + 1];
                    func_val = fabs(f(p) - temp_res);
                    resid = (resid > func_val)? resid: func_val;
                } 
                else
                {
                    p_1 = cutout_vertices[i] + vec_y*(l_real + 1) + vec_x*(j_local_real + 1);
                    p_3 = cutout_vertices[i] + vec_y*(l_real + 1) + vec_x*(j_local_real);
                    p_2 = cutout_vertices[i] + vec_y*(l_real) + vec_x*(j_local_real + 1);
                    p = points[ltj];
                    temp_res = value(p_1, p_2, p_3, p)*x[ltj_real + 2*n + 1] + value(p_2, p_3, p_1, p)*x[ltj_real + n + 1] + value(p_3, p_1, p_2, p)*x[ltj_real + n] ;
                    func_val = fabs(f(p) - temp_res);
                    resid = (resid > func_val)? resid: func_val;
                } 
                
            }
            if(i == 2)
            {
                int j = (n_grid - l)*(n_grid + 1);
                ltj_real = lt_real + j_real;
                ltj = lt + j;
                p_1 = cutout_vertices[i] + vec_y*(l_real) + vec_x*(j_real/(n + 1));
                p_2 = cutout_vertices[i] + vec_y*(l_real + 1) + vec_x*(j_real/(n + 1));
                p_3 = cutout_vertices[i] + vec_y*(l_real) + vec_x*((j_real/(n + 1) + 1));
                p = points[ltj];
                temp_res = value(p_1, p_2, p_3, p)*x[ltj_real] + value(p_2, p_3, p_1, p)*x[ltj_real + n] + value(p_3, p_1, p_2, p)*x[ltj_real + n + 1];
                func_val = fabs(f(p) - temp_res);
                resid = (resid > func_val)? resid: func_val;
            }
            
        }
        if(i == 2)
        {
            trapeze = (i + 1)*n_grid*(n_grid + 1);
            trapeze_real = (i + 1)*n*(n + 1);
            temp_res = x[trapeze_real]; 
            func_val = fabs(f(points[trapeze]) - temp_res);         
            resid = (resid > func_val)? resid: func_val;       
        }
    }

    fill_x(vertices, cutout_vertices, points, n);
    for(int i = 0; i < 3; i++)
    {
        trapeze = i*n*(n + 1);
        for(int l = 0; i < n; i++)
        {
            left = l*(n + 1);
            lt = left + trapeze;
            ltj = lt;
            p_1 = points[ltj];
            p_2 = points[ltj + n + 1];
            p_3 =  points[ltj + 1];
            p = (p_1 + p_2 + p_3)/3;  
            temp_res = value(p_1, p_2, p_3, p)*x[ltj] + value(p_2, p_3, p_1, p)*x[ltj + n + 1] + value(p_3, p_1, p_2, p)*x[ltj + 1];
            func_val = fabs(f(p) - temp_res);
            resid = (resid > func_val)? resid: func_val;
            for(int j = 1; j < n - l; j++)
            {
                ltj = lt + j;

                p_1 = points[ltj];
                p_2 = points[ltj + n + 1];
                p_3 =  points[ltj + 1];
                p = (p_1 + p_2 + p_3)/3;  
                temp_res = value(p_1, p_2, p_3, p)*x[ltj] + value(p_2, p_3, p_1, p)*x[ltj + n + 1] + value(p_3, p_1, p_2, p)*x[ltj + 1];
                func_val = fabs(f(p) - temp_res);
                resid = (resid > func_val)? resid: func_val;

                p_1 = points[ltj + n + 1];
                p_2 = points[ltj];
                p_3 = points[ltj + n];
                p = (p_1 + p_2 + p_3)/3;
                temp_res = value(p_1, p_2, p_3, p)*x[ltj + n + 1] + value(p_2, p_3, p_1, p)*x[ltj] + value(p_3, p_1, p_2, p)*x[ltj + n];
                func_val = fabs(f(p) - temp_res);
                resid = (resid > func_val)? resid: func_val;
            }

            ltj = lt + n - l;
            p_1 = points[ltj];
            p_2 = points[ltj + n];
            p_3 =  points[ltj + n + 1];
            p = (p_1 + p_2 + p_3)/3;  
            temp_res = value(p_1, p_2, p_3, p)*x[ltj] + value(p_2, p_3, p_1, p)*x[ltj + n] + value(p_3, p_1, p_2, p)*x[ltj + n + 1];
            func_val = fabs(f(p) - temp_res);
            resid = (resid > func_val)? resid: func_val;

            for(int j = n - l + 1; j < n + 1; j++)
            {
                ltj = lt + j;

                p_1 = points[ltj];
                p_2 = points[ltj + n];
                p_3 =  points[ltj + n + 1];
                p = (p_1 + p_2 + p_3)/3;  
                temp_res = value(p_1, p_2, p_3, p)*x[ltj] + value(p_2, p_3, p_1, p)*x[ltj + n] + value(p_3, p_1, p_2, p)*x[ltj + n + 1];
                func_val = fabs(f(p) - temp_res);
                resid = (resid > func_val)? resid: func_val;

                p_1 = points[ltj + n];
                p_2 = points[ltj];
                p_3 =  points[ltj - 1];
                p = (p_1 + p_2 + p_3)/3;  
                temp_res = value(p_1, p_2, p_3, p)*x[ltj + n] + value(p_2, p_3, p_1, p)*x[ltj] + value(p_3, p_1, p_2, p)*x[ltj - 1];
                func_val = fabs(f(p) - temp_res);
                resid = (resid > func_val)? resid: func_val;
            }
        }
    }
    return resid;
}