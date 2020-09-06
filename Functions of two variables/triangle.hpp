struct point 
{
    double x;
    double y;

    point operator + (const point p)
    {
        point temp;
        temp.x = this->x + p.x;
        temp.y = this->y + p.y;
        return temp;  
    }

    point operator - (const point p)
    {
        point temp;
        temp.x = this->x - p.x;
        temp.y = this->y - p.y;
        return temp;  
    }

    point operator / (const double a)
    {
        point temp;
        temp.x = this->x/a;
        temp.y = this->y/a;
        return temp;  
    }
    point operator * (const double a)
    {
        point temp;
        temp.x = this->x*a;
        temp.y = this->y*a;
        return temp;  
    }
};

struct arg
{
    double *matrix_msr;
    double *right;
    double *jacobi_matrix_msr;
    double *u;
    double *v;
    double *r; 
    double *x;
    double *scalar_product_1;
    double *scalar_product_2;
    double eps;
    int *vector_msr;
    int *jacobi_vector_msr;
    int *is_ready;
    int n;
    int p;
    int num;
    pthread_barrier_t* barrier;
    
};

struct thr
{
    double (*f) (point);
    double *x;
    point *vertices;
    point *cutout_vertices;
    point *points;
    double *func; 
    int n;
    int p;
    int p_error;
    double *max;
    double *min;
    double eps;
    pthread_t *threads;
    pthread_barrier_t *barrier;
    int *is_ready;
    thr *th;
};

// Класс, отвечающий за треугольники, на которые разбивается четырёхугольник с вырезом
class Triangle 
{
    private:

        // Вершины треугольника
        point A;
        point B;
        point C;

        // Количество точек разбиения
        int n;

        // Якобианы для двух видов треугольников
        double Jacobian;

        //Функция подсчета якобиана
        double jacobian(point a, point b, point c);

    public:

        Triangle(point a, point b, point c, int n);
        ~Triangle();
        double integral(double f_1, double f_2, double f_3, double g_1, double g_2, double g_3, point, point, point);
        double get_jacobian();
};