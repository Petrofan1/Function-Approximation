#include "window.h"
#include "header.hpp"
#define SCREEN_SIZE 1980
#define NUMBER_OF_FUNCTIONS 7

Window::Window (QWidget *parent) : QWidget (parent)
{

}

QSize Window::minimumSizeHint() const
{
  return QSize(500, 500);
}

QSize Window::sizeHint() const
{
  return QSize(1000, 1000);
}

int Window::command_line(int argc, char *argv[])
{
  if (   argc < 5
      || sscanf (argv[1], "%lf", &a) != 1
      || sscanf (argv[2], "%lf", &b) != 1
      || b - a < 1.e-6
      || sscanf (argv[3], "%d", &N) != 1
      || N < 3
      || sscanf(argv[4], "%d", &k) != 1
      || (k < 0 || k > NUMBER_OF_FUNCTIONS - 1)) return -2;
  choose_function();
  choose_approximation();
  update();
  return 0;
}

void Window::reduce_n()
{
  N = (N/2 < 3)? 3: N/2; 
  update();
}
void Window::increase_scale()
{
  scale *= 2;
  update();
}
void Window::decrease_scale()
{
  scale *= 0.5;
  update();
}
void Window::increase_n()
{
  N *= 2;
  update();
}

void Window::choose_approximation()
{
  switch (approximation_id)
  {
    case 0:
      approximation_name = "Newton's interpolation with multiple nodes";
      FUNCTION = true;
      APPROX_0 = true;
      APPROX_1 = false;
      RESIDUAL_0 = false;
      RESIDUAL_1 = false;
      break;
    case 1:
      approximation_name = "Piecewise interpolation by cubic Bessel polynomials";
      FUNCTION = true;
      APPROX_0 = false;
      APPROX_1 = true;
      RESIDUAL_0 = false;
      RESIDUAL_1 = false;
      break;
    case 2:
      approximation_name = "Newton's interpolation with multiple nodes + Piecewise interpolation by cubic Bessel polynomials";
      FUNCTION = true;
      APPROX_0 = true;
      APPROX_1 = true;
      RESIDUAL_0 = false;
      RESIDUAL_1 = false;
      break;
    case 3:
      approximation_name = "Errors of the first and second approximations";
      FUNCTION = false;
      APPROX_0 = false;
      APPROX_1 = false;
      RESIDUAL_0 = true;
      RESIDUAL_1 = true;
      break;
  }
}

void Window::change_approximation()
{
  approximation_id = (approximation_id + 1) % 4;
  choose_approximation();
  update();
}

void Window::choose_function()
{
  switch (k)
  {
    case 0:
      function_name = "k = 0 f(x) = 1";
      f = f_0;
      f_der = f_der_0;
      break;
    case 1:
      function_name = "k = 1 f(x) = x";
      f = f_1;
      f_der = f_der_1;
      break;
    case 2:
      function_name = "k = 2 f(x) = x²";
      f = f_2;
      f_der = f_der_2;
      break;
    case 3:
      function_name = "k = 3 f(x) = x^3";
      f = f_3;
      f_der = f_der_3;
      break;
    case 4:
      function_name = "k = 4 f(x) = x^4";
      f = f_4;
      f_der = f_der_4;
      break;
    case 5:
      function_name = "k = 5 f(x) = exp(x)";
      f = f_5;
      f_der = f_der_5;
      break;
    case 6:
      function_name = "k = 6 f(x) = 1/(25x² + 1)";
      f = f_6;
      f_der = f_der_6;
      break;
  }
}

void Window::change_function()
{
  k = (k + 1) % NUMBER_OF_FUNCTIONS;
  choose_function();
  update();
}

void Window::drawFunction(QPainter *painter)
{
  double x1, x2, y1, y2, step;
  x1 = a;
  y1 = f(x1);
  step = (b - a)/width();
  for (x2 = x1 + step; x2 - b < step; x2 += step) 
  {
    y2 = f(x2);
    painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
    x1 = x2, y1 = y2;
  }
  x2 = b;
  y2 = f(x2);
  painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
}

void Window::drawApproximation(QPainter *painter, double *approx)
{
  double x1, x2, y1, y2, step;
  x1 = a;
  y1 = polynom_val(x1, a, b, N, approx);
  step = (b - a)/width();
  for (x2 = x1 + step; x2 - b < step; x2 += step) 
  {
    y2 = polynom_val(x2, a, b, N, approx);
    painter->drawLine (QPointF(x1, y1), QPointF(x2, y2));
    x1 = x2, y1 = y2;
  }
  x2 = b;
  y2 = polynom_val(x2, a, b, N, approx);
  painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
}

void Window::drawResidual(QPainter *painter, double *approx)
{
  double x1, x2, y1, y2, step;
  x1 = a;
  y1 = fabs(f(a)- polynom_val(x1, a, b, N, approx));
  step = (b - a)/width();
  for (x2 = x1 + step; x2 - b < step; x2 += step) 
  {
    y2 = fabs(f(x2) - polynom_val(x2, a, b, N, approx));
    painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
    x1 = x2, y1 = y2;
  }
  x2 = b;
  y2 = fabs(f(x2) - polynom_val(x2, a, b, N, approx));
  painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
}

void Window::functionMinMax(double *min, double *max, double scale)
{
  double x, y, step, local_a, local_b;
  local_a = (a + b)/2 - 0.5*(b - a)*scale;
  local_b = (a + b)/2 + 0.5*(b - a)*scale;  
  step = scale*(b - a)/width();
  *min = f(local_a);
  *max = f(local_a);
  for (x = local_a; x - local_b <= 0; x += step)
  {
    y = f(x);
    *min = (*min < y)? *min: y;
    *max = (*max > y)? *max: y;
  }
  std::cout<<"Function min = "<<*min<<std::endl;
  std::cout<<"Function max = "<<*max<<std::endl;

}

void Window::approximationMinMax(double *min, double *max, double *approx, double scale)
{
  double x, y, step, local_a, local_b;
  local_a = (a + b)/2 - 0.5*(b - a)*scale;
  local_b = (a + b)/2 + 0.5*(b - a)*scale; 
  step = scale*(b - a)/width();
  *min = polynom_val(local_a, a, b, N, approx);
  *max = polynom_val(local_a, a, b, N, approx);;
  for (x = local_a; x - local_b <= 0; x += step)
  {
    y = polynom_val(x, a, b, N, approx);
    *min = (*min < y)? *min: y;
    *max = (*max > y)? *max: y;
  }
}

void Window::residualMinMax(double *min, double *max, double *approx, double scale)
{
  double x, y, step, local_a, local_b;
  step = scale*(b - a)/width();
  local_a = (a + b)/2 - 0.5*(b - a)*scale;
  local_b = (a + b)/2 + 0.5*(b - a)*scale; 
  *min = fabs(f(local_a) - polynom_val(local_a, a, b, N, approx));
  *max = fabs(f(local_a) - polynom_val(local_a, a, b, N, approx));
  for (x = local_a; x - local_b <= 0; x += step)
  {
    y = fabs(f(x) - polynom_val(x, a, b, N, approx));
    *min = (*min < y)? *min: y;
    *max = (*max > y)? *max: y;
  }
}

