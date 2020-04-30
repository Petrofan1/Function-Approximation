#include "window.h"
#include "header.hpp"

int Window::command_line(int argc, char *argv[])
{
  if (   argc < 5
      || sscanf (argv[1], "%lf", &a) != 1
      || sscanf (argv[2], "%lf", &b) != 1
      || b - a < 1.e-6
      || sscanf (argv[3], "%d", &N) != 1
      || N < 3
      || sscanf(argv[4], "%d", &k) != 1
      || (k < 0 || k > NUMBER_OF_FUNCTIONS - 1)) 
  {
    std:: cout<<"\nUsage: a b n k\nn > 2\nk = 0...6"<<std::endl;
    return -2;
  }
  choose_function();
  choose_approximation();
  set_function_array();
  calculate_approximation();
  update();
  return 0;
}

void Window::reduce_n()
{
  N = (N/2 <= 2)? 3: N/2;
  set_function_array();
  calculate_approximation();
  update();
}

void Window::increase_n()
{
  N *= 2;
  set_function_array();
  calculate_approximation();
  update();
}


void Window::decrease_scale()
{
  scale *= 0.5;
  update();
}

void Window::increase_scale()
{
  scale = (scale*2 > 1)? 1: scale*2;
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
  calculate_approximation();
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
  set_function_array();
  calculate_approximation(); 
  update();
}

void Window::add_delta()
{
  p += 1;
  func[N/2] += 0.1*max_abs_value; 
  calculate_approximation();
  update();
}

void Window::subtract_delta()
{
  p -= 1;   
  func[N/2] -= 0.1*max_abs_value; 
  calculate_approximation();
  update();
}