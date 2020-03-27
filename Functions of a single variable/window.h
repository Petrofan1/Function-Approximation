
#ifndef WINDOW_H
#define WINDOW_H

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
  int N;
  // double (*fun) (double);
  double (*f) (double);
  double (*f_der) (double);
  double (*polynom_val) (double, double, double, int, double*);
  int (*approx) (double (*f) (double), double (*f_der) (double), double, double, int, double*);
public:
  Window (QWidget *parent);

  QSize minimumSizeHint () const;
  QSize sizeHint () const;

  int parse_command_line (int argc, char *argv[]);

public slots:
  void change_func();
  void change_approx();

protected:
  void paintEvent (QPaintEvent *event);
};

#endif
