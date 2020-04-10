#include <QtWidgets/QtWidgets>

#define NUMBER_OF_FUNCTIONS 7

class Window : public QWidget
{
  Q_OBJECT

private:
  double a;
  double b;
  int N;
  int k;
  double scale = 1;
  int p = 0;
  int approximation_id = 0;
  double max_abs_value = 0;
  bool FUNCTION = true;
  bool APPROX_0 = true;
  bool APPROX_1 = false;
  bool RESIDUAL_0 = false;
  bool RESIDUAL_1 = false;
  const char *approximation_name;
  const char *function_name;
  double *x;
  double *func;
  double *func_der;
  double *approx_0;
  double *approx_1;
  double (*f) (double);
  double (*f_der) (double);
  double (*polynom_val) (double, double, double, int, double*);
  int (*approximation) (int , double*, double*, double*, double*);

public:
  Window (QWidget *parent);
  ~Window();
  QSize minimumSizeHint() const;
  QSize sizeHint() const;
  int command_line(int argc, char *argv[]);
  void paintEvent (QPaintEvent *event);
  void draw_function(QPainter *painter);
  void draw_approximation(QPainter *painter, double *approx);
  void draw_residual(QPainter *painter, double *approx);
  void find_min_max(double *approx_0, double *approx_1, double *min, double *max, double scale);
  void residual_min_max(double *min, double *max, double *approx, double scale);
  void approximation_min_max(double *min, double *max, double *approx, double scale);
  void function_min_max(double *min, double *max, double scale);
  void set_function_array();
  void calculate_approximation();
  void find_abs_max();

public slots:
  void change_function();
  void change_approximation();
  void choose_function();
  void choose_approximation();
  void reduce_n();
  void increase_n();
  void increase_scale();
  void decrease_scale();
  void subtract_delta();
  void add_delta();
};
