#include "widget.h" 
using namespace std;

#define PI 3.14159265

Widget::Widget(QWidget *parent, char* filename, int k, int n, double eps, int p) : QGLWidget(parent)
{

    this->n = n; 
    this->k = k;
    this->eps = eps;
    this->p = p;
    
    choose_function();

    vertices = new point[4];
    cutout_vertices = new point[4];

    connect(&timer, SIGNAL(timeout()), SLOT(check_result()));

    double q;
    if(read_area_parameters(filename, vertices, &q) == -1)
    {
        delete [] vertices;
        delete [] cutout_vertices;
        throw -1;
    }

    if(convexity(vertices) == -1)
    {
        delete [] vertices;
        delete [] cutout_vertices;
        throw -1;
    }

    set_vertices_of_cutout(vertices, cutout_vertices, q);

    threads = new pthread_t[p + 1];
    if(!threads)
    {
        cout<<"Cannot create array of threads!"<<endl;
        delete [] vertices;
        delete [] cutout_vertices;
    }
    pthread_barrier_init(&barrier, 0, p);

    approx = new double[number_of_vertices(n_grid)];
    func_draw = new double[number_of_vertices(n_grid)];
    points_draw = new point[number_of_vertices(n_grid)];
    
    center = diagonals_intersection(vertices);

    pov_x = center.x;
    pov_y = center.y;
    pov_z = 0;
    rot.x = -1;
    rot.y = -1;
    m_qObj = gluNewQuadric();
    
    calculate_solution();

    while(is_ready == 0)
    {
        cout<<"";
    }

    residual = find_residual(f, vertices, cutout_vertices, x, n, points);
    fill_f_and_x(f, vertices, cutout_vertices, func_draw, points_draw, n_grid);
    min_max(func_draw, n_grid, &min_draw, &max_draw);
    fill_f_approx(vertices, cutout_vertices, approx, points_draw, x, n);
    min_max(approx, n_grid, &min_approx, &max_approx);
    choose_point_of_view();


}

void Widget::initializeGL()
{
    glClearColor(255,255,255,255);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
    
void Widget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
     if(proj == 0) glOrtho(-1.0, 1.0, -1.0, 1.0, -10, 10);
    if(proj == 1) gluPerspective(45, 1, 0.1, 30);
}

void Widget::choose_point_of_view()
{
    switch(pov)
    {
        case 0:
            pov_x = center.x;
            pov_y = center.y;
            pov_z = 0;
            break;
        case 1:
            pov_x = 0;
            pov_y = 0;
            pov_z = 0;
            break;
        default:
            pov_x = center.x;
            pov_y = center.y;
            pov_z = 0;
            break;
    }
}

void Widget::change_point_of_view()
{
    pov = (pov + 1) % 2;
    choose_point_of_view();
    paintGL();
}

