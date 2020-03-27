#include "window.h"
#include "header.hpp"

#define DEFAULT_A -10
#define DEFAULT_B 10
#define DEFAULT_N 11
#define SCREEN_SIZE 1980
#define NUMBER_OF_FUNCTIONS 2

Window::Window (QWidget *parent) : QWidget (parent)
{
  a = DEFAULT_A;
  b = DEFAULT_B;
  N = DEFAULT_N;

  approx_id = 0;
  change_approx();
  change_func();
}

QSize Window::minimumSizeHint() const
{
  return QSize(100, 100);
}

QSize Window::sizeHint() const
{
  return QSize(2000, 1500);
}

int Window::parse_command_line (int argc, char *argv[])
{
  if (argc == 1)  return 0;
  if (argc == 2)  return -1;

  if (   sscanf (argv[1], "%lf", &a) != 1
      || sscanf (argv[2], "%lf", &b) != 1
      || b - a < 1.e-6
      || (argc > 3 && sscanf (argv[3], "%d", &N) != 1)
      || N < 2)
    return -2;

  return 0;
}

void Window::change_approx()
{
  approx_id = (approx_id + 1) % 2;
  switch (approx_id)
  {
    case 0:
      approx_name = "Newton's interpolation with multiple nodes";
      approx = approximation_0;
      polynom_val = polynomial_value_0;
      break;
    case 1:
      approx_name = "Piecewise interpolation by cubic Bessel polynomials";
      approx = approximation_1;
      polynom_val = polynomial_value_1;
      break;
  }
  update();
}

void Window::change_func()
{
  func_id = (func_id + 1) % NUMBER_OF_FUNCTIONS;
  switch (func_id)
  {
    case 0:
      func_name = "First func";
      f = f_0;
      f_der = f_der_0;
      break;
    case 1:
      func_name = "Second func";
      f = f_1;
      f_der = f_der_1;
      break;
  }
  update ();
}

void Window::paintEvent (QPaintEvent *)
{  
  QPainter painter (this);
  double x1, x2, y1, y2;
  double max_y, min_y, delta_y;
  double step = (b - a)/width();
  double *coef;
  coef = new double[4*N];
  memset(coef, 0, 4*N*sizeof(double));
  approx(f, f_der, a, b, N, coef);

  QPen balck_pen(Qt::black, 0, Qt::SolidLine); 
  QPen red_pen(Qt::red, 0, Qt::SolidLine); 
  QPen blue_pen(Qt::blue, 0, Qt::SolidLine); 

  max_y = 0;
  min_y = 0;
  for (x1 = a; x1 - b < step; x1 += step)
  {
    y1 = polynom_val(x1, a, b, N, coef);
    if (y1 < min_y) min_y = y1;
    if (y1 > max_y) max_y = y1;
  }
  delta_y = 0.01*(max_y - min_y);
  min_y -= delta_y;
  max_y += delta_y;

  painter.save();

  painter.translate(0.5*width(), 0.5*height());
  painter.scale(width()/(b - a), -height()/(max_y - min_y));
  painter.translate(-0.5*(a + b), -0.5*(min_y + max_y));

  painter.setPen(red_pen);
  painter.drawLine(QPointF(a, 0), QPointF(b, 0));
  painter.drawLine(QPointF(0, min_y), QPointF(0 , max_y));

  painter.setPen (balck_pen);
  x1 = a;
  y1 = polynom_val(x1, a, b, N, coef);
  for (x2 = x1 + step; x2 - b < step; x2 += step) 
  {
    y2 = polynom_val(x2, a, b, N, coef);
    painter.drawLine (QPointF(x1, y1), QPointF(x2, y2));

    x1 = x2, y1 = y2;
  }
  x2 = b;
  y2 = polynom_val(x2, a, b, N, coef);
  painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));

  painter.setPen(blue_pen);
  x1 = a;
  y1 = f(x1);
  for (x2 = x1 + step; x2 - b < step; x2 += step) 
  {
    y2 = f(x2);
    painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
    x1 = x2, y1 = y2;
  }
  x2 = b;
  y2 = f(x2);
  painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));

  painter.restore();

  painter.setPen("blue");
  painter.drawText(0, 20, approx_name);
  delete [] coef;
}
