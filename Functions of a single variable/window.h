#include <QtWidgets/QtWidgets>

class Window : public QWidget
{
  Q_OBJECT

private:
  double a;
  double b;
  int N;
  int k;
  double scale = 1;
  int p = 1;
  int approximation_id = 0;
  bool FUNCTION = true;
  bool APPROX_0 = true;
  bool APPROX_1 = false;
  bool RESIDUAL_0 = false;
  bool RESIDUAL_1 = false;
  const char *approximation_name;
  const char *function_name;
  double (*f) (double);
  double (*f_der) (double);
  double (*polynom_val) (double, double, double, int, double*);
  int (*approximation) (int , double*, double*, double*, double*);

public:
  Window (QWidget *parent);
  QSize minimumSizeHint() const;
  QSize sizeHint() const;
  int command_line(int argc, char *argv[]);
  void paintEvent (QPaintEvent *event);
  void drawFunction(QPainter *painter);
  void drawApproximation(QPainter *painter, double *approx);
  void drawResidual(QPainter *painter, double *approx);
  void findMinMax(double *approx_0, double *approx_1, double *min, double *max, double scale);
  void residualMinMax(double *min, double *max, double *approx, double scale);
  void approximationMinMax(double *min, double *max, double *approx, double scale);
  void functionMinMax(double *min, double *max, double scale);

public slots:
  void change_function();
  void change_approximation();
  void choose_function();
  void choose_approximation();
  void reduce_n();
  void increase_n();
  void increase_scale();
  void decrease_scale();
};