void Widget::draw_residual()
{
    double f_1, f_2, f_3, f_4;
    double color_r = 255, color_g = 255, color_b = 50;
    int trapeze, left, ltj, ltj_one, ltj_n_one;
    double color_norm = (residual < 1e-14)? 1: 1/(residual);
    double crn = color_norm*color_r;
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < 3; i++)
    {   
        trapeze = i*n_grid*(n_grid + 1);
        for(int l = 0; l < n_grid; l++)
        {
            left = l*(n_grid + 1);
            f_1 = (fabs(approx[left + trapeze] - func_draw[left + trapeze]));
            f_3 = fabs(approx[left + trapeze + n_grid + 1] - func_draw[left + trapeze + n_grid + 1]);
            for(int j = 0; j < n_grid; j++)
            {
                ltj = left + trapeze + j;
                ltj_one = ltj + 1;
                ltj_n_one = ltj_one + n_grid;
                f_2 = fabs(approx[ltj_one] - func_draw[ltj_one]);
                f_4 = fabs(approx[ltj_n_one + 1] - func_draw[ltj_n_one + 1]);

                qglColor(QColor((crn*f_1 > 255)? 255: crn*f_1, color_g, color_b));   
                glVertex3d(points_draw[ltj].x - pov_x, points_draw[ltj].y - pov_y, (0.7/residual)*(f_1 - pov_z));
                qglColor(QColor((crn*f_2 > 255)? 255: crn*f_2, color_g, color_b));                
                glVertex3d(points_draw[ltj_one].x - pov_x, points_draw[ltj_one].y - pov_y, (0.7/residual)*(f_2 - pov_z));
                qglColor(QColor((crn*f_3 > 255)? 255: crn*f_3, color_g, color_b));            
                glVertex3d(points_draw[ltj_n_one].x - pov_x, points_draw[ltj_n_one].y - pov_y, (0.7/residual)*(f_3 - pov_z));

                qglColor(QColor((crn*f_2 > 255)? 255: crn*f_2, color_g, color_b));                
                glVertex3d(points_draw[ltj_one].x - pov_x, points_draw[ltj_one].y - pov_y, (0.7/residual)*(f_2 - pov_z));
                qglColor(QColor((crn*f_3 > 255)? 255: crn*f_3, color_g, color_b));            
                glVertex3d(points_draw[ltj_n_one].x - pov_x, points_draw[ltj_n_one].y - pov_y, (0.7/residual)*(f_3 - pov_z));
                qglColor(QColor((crn*f_4 > 255)? 255: crn*f_4, color_g, color_b));            
                glVertex3d(points_draw[ltj_n_one + 1].x - pov_x, points_draw[ltj_n_one + 1].y - pov_y, (0.7/residual)*(f_4 - pov_z));

                f_1 = f_2;
                f_3 = f_4;
            }
        }
    }
    glEnd();
}

void Widget::draw_approx()
{
    double f_1, f_2, f_3, f_4;
    double color_r = 100, color_g = 255, color_b = 255;
    int trapeze, left, ltj, ltj_one, ltj_n_one;
    double color_norm = (max_approx - min_approx < 1e-13)? 1: 1/(max_approx - min_approx);
    double cgn = color_norm*color_g;
    double crn = color_norm*color_r;
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < 3; i++)
    {   
        trapeze = i*n_grid*(n_grid + 1);
        for(int l = 0; l < n_grid; l++)
        {
            left = l*(n_grid + 1);
            f_1 = approx[left + trapeze];
            f_3 = approx[left + trapeze + n_grid + 1];
            for(int j = 0; j < n_grid; j++)
            {
                ltj = left + trapeze + j;
                ltj_one = ltj + 1;
                ltj_n_one = ltj_one + n_grid;
                f_2 = approx[ltj_one];
                f_4 = approx[ltj_n_one + 1];

                qglColor(QColor(crn*(f_1 - min_approx), cgn*(f_1 - min_approx), color_b));   
                glVertex3d(points_draw[ltj].x - pov_x, points_draw[ltj].y - pov_y, f_1 - pov_z);
                qglColor(QColor(crn*(f_2 - min_approx), cgn*(f_2 - min_approx), color_b));                
                glVertex3d(points_draw[ltj_one].x - pov_x, points_draw[ltj_one].y - pov_y, f_2 - pov_z);
                qglColor(QColor(crn*(f_3 - min_approx), cgn*(f_3 - min_approx), color_b));            
                glVertex3d(points_draw[ltj_n_one].x - pov_x, points_draw[ltj_n_one].y - pov_y, f_3 - pov_z);

                qglColor(QColor(crn*(f_2 - min_approx), cgn*(f_2 - min_approx), color_b));                
                glVertex3d(points_draw[ltj_one].x - pov_x, points_draw[ltj_one].y - pov_y, f_2 - pov_z);
                qglColor(QColor(crn*(f_3 - min_approx), cgn*(f_3 - min_approx), color_b));            
                glVertex3d(points_draw[ltj_n_one].x - pov_x, points_draw[ltj_n_one].y - pov_y, f_3 - pov_z);
                qglColor(QColor(crn*(f_4 - min_approx), cgn*(f_4 - min_approx), color_b));            
                glVertex3d(points_draw[ltj_n_one + 1].x - pov_x, points_draw[ltj_n_one + 1].y - pov_y, f_4 - pov_z);

                f_1 = f_2;
                f_3 = f_4;
            }
        }
    }
    glEnd();
}

