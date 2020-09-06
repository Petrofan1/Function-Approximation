#ifndef HEADER_HPP
#define HEADER_HPP
#include "header.hpp"
#endif

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP
#include "triangle.hpp"
#endif

#include <QGLWidget>
#include <QTimer>
#include <QtGui>
#include <GL/gl.h>
#include <GL/glu.h>

class Widget: public QGLWidget
{
    Q_OBJECT
    private:
        point *vertices;                                        // Vertices of area
        point *cutout_vertices;                                 // Vertices of cutout on area
        point *points;                                          // Array of points 
        point *points_draw;
        point center;                                           // Coordinates of diagonales intersection
        double *func;
        double *approx;
        double *func_draw;
        int scale = 1;
        double *x;
        GLUquadricObj* m_qObj;                                  // Quadric object
        int rotate = 0;                                         // Angle of rotate
        int coor_grid = 1;                                      // Flag that indicates whether or not to draw a coordinate grid
        int spl_grid = 0;                                       // Flag that indicates whether or not to draw a split grid
        int n_grid = NUMBER_OF_POINTS_FOR_DRAWING;              // Number of split points for drawing graph of a function
        int n = 32;                                                  // Split parameter
        int proj = 1;                                           // Type of projection: 0 - ortho, 1 - perspective
        double (*f) (point);                                    // Function to be approximated
        const char *func_name;
        point rot;                                              // Coordinate of vector to rotate around
        double eps = 1e-16;
        double max;
        double min;
        double max_approx;
        double min_approx;
        double min_draw;
        double max_draw;
        double residual;
        int k = 1;
        int p = 4;
        int p_error = 0;
        int pov = 0;
        double pov_x;
        double pov_y;
        double pov_z;
        int GRAPH = 0;
        int init = 0;
        pthread_t  *threads;
        pthread_barrier_t barrier;
        QTimer timer;
        int is_ready = 0;

    public:
        QPushButton *m_button;
        Widget(QWidget *parent = 0, char* filename = 0, int k = 1, int n = 32, double eps = 1e-16, int p = 4);
        QSize minimumSizeHint() const;
        QSize sizeHint() const;
        ~Widget();

    protected:
        void initializeGL();
        void resizeGL(int nWidth, int nHeight);
        void paintGL();
        void draw_split_grid();
        void draw_coordinate_grid();
        void draw_graph();
        void draw_approx();
        void draw_residual();
        void draw_points(double radius);
        void choose_function();
        void choose_point_of_view();
        void calculate_solution();


    public slots:   
        void rotate_clockwise();
        void rotate_counterclockwise();
        void split_grid();
        void coordinate_grid();
        void change_projection();
        void change_function();
        void change_point_of_view();
        void increase_n();
        void reduce_n();
        void increase_p();
        void reduce_p();
        void change_graph();
        void increase_scale();
        void reduce_scale();
        void check_result();
};

