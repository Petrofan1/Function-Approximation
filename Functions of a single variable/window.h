#include <QtWidgets/QtWidgets>

class Window : public QWidget
{
  Q_OBJECT

private:
  int approx_id;
  int func_id;
  const char *approx_name;
  const char *func_name;
  double a;
  double b;
  double scale_x;
  double scale_y;
  int N;
  bool RESIDUAL;
  double (*f) (double);
  double (*f_der) (double);
  double (*polynom_val) (double, double, double, int, double*);
  int (*approx) (double (*f) (double), double (*f_der) (double), double, double, int, double*);

public:
  Window (QWidget *parent);
  QSize minimumSizeHint() const;
  QSize sizeHint() const;
  int command_line(int argc, char *argv[]);
  void paintEvent (QPaintEvent *event);

public slots:
  void change_func();
  void change_approx();
  void reduce_n();
  void increase_n();
  void residual();
  void increase_scale();
  void decrease_scale();
};