void Widget::draw_graph()
{
    double f_1, f_2, f_3, f_4;
    double color_r = 255, color_g = 255, color_b = 50;
    int trapeze, left, ltj, ltj_one, ltj_n_one;
    double color_norm = (max_draw - min_draw < 1e-17)? 1: 1/(max_draw - min_draw);
    double cgn = color_norm*color_g;
    double cbn = color_norm*color_b;
    double min_1, min_2, min_3, min_4;
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < 3; i++)
    {   
        trapeze = i*n_grid*(n_grid + 1);
        for(int l = 0; l < n_grid; l++)
        {
            left = l*(n_grid + 1);
            f_1 = func_draw[left + trapeze];
            min_1 = f_1 - min_draw;
            f_3 = func_draw[left + trapeze + n_grid + 1];
            min_3 = f_3 - min_draw;
            for(int j = 0; j < n_grid; j++)
            {
                ltj = left + trapeze + j;
                ltj_one = ltj + 1;
                ltj_n_one = ltj_one + n_grid;
                f_2 = func_draw[ltj_one];
                min_2 = f_2 - min_draw;
                f_4 = func_draw[ltj_n_one + 1];
                min_4 = f_4 - min_draw;
                qglColor(QColor(color_r, cgn*min_1, cbn*min_1));   
                glVertex3d(points_draw[ltj].x - pov_x, points_draw[ltj].y - pov_y, f_1 - pov_z);
                qglColor(QColor(color_r, cgn*min_2, cbn*min_2));                
                glVertex3d(points_draw[ltj_one].x - pov_x, points_draw[ltj_one].y - pov_y, f_2 - pov_z);
                qglColor(QColor(color_r, cgn*min_3, cbn*min_3));            
                glVertex3d(points_draw[ltj_n_one].x - pov_x, points_draw[ltj_n_one].y - pov_y, f_3 - pov_z);

                qglColor(QColor(color_r, cgn*min_2, cbn*min_2));                
                glVertex3d(points_draw[ltj_one].x - pov_x, points_draw[ltj_one].y - pov_y, f_2 - pov_z);
                qglColor(QColor(color_r, cgn*min_3, cbn*min_3));            
                glVertex3d(points_draw[ltj_n_one].x - pov_x, points_draw[ltj_n_one].y - pov_y, f_3 - pov_z);
                qglColor(QColor(color_r, cgn*min_4, cbn*min_4));            
                glVertex3d(points_draw[ltj_n_one + 1].x - pov_x, points_draw[ltj_n_one + 1].y - pov_y, f_4 - pov_z);

                f_1 = f_2;
                min_1 = min_2;
                f_3 = f_4;
                min_3 = min_4;
            }
        }
    }
    glEnd();
}

void Widget::draw_coordinate_grid()
{
    glTranslatef(-100.0 - pov_x, -pov_y, -pov_z);
    qglColor(QColor(225, 225, 225));
    for(int i = 0; i < 100; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(0.0, -100.0, 0.0);
        glVertex3f(0.0, 100.0, 0.0);
        glEnd();
        glTranslatef(1.0, 0.0, 0.0);
    }
    glTranslatef(1.0, 0.0, 0.0);
    for(int i = 101; i < 200; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(0.0, -100.0, 0.0);
        glVertex3f(0.0, 100.0, 0.0);
        glEnd();
        glTranslatef(1.0, 0.0, 0.0);
    }
    glTranslatef(-100, 0.0, 0.0);
    glTranslatef(pov_x, pov_y, pov_z);

    glTranslatef(-pov_x, -pov_y - 100.0, -pov_z);
    for(int i = 0; i < 100; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(-100.0, 0.0, 0.0);
        glVertex3f(100.0, 0.0, 0.0);
        glEnd();
        glTranslatef(0.0, 1.0, 0.0);
    }
    glTranslatef(0.0, 1.0, 0.0);
    for(int i = 101; i <= 199; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(-100.0, 0.0, 0.0);
        glVertex3f(100.0, 0.0, 0.0);
        glEnd();
        glTranslatef(0.0, 1.0, 0.0);
    }
    glTranslatef(pov_x, pov_y - 100.0, pov_z);
}

