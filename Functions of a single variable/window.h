
#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets/QtWidgets>

class Window : public QWidget
{
  Q_OBJECT

private:
  // int func_id;
  int approx_id;
  const char *f_name;
  const char *approx_name;
  double a;
  double b;
  int N;
  // double (*fun) (double);
  int (*approx) (double, double, int, double*);

public:
  Window (QWidget *parent);

  QSize minimumSizeHint () const;
  QSize sizeHint () const;

  int parse_command_line (int argc, char *argv[]);

public slots:
  void change_func ();

protected:
  void paintEvent (QPaintEvent *event);
};

#endif
