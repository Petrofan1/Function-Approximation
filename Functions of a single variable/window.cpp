#include "window.h"
#include "header.hpp"

#define DEFAULT_A -10
#define DEFAULT_B 10  
#define DEFAULT_N 1980

Window::Window (QWidget *parent)
  : QWidget (parent)
{
  a = DEFAULT_A;
  b = DEFAULT_B;
  N = DEFAULT_N;

  approx_id = 0;

  change_func ();
}

QSize Window::minimumSizeHint () const
{
  return QSize (100, 100);
}

QSize Window::sizeHint () const
{
  return QSize (1000, 1000);
}

int Window::parse_command_line (int argc, char *argv[])
{
  if (argc == 1)
    return 0;
  if (argc == 2)
    return -1;

  if (   sscanf (argv[1], "%lf", &a) != 1
      || sscanf (argv[2], "%lf", &b) != 1
      || b - a < 1.e-6
      || (argc > 3 && sscanf (argv[3], "%d", &N) != 1)
      || N <= 0)
    return -2;

  return 0;
}

/// change current function for drawing
void Window::change_func ()
{
  approx_id = (approx_id + 1) % 2;

  switch (approx_id)
  {
    case 0:
      approx_name = "Newton's interpolation with multiple nodes";
      approx = approximation_0;
      break;
    case 1:
      approx_name = "Other method";
      approx = approximation_1;
      break;
  }
  update ();
}

/// render graph
void Window::paintEvent (QPaintEvent * /* event */)
{  
  QPainter painter (this);
  double x1, x2, y1, y2;
  double max_y, min_y;
  double delta_y, delta_x = (b - a) / N;
  int n = 3; // степень многочлена
  double *coef;
  coef = new double[n + 1];

  approx(a, b, n, coef);

  QPen pen_black(Qt::black, 0, Qt::SolidLine); 
  QPen pen_red(Qt::red, 0, Qt::SolidLine); 

  painter.setPen (pen_black);

  // calculate min and max for current function
  max_y = 0;
  min_y = 0;
  for (x1 = a; x1 - b < 1.e-6; x1 += delta_x)
  {
    y1 = polynomial_value(x1, coef, n);
    if (y1 < min_y)
      min_y = y1;
    if (y1 > max_y)
      max_y = y1;
  }
  delta_y = 0.01*(max_y - min_y);
  min_y -= delta_y;
  max_y += delta_y;

  // save current Coordinate System
  painter.save ();

  // make Coordinate Transformations
  painter.translate (0.5 * width (), 0.5 * height ());
  painter.scale (width () / (b - a), -height () / (max_y - min_y));
  painter.translate (-0.5 * (a + b), -0.5 * (min_y + max_y));

  // draw approximated line for graph
  x1 = a;
  y1 = polynomial_value(x1, coef, n);
  for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x) 
  {
    y2 = polynomial_value(x2, coef, n);
    painter.drawLine (QPointF (x1, y1), QPointF (x2, y2));

    x1 = x2, y1 = y2;
  }
  x2 = b;
  y2 = polynomial_value(x2, coef, n);
  painter.drawLine (QPointF (x1, y1), QPointF (x2, y2));

  // draw axis
  painter.setPen (pen_red);
  painter.drawLine (a, 0, b, 0);
  painter.drawLine (0, max_y, 0, min_y);

  // restore previously saved Coordinate System
  painter.restore ();

  // render function name
  painter.setPen ("blue");
  painter.drawText (0, 20, approx_name);
  delete [] coef;
}