void Widget::coordinate_grid()
{
    coor_grid = (coor_grid + 1) % 2;
    paintGL();
}

void Widget::draw_split_grid()
{
    glBegin(GL_LINES);
    qglColor(QColor(Qt::black));

    glVertex3f(vertices[1].x - pov_x, vertices[1].y - pov_y, -pov_z);
    glVertex3f(cutout_vertices[1].x - pov_x, cutout_vertices[1].y - pov_y, -pov_z);

    glVertex3f(vertices[2].x - pov_x, vertices[2].y - pov_y, -pov_z);
    glVertex3f(cutout_vertices[2].x - pov_x, cutout_vertices[2].y - pov_y, -pov_z);
    for(int i = 0; i < 3; i++)
    {
        glVertex3f(vertices[i + 1].x - pov_x, vertices[i + 1].y - pov_y, -pov_z);
        glVertex3f(cutout_vertices[i].x - pov_x, cutout_vertices[i].y - pov_y, -pov_z);
        for(int j = 1; j < n; j++)
        {
            glVertex3f(vertices[i].x + j*(vertices[i + 1].x - vertices[i].x)/n - pov_x, vertices[i].y + j*(vertices[i + 1].y - vertices[i].y)/n - pov_y, -pov_z);
            glVertex3f(vertices[i].x + j*(cutout_vertices[i].x - vertices[i].x)/n - pov_x, vertices[i].y + j*(cutout_vertices[i].y - vertices[i].y)/n - pov_y, -pov_z);

            glVertex3f(vertices[i].x + j*(vertices[i + 1].x - vertices[i].x)/n - pov_x, vertices[i].y + j*(vertices[i + 1].y - vertices[i].y)/n - pov_y, -pov_z);
            glVertex3f(cutout_vertices[i].x + j*(vertices[i + 1].x - cutout_vertices[i].x)/n - pov_x, cutout_vertices[i].y + j*(vertices[i + 1].y - cutout_vertices[i].y)/n - pov_y, -pov_z);

            glVertex3f(cutout_vertices[i].x + j*(vertices[i + 1].x - cutout_vertices[i].x)/n - pov_x, cutout_vertices[i].y + j*(vertices[i + 1].y - cutout_vertices[i].y)/n - pov_y, -pov_z);
            glVertex3f(cutout_vertices[i].x + j*(vertices[i].x - cutout_vertices[i].x)/n - pov_x, cutout_vertices[i].y + j*(vertices[i].y - cutout_vertices[i].y)/n - pov_y, -pov_z);
        
            glVertex3f(cutout_vertices[i].x + j*(vertices[i + 1].x - cutout_vertices[i].x)/n - pov_x, cutout_vertices[i].y + j*(vertices[i + 1].y - cutout_vertices[i].y)/n - pov_y, -pov_z);
            glVertex3f(cutout_vertices[i].x + j*(cutout_vertices[i + 1].x - cutout_vertices[i].x)/n - pov_x, cutout_vertices[i].y + j*(cutout_vertices[i + 1].y - cutout_vertices[i].y)/n - pov_y, -pov_z);

            glVertex3f(cutout_vertices[i].x + j*(vertices[i + 1].x - cutout_vertices[i].x)/n - pov_x, cutout_vertices[i].y + j*(vertices[i + 1].y - cutout_vertices[i].y)/n - pov_y, -pov_z);
            glVertex3f(cutout_vertices[i + 1].x + j*(vertices[i + 1].x - cutout_vertices[i + 1].x)/n - pov_x, cutout_vertices[i + 1].y + j*(vertices[i + 1].y - cutout_vertices[i + 1].y)/n - pov_y, -pov_z);

            glVertex3f(cutout_vertices[i + 1].x + j*(cutout_vertices[i].x - cutout_vertices[i + 1].x)/n - pov_x, cutout_vertices[i + 1].y + j*(cutout_vertices[i].y - cutout_vertices[i + 1].y)/n - pov_y, -pov_z);
            glVertex3f(cutout_vertices[i + 1].x + j*(vertices[i + 1].x - cutout_vertices[i + 1].x)/n - pov_x, cutout_vertices[i + 1].y + j*(vertices[i + 1].y - cutout_vertices[i + 1].y)/n - pov_y, -pov_z);
        }
    }
    glEnd();
}

