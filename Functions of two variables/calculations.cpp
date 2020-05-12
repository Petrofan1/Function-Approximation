#include "header.hpp"
using namespace std;

int parse_command_line(int argc, char* argv[])
{
    if(argc < 11)
    {
        cout<<"\nToo few arguments\n"<<endl;
        cout<<"Usage: "<<argv[0]<<" A_x A_y B_x B_y C_x C_y D_x D_y n q\n"<<endl;
        return -1;
    }
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
        C = cutout_vertices[i + 1];
        D = cutout_vertices[i];
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
        x[counter].x = B.x + (C.x - B.x)*i/n;
        x[counter].y = B.y + (C.y - B.y)*i/n;
        func[counter] = f(x[counter]);
        counter++;
    }
}
