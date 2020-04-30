#include "window.h"
#include "header.hpp"

void Window::function_min_max(double *min, double *max, double scale)
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
}

void Window::calculate_approximation()
{
    if((APPROX_0 == true || RESIDUAL_0 == true) && N <= 50) 
    {
      delete [] approx_0;
      approx_0 = new double[4*N];
      memset(approx_0, 0, 4*N*sizeof(double));
      approximation_0(N, x, func, func_der, approx_0);
    }
    if(APPROX_1 == true || RESIDUAL_1 == true) 
    {
      delete [] approx_1;
      approx_1 = new double[4*N];
      memset(approx_1, 0, 4*N*sizeof(double));
      approximation_1(N, x, func, func_der, approx_1);
    }
}
void Window::find_abs_max()
{
  double step = (b - a)/(width() - 1);
  max_abs_value = fabs(a);
  for (double x = a; x - b <= 0; x += step)
  {
    max_abs_value = (max_abs_value > fabs(f(x)))? max_abs_value: fabs(f(x));
  }
}
void Window::set_function_array()
{
  double delta = (b - a)/(N - 1);
  delete [] func;
  delete [] x;
  delete [] func_der;
  x = new double[N];
  func = new double[N];
  func_der = new double[N];
  for(int i = 0; i < N; i++) x[i] = a + i*delta;
  for(int i = 0; i < N; i++) func[i] = f(x[i]);
  for(int i = 0; i < N; i++) func_der[i] = f_der(x[i]);
  find_abs_max();
  func[N/2] += p*0.1*max_abs_value;
}

void Window::approximation_min_max(double *min, double *max, double *approx, double scale)
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

void Window::residual_min_max(double *min, double *max, double *approx, double scale)
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

void Window::find_min_max(double *approx_0, double *approx_1, double *min, double *max, double scale)
{
  double local_min = 0, local_max = 0, local_scale = (scale > 1)? 1: scale;
  switch(approximation_id)
  {
    case 0:
      function_min_max(&local_min, &local_max, local_scale);
      *min = local_min;
      *max = local_max;
      polynom_val = polynomial_value_0;
      if(N < 50)
      {
        approximation_min_max(&local_min, &local_max, approx_0, local_scale);
        *min = (*min < local_min)? *min: local_min;
        *max = (*max > local_max)? *max: local_max;
      }
      break;
    case 1:
      function_min_max(&local_min, &local_max, local_scale);
      *min = local_min;
      *max = local_max;
      if(N < 50)
      {
        polynom_val = polynomial_value_1;
        approximation_min_max(&local_min, &local_max, approx_1, local_scale);
        *min = (*min < local_min)? *min: local_min;
        *max = (*max > local_max)? *max: local_max;
      }
      break;
    case 2:
      function_min_max(&local_min, &local_max, local_scale);
      *min = local_min;
      *max = local_max;
      if(N < 50)
      {
        polynom_val = polynomial_value_0;
        approximation_min_max(&local_min, &local_max, approx_0, local_scale);
        *min = (*min < local_min)? *min: local_min;
        *max = (*max > local_max)? *max: local_max;
      }
      polynom_val = polynomial_value_1;
      approximation_min_max(&local_min, &local_max, approx_1, local_scale);
      *min = (*min < local_min)? *min: local_min;
      *max = (*max > local_max)? *max: local_max;
      break;
    case 3: 
      if(N < 50)
      {
        polynom_val = polynomial_value_0;
        residual_min_max(&local_min, &local_max, approx_0, local_scale);
        *min = local_min;
        *max = local_max;
      }
      polynom_val = polynomial_value_1;
      residual_min_max(&local_min, &local_max, approx_1, local_scale);
      *min = (*min < local_min)? *min: local_min;
      *max = (*max > local_max)? *max: local_max;
      break;
  }
}