void Widget::draw_points(double radius)
{
    qglColor(QColor(Qt::red));  
    glTranslatef(-100.0 - pov_x, -pov_y, -pov_z);
    for(int i = 0; i < 200; i++)
    {
        gluSphere(m_qObj, radius, 10, 10);
        glTranslatef(1.0, 0.0, 0.0);
    }
    glTranslatef(-100, 0.0, 0.0);
    qglColor(QColor(Qt::black));
    gluSphere(m_qObj, 2*radius, 10, 10);
    glTranslatef(pov_x, pov_y, pov_z);
    qglColor(QColor(66, 170, 255));
    gluSphere(m_qObj, 1.5*radius, 10, 10);

    glTranslatef(-pov_x, -pov_y - 100.0, -pov_z);
    
    qglColor(QColor(Qt::green));
    for(int i = 0; i < 100; i++)
    {
        gluSphere(m_qObj, radius, 10, 10);
        glTranslatef(0.0, 1.0, 0.0);
    }
    glTranslatef(0.0, 1.0, 0.0);
    for(int i = 101; i <= 199; i++)
    {
        gluSphere(m_qObj, radius, 10, 10);
        glTranslatef(0.0, 1.0, 0.0);
    }
    glTranslatef(pov_x, pov_y - 100.0, pov_z);

    qglColor(QColor(Qt::blue));
    glTranslatef(-pov_x, -pov_y, -100.0 - pov_z);
    for(int i = 0; i <= 199; i++)
    {
        gluSphere(m_qObj, radius, 10, 10);
        glTranslatef(0.0, 0.0, 1.0);
    }
    glTranslatef(pov_x, pov_y, pov_z - 100.0);
}

void Widget::split_grid()
{
    spl_grid = (spl_grid + 1) % 2;
    paintGL();
}

void Widget::change_projection()
{
    proj = (proj + 1) % 2;
    paintGL();  
}

QSize Widget::minimumSizeHint() const
{
  return QSize(900, 900);
}

QSize Widget::sizeHint() const
{
  return QSize(1100, 1000);
}

void Widget::rotate_clockwise()
{
    rotate = (rotate - 15) % 360;
    paintGL();
}

void Widget::increase_scale()
{
    scale++;
    paintGL();
}

void Widget::reduce_scale()
{
    scale = (scale - 1 < 1)? 1: scale - 1;
    paintGL();
}

void Widget::rotate_counterclockwise()
{
    rotate = (rotate + 15) % 360;
    paintGL();
}

void Widget::change_graph()
{
    GRAPH = (GRAPH + 1) % 3;
    paintGL();
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    double diag_1 = sqrt(pow(vertices[0].x - vertices[2].x, 2) + pow(vertices[0].y - vertices[2].y, 2));
    double diag_2 = sqrt(pow(vertices[1].x - vertices[3].x, 2) + pow(vertices[1].y - vertices[3].y, 2));
    double diag = (diag_1 > diag_2)? diag_1: diag_2;

    // double radius = residual;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(proj == 0) glOrtho(-1.0, 1.0, -1.0, 1.0, -10, 10);
    if(proj == 1) gluPerspective(45, 1, 0.1, 30);
    glMatrixMode(GL_MODELVIEW);    
    glLoadIdentity();
    if(proj == 1) glTranslatef(0.0, 0.0, -3);
    glRotatef(-90, 0.1, 0.0, 0.0);
    glRotatef(-45 - rotate, 0.0, 0.0, 1.0);
    rot.x = cos(rotate*PI/180)*(-1.0) - sin(rotate*PI/180)*(-1.0);
    rot.y = sin(rotate*PI/180)*(-1.0) + cos(rotate*PI/180)*(-1.0);

    glRotatef(-19, rot.x, rot.y, 0.0);

    switch(GRAPH)
    {
        case 0:
            glScaled(scale*1.5/diag, scale*1.5/diag, scale*0.7/max_draw);
            break;
        case 1:
            glScaled(scale*1.5/diag, scale*1.5/diag, scale*0.7/max_approx);
            break;
        case 2:
            glScaled(scale*1.5/diag, scale*1.5/diag, scale);  
            break;
    }

    switch(GRAPH)
    {
        case 0:
            draw_graph();
            break;
        case 1:
            draw_approx();
            break;
        case 2:
            draw_residual();
            break;
    }

    if(coor_grid == 1) draw_coordinate_grid();
    draw_points(0.008*diag);
    glBegin(GL_LINES);
    qglColor(QColor(Qt::red));
    glVertex3f(-100.0 - pov_x, -pov_y, -pov_z);
    glVertex3f(100.0 - pov_x, -pov_y, -pov_z);
    
    qglColor(QColor(Qt::green));
    glVertex3f(-pov_x, -100.0 - pov_y, -pov_z);
    glVertex3f(-pov_x, 100.0 - pov_y, -pov_z);
    
    qglColor(QColor(Qt::blue));
    glVertex3f(-pov_x, -pov_y, -100.0 - pov_z);
    glVertex3f(-pov_x, -pov_y, 100.0 - pov_z);
    glEnd();

    qglColor(QColor(194, 132, 224));
    for(int k = 0; k < 3; k++)
    {
        glBegin(GL_POLYGON);
        glVertex3f(GLfloat(vertices[k].x - pov_x), GLfloat(vertices[k].y - pov_y), -pov_z);
        glVertex3f(GLfloat(vertices[k + 1].x - pov_x), GLfloat(vertices[k + 1].y - pov_y), -pov_z);
        glVertex3f(GLfloat(cutout_vertices[k + 1].x - pov_x), GLfloat(cutout_vertices[k + 1].y - pov_y), -pov_z);
        glVertex3f(GLfloat(cutout_vertices[k].x - pov_x), GLfloat(cutout_vertices[k].y - pov_y), -pov_z);
        glEnd();
    }
    if(spl_grid == 1) draw_split_grid();
    qglColor(QColor(Qt::black));
    renderText(20, 50, func_name);
    renderText(20, 75, "n = " + QString::number(n) + "   p = " + QString::number(p_error) + "   scale = " + QString::number(scale));
    renderText(20, 100, "Residual = " + QString::number(residual));
    renderText(20, 150, "Rotate = " + QString::number(rotate) + "°");


    switch(GRAPH)
    {
        case 0:
            qglColor(QColor(255, 125, 25));
            renderText(20, 25, "Graph of function");
            qglColor(QColor(Qt::black));
            renderText(20, 125, "|F_min| = " + QString::number(fabs(min)) + "   |F_max| = " + QString::number(fabs(max)));
            cout<<"|F_min| = "<<fabs(min)<<" |F_max| = "<<fabs(max)<<endl;
            break;
        case 1:
            qglColor(QColor(125, 125, 210));
            renderText(20, 25, "Graph of approximation");
            qglColor(QColor(Qt::black));
            renderText(20, 125, "|F_min| = " + QString::number(fabs(min_approx)) + "   |F_max| = " + QString::number(fabs(max_approx)));
            cout<<"|F_min| = "<<fabs(min_approx)<<" |F_max| = "<<fabs(max_approx)<<endl;
            break;
        case 2:
            qglColor(QColor(125, 255, 210));
            renderText(20, 25, "Graph of residual");
            qglColor(QColor(Qt::black));
            renderText(20, 125, "|F_min| = " + QString::number(0) + "   |F_max| = " + QString::number(residual));
            cout<<"|F_min| = "<<0<<" |F_max| = "<<residual<<endl;
            break;
    }
    if(init == 1)  swapBuffers();
    init = 1;
}

Widget::~Widget()
{
    pthread_barrier_destroy(&barrier);
    delete [] threads;
    delete [] vertices;
    delete [] cutout_vertices;
    delete [] points;
    delete [] approx;
    delete [] func;
    delete [] func_draw;
    delete [] points_draw;
    delete [] x;
}
