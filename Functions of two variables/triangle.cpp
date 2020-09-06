#include "header.hpp"

using namespace std;

double Triangle::jacobian(point a, point b, point c)
{
    return(fabs((c.x - a.x)*(b.y - a.y) - (b.x - a.x)*(c.y - a.y)));
}

Triangle::Triangle(point a, point b, point c, int n)
{
    point a_temp, b_temp, c_temp;
    A = a;
    B = b;
    C = c;
    this->n = n;
    a_temp.x = A.x;
    a_temp.y = A.y;
    b_temp.x = A.x + (B.x - A.x)/n;
    b_temp.y = A.y + (B.y - A.y)/n;
    c_temp.x = A.x + (C.x - A.x)/n;
    c_temp.y = A.y + (C.y - A.y)/n;

    Jacobian = jacobian(a_temp, b_temp, c_temp);
}

double Triangle::integral(double f_3, double f_2, double f_1, double g_3, double g_2, double g_1, point, point, point)
{
    double A_, B_, C_, D_, E_, F_;
    A_ = f_1 - f_3;
    B_ = f_2 - f_3;
    C_ = f_3;
    D_ = g_1 - g_3;
    E_ = g_2 - g_3;
    F_ = g_3;
    double temp = (A_*(2*D_ + E_ + 4*F_) + B_*(D_  + 2*E_ + 4*F_) + 4*C_*(D_ + E_ + 3*F_))/24;
    return Jacobian*temp;
}

double Triangle::get_jacobian()
{
    return Jacobian;
}
Triangle::~Triangle()
{

}