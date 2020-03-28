#include "window.h"
#include "header.hpp"

#define DEFAULT_A -10
#define DEFAULT_B 10
#define DEFAULT_N 11
#define SCREEN_SIZE 1980
#define NUMBER_OF_FUNCTIONS 5

Window::Window (QWidget *parent) : QWidget (parent)
{
  a = DEFAULT_A;
  b = DEFAULT_B;
  N = DEFAULT_N;

  scale_y = 0;
  scale_x = 0;
  approx_id = 0;
  RESIDUAL = false;
  change_approx();
  change_func();
}

QSize Window::minimumSizeHint() const
{
  return QSize(500, 500);
}

QSize Window::sizeHint() const
{
  return QSize(2000, 1500);
}

int Window::command_line (int argc, char *argv[])
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
void Window::residual()
{
  RESIDUAL = (RESIDUAL == false)? true: false;
  update();
}

void Window::reduce_n()
{
  N = (N/2 < 3)? 3: N/2; 
  update();
}
void Window::increase_scale()
{
  scale_x *= 1.1;
  scale_y *= 1.1;
  update();
}
void Window::decrease_scale()
{
  scale_x *= 0.9;
  scale_y *= 0.9;
  update();
}
void Window::increase_n()
{
  N *= 2;
  update();
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
      func_name = "f(x) = x";
      f = f_0;
      f_der = f_der_0;
      break;
    case 1:
      func_name = "f(x) = x²";
      f = f_1;
      f_der = f_der_1;
      break;
    case 2:
      func_name = "f(x) = x^3";
      f = f_2;
      f_der = f_der_2;
      break;
    case 3:
      func_name = "f(x) = exp(x)";
      f = f_3;
      f_der = f_der_3;
      break;
    case 4:
      func_name = "f(x) = cos(x)";
      f = f_4;
      f_der = f_der_4;
      break;
  }
  update();
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

  QPen black_pen(Qt::black, 0, Qt::SolidLine); 
  QPen red_pen(Qt::red, 0, Qt::SolidLine); 
  QPen blue_pen(Qt::blue, 0, Qt::SolidLine);
  QPen green_pen(Qt::green, 0, Qt::SolidLine);

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
  if(scale_x == 0 || scale_y == 0)
  {
    scale_x = b - a;
    scale_y = max_y - min_y; 
  }
  painter.translate(0.5*width(), 0.5*height());
  painter.scale(width()/scale_x, -height()/scale_y);
  painter.translate(-0.5*(a + b), -0.5*(min_y + max_y));

  painter.setPen(red_pen);
  painter.drawLine(QPointF(-width(), 0), QPointF(width(), 0));
  painter.drawLine(QPointF(0, -height()), QPointF(0 , height()));
  painter.setPen(black_pen);


  painter.setPen (black_pen);
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
  if(RESIDUAL == true)
  {
    painter.setPen(green_pen);
    x1 = a;
    y1 = fabs(f(x1) - polynom_val(x1, a, b, N, coef));
    for (x2 = x1 + step; x2 - b < step; x2 += step) 
    {
      y2 = fabs(f(x2) - polynom_val(x2, a, b, N, coef));
      painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
      x1 = x2, y1 = y2;
    }
    x2 = b;
    y2 = fabs(f(x2) - polynom_val(x2, a, b, N, coef));
    painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
  }

  painter.restore();
  QString draw_N, draw_a_b;
  draw_a_b.append("a = ");
  draw_a_b.append(QString::number(a));
  draw_a_b.append(" b = ");
  draw_a_b.append(QString::number(b));
  draw_N.append("n = ");
  draw_N.append(QString::number(N));
  painter.setPen(black_pen);
  painter.drawText(10, 20, approx_name);
  painter.setPen(blue_pen);
  painter.drawText(10, 40, func_name);
  painter.setPen(black_pen);
  painter.drawText(10, 60, draw_N);
  painter.drawText(10, 80, draw_a_b);
  delete [] coef;
}
