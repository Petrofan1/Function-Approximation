struct point {
    double x;
    double y;
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
        // double j_1;
        // double j_2;

        //Функция подсчета якобиана
        double jacobian(point a, point b, point c);

    public:

        Triangle(point a, point b, point c, int n);
        ~Triangle();
        double integral(double f_1, double f_2, double f_3, double g_1, double g_2, double g_3, point, point, point);
        double get_jacobian();
};