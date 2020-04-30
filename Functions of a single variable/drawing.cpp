#include "window.h"
#include "header.hpp"
#define EPS 1e-20

Window::Window (QWidget *parent) : QWidget (parent)
{
  N = 1;
  approx_0 = new double[4*N];
  approx_1 = new double[4*N];
  x = new double[N];
  func = new double[N];
  func_der = new double[N];
}

Window::~Window(){
    delete [] approx_0;
    delete [] approx_1;
    delete [] x;
    delete [] func;
    delete [] func_der;
}

QSize Window::minimumSizeHint() const
{
  return QSize(500, 500);
}

QSize Window::sizeHint() const
{
  return QSize(1000, 1000);
}

void Window::draw_function(QPainter *painter)
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

void Window::draw_approximation(QPainter *painter, double *approx)
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

void Window::draw_residual(QPainter *painter, double *approx)
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

void Window::paintEvent (QPaintEvent *)
{  
  QPainter painter (this);
  double max_y = 0, min_y = 0, delta_y, delta, local_a, local_b;
  local_a = (a + b)/2 - 0.5*(b - a)*scale;
  local_b = (a + b)/2 + 0.5*(b - a)*scale; 
  QPen black_pen(Qt::black, 0, Qt::SolidLine); 
  QPen red_pen(Qt::red, 0, Qt::SolidLine); 
  QPen blue_pen(Qt::blue, 0, Qt::SolidLine);
  QPen green_pen(Qt::green, 0, Qt::SolidLine);
  QPen orange_pen(QColor("orange"), 0, Qt::SolidLine);
  find_min_max(approx_0, approx_1, &min_y, &max_y, scale);
  std::cout<<"max_abs = "<<((fabs(min_y) > fabs(max_y))? fabs(min_y): fabs(max_y))<<std::endl;
  std::cout<<std::endl;
  delta_y = 0.01*(max_y - min_y);
  if(fabs(max_y - min_y) < EPS) delta = 1;
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
  if(fabs(max_y - min_y) < EPS) painter.drawLine(QPointF(0, height()), QPointF(0, -height()));

  if(FUNCTION == true)
  {
    painter.setPen(black_pen);
    draw_function(&painter);
  }
  if(APPROX_0 == true && !(N > 50))
  {
    painter.setPen (blue_pen);
    polynom_val = polynomial_value_0;
    draw_approximation(&painter, approx_0);
  }
  if(APPROX_1 == true)
  {
    painter.setPen (red_pen);
    polynom_val = polynomial_value_1;
    draw_approximation(&painter, approx_1);
  }
  if(RESIDUAL_0 == true && N < 50)
  {
    painter.setPen(green_pen);
    polynom_val = polynomial_value_0;
    draw_residual(&painter, approx_0);
  }
  if(RESIDUAL_1 == true)
  {
    painter.setPen(orange_pen);
    polynom_val = polynomial_value_1;
    draw_residual(&painter, approx_1);
    
  }
  painter.restore();
  
  painter.setPen(black_pen);
  
  QString draw_N, draw_a_b, draw_abs_max, draw_scale_p;

  min_y += delta_y;
  max_y -= delta_y;
  draw_abs_max.append("abs_max = ");
  draw_abs_max.append(QString::number((fabs(min_y) > fabs(max_y))? fabs(min_y): fabs(max_y)));
 
  
  draw_scale_p.append("Scale = ");
  draw_scale_p.append(QString::number(1/scale));
  draw_scale_p.append("    p = ");
  draw_scale_p.append(QString::number(p));
  
  draw_a_b.append("a = ");
  draw_a_b.append(QString::number(a));
  draw_a_b.append("    b = ");
  draw_a_b.append(QString::number(b));
  
  draw_N.append("n = ");
  draw_N.append(QString::number(N));
  
  painter.drawText(10, 20, approximation_name);
  painter.drawText(10, 40, function_name);
  painter.drawText(10, 60, draw_N);
  painter.drawText(10, 80, draw_a_b);
  painter.drawText(10, 100, draw_abs_max);
  painter.drawText(10, 120, draw_scale_p);
  
  painter.setPen(black_pen);
  painter.drawText(10, height() - 100, QString("Original function"));
  painter.setPen(blue_pen);
  painter.drawText(10, height() - 80, QString("I approximation"));
  painter.setPen(red_pen);
  painter.drawText(10, height() - 60, QString("II approximation"));
  painter.setPen(green_pen);
  painter.drawText(10, height() - 40, QString("I approximation residual"));
  painter.setPen(orange_pen);
  painter.drawText(10, height() - 20, QString("II approximation residual"));
}