void Window::findMinMax(double *approx_0, double *approx_1, double *min, double *max, double scale)
{
  double local_min = 0, local_max = 0, local_scale = (scale > 1)? 1: scale;
  switch(approximation_id)
  {
    case 0:
      functionMinMax(&local_min, &local_max, local_scale);
      *min = local_min;
      *max = local_max;
      polynom_val = polynomial_value_0;
      if(N < 50)
      {
        approximationMinMax(&local_min, &local_max, approx_0, local_scale);
        *min = (*min < local_min)? *min: local_min;
        *max = (*max > local_max)? *max: local_max;
      }
      break;
    case 1:
      functionMinMax(&local_min, &local_max, local_scale);
      *min = local_min;
      *max = local_max;
      if(N < 50)
      {
        polynom_val = polynomial_value_1;
        approximationMinMax(&local_min, &local_max, approx_1, local_scale);
        *min = (*min < local_min)? *min: local_min;
        *max = (*max > local_max)? *max: local_max;
      }
      break;
    case 2:
      functionMinMax(&local_min, &local_max, local_scale);
      *min = local_min;
      *max = local_max;
      if(N < 50)
      {
        polynom_val = polynomial_value_0;
        approximationMinMax(&local_min, &local_max, approx_0, local_scale);
        *min = (*min < local_min)? *min: local_min;
        *max = (*max > local_max)? *max: local_max;
      }
      polynom_val = polynomial_value_1;
      approximationMinMax(&local_min, &local_max, approx_1, local_scale);
      *min = (*min < local_min)? *min: local_min;
      *max = (*max > local_max)? *max: local_max;
      break;
    case 3: 
      if(N < 50)
      {
        polynom_val = polynomial_value_0;
        residualMinMax(&local_min, &local_max, approx_0, local_scale);
        *min = local_min;
        *max = local_max;
      }
      polynom_val = polynomial_value_1;
      residualMinMax(&local_min, &local_max, approx_1, local_scale);
      *min = (*min < local_min)? *min: local_min;
      *max = (*max > local_max)? *max: local_max;
      break;
  }
}
void Window::paintEvent (QPaintEvent *)
{  
  QPainter painter (this);
  double max_y, min_y, delta_y, delta, local_a, local_b;
  double *approx_0;
  double *approx_1;
  double *x;
  double *func;
  double *func_der;

  local_a = (a + b)/2 - 0.5*(b - a)*scale;
  local_b = (a + b)/2 + 0.5*(b - a)*scale; 

  approx_0 = new double[4*N];
  approx_1 = new double[4*N];
  x = new double[N];
  func = new double[N];
  func_der = new double[N];

  memset(approx_0, 0, 4*N*sizeof(double));
  memset(approx_1, 0, 4*N*sizeof(double));
  for(int i = 0; i < N; i++) x[i] = point_value(N, i, a, b);
  for(int i = 0; i < N; i++) func[i] = f(x[i]);
  for(int i = 0; i < N; i++) func_der[i] = f_der(x[i]);

  if(APPROX_0 == true || RESIDUAL_0 == true) approximation_0(N, x, func, func_der, approx_0);
  if(APPROX_1 == true || RESIDUAL_1 == true) approximation_1(N, x, func, func_der, approx_1);

  QPen black_pen(Qt::black, 0, Qt::SolidLine); 
  QPen red_pen(Qt::red, 0, Qt::SolidLine); 
  QPen blue_pen(Qt::blue, 0, Qt::SolidLine);
  QPen green_pen(Qt::green, 0, Qt::SolidLine);
  QPen orange_pen(QColor("orange"), 0, Qt::SolidLine);

  findMinMax(approx_0, approx_1, &min_y, &max_y, scale);
  std::cout<<"f_min = "<<min_y<<std::endl;
  std::cout<<"f_max = "<<max_y<<std::endl;
  delta_y = 0.01*(max_y - min_y);
  if(max_y == min_y) delta = 1;
  else
  {
    min_y -= delta_y;
    max_y += delta_y;
    delta = max_y - min_y;
  }
  
  painter.save();
  painter.translate(0.5*width(), 0.5*height());
  painter.scale(width()/((local_b - local_a)), -height()/(delta));
  painter.translate(-0.5*(local_b + local_a), -0.5*(min_y + max_y));

  painter.setPen(black_pen);
  painter.drawLine(QPointF(local_a, 0), QPointF(local_b, 0));
  painter.drawLine(QPointF(0, min_y), QPointF(0, max_y));
  if(min_y == max_y) painter.drawLine(QPointF(0, height()), QPointF(0, -height()));

  if(FUNCTION == true)
  {
    painter.setPen(black_pen);
    drawFunction(&painter);
  }
  if(APPROX_0 == true && !(N > 50))
  {
    painter.setPen (blue_pen);
    polynom_val = polynomial_value_0;
    drawApproximation(&painter, approx_0);
  }
  if(APPROX_1 == true)
  {
    painter.setPen (red_pen);
    polynom_val = polynomial_value_1;
    drawApproximation(&painter, approx_1);
  }
  if(RESIDUAL_0 == true && !(N > 50))
  {
    painter.setPen(green_pen);
    polynom_val = polynomial_value_0;
    drawResidual(&painter, approx_0);
  }
  if(RESIDUAL_1 == true)
  {
    painter.setPen(orange_pen);
    polynom_val = polynomial_value_1;
    drawResidual(&painter, approx_1);
    
  }
  painter.restore();
  QString draw_N, draw_a_b, draw_min_max, draw_scale;
  draw_min_max.append("f_min = ");
  draw_min_max.append(QString::number(min_y + delta_y));
  draw_min_max.append(" f_max = ");
  draw_min_max.append(QString::number(max_y - delta_y));
  draw_scale.append("Scale = ");
  draw_scale.append(QString::number(scale));
  draw_a_b.append("a = ");
  draw_a_b.append(QString::number(a));
  draw_a_b.append(" b = ");
  draw_a_b.append(QString::number(b));
  draw_N.append("n = ");
  draw_N.append(QString::number(N));
  painter.setPen(black_pen);
  painter.drawText(10, 20, approximation_name);
  painter.setPen(blue_pen);
  painter.drawText(10, 40, function_name);
  painter.setPen(black_pen);
  painter.drawText(10, 60, draw_N);
  painter.drawText(10, 80, draw_a_b);
  painter.drawText(10, 100, draw_min_max);
  painter.drawText(10, 120, draw_scale);
  delete [] func;
  delete [] x;
  delete [] func_der;
  delete [] approx_0;
  delete [] approx_1;
}
