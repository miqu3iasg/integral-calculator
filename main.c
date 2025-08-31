#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double function_x2(double x)
{
  return x * x;
}

double function_sin(double x)
{
  return sin(x);
}

double function_exp(double x)
{
  return exp(x);
}

double function_one_over_x(double x)
{
  if (x == 0.0)
  {
    fprintf(stderr, "Error: Division by zero in function_one_over_x\n");
    exit(EXIT_FAILURE);
  }
  return 1.0 / x;
}

double (*get_function(const char *name))(double)
{
  if (strcmp(name, "x2") == 0)
    return function_x2;

  if (strcmp(name, "sin") == 0)
    return function_sin;

  if (strcmp(name, "exp") == 0)
    return function_exp;

  if (strcmp(name, "1/x") == 0)
    return function_one_over_x;

  fprintf(stderr, "Error: Unknown function name '%s'\n", name);

  return function_x2;
}

double left_riemann_sum(double (*f)(double), double a, double b, int n)
{
  double dx = (b - a) / n;
  double sum = 0.0;
  for (int i = 0; i < n; i++)
  {
    double x = a + i * dx;
    sum += f(x) * dx;
  }
  return sum;
}

double right_riemann_sum(double (*f)(double), double a, double b, int n)
{
  double dx = (b - a) / n;
  double sum = 0.0;
  for (int i = 1; i <= n; i++)
  {
    double x = a + i * dx;
    sum += f(x) * dx;
  }
  return sum;
}

double midpoint_riemann_sum(double (*f)(double), double a, double b, int n)
{
  double dx = (b - a) / n;
  double sum = 0.0;
  for (int i = 1; i <= n; i++)
  {
    double x = a + (i + 0.5) * dx;
    sum += f(x) * dx;
  }
  return sum;
}

double compute_integral(
    const char *method,
    double (*f)(double),
    double a,
    double b,
    int n,
    double *error)
{

  if (strcmp(method, "left"))
    return left_riemann_sum(f, a, b, n);

  if (strcmp(method, "right"))
    return right_riemann_sum(f, a, b, n);

  if (strcmp(method, "midpoint"))
    return midpoint_riemann_sum(f, a, b, n);

  fprintf(stderr, "Error: Unknown method name '%s'\n", method);

  return right_riemann_sum(f, a, b, n);
}

void print_help()
{
  printf("Usage: ./integral [options]\n");
  printf("Options:\n");
  printf("  -m <method>   : left, right, midpoint (default: right)\n");
  printf("  -f <function> : x2, sin, exp, 1/x (default: x2)\n");
  printf("  -a <lower>    : Lower limit (default: prompt)\n");
  printf("  -b <upper>    : Upper limit (default: prompt)\n");
  printf("  -n <subints>  : Number of subintervals (default: prompt)\n");
  printf("  -h            : Show this help\n");
  printf("If no options, runs interactively.\n");
}

int main(int argc, char *argv[])
{
  char method[20] = "right";
  char function_name[20] = "x2";
  double a = 0.0, b = 0.0;
  int n = 0;
  int interactive = 1;

  for (int i = 1; i < argc; i += 2)
  {
    if (strcmp(argv[i], "-m") == 0)
      strcpy(method, argv[i + 1]);

    else if (strcmp(argv[i], "-f") == 0)
      strcpy(function_name, argv[i + 1]);

    else if (strcmp(argv[i], "-a") == 0)
      a = atof(argv[i + 1]);

    else if (strcmp(argv[i], "-b") == 0)
      b = atof(argv[i + 1]);

    else if (strcmp(argv[i], "-n") == 0)
      n = atoi(argv[i + 1]);

    else if (strcmp(argv[i], "-h") == 0)
    {
      print_help();
      return 0;
    }

    else
    {
      fprintf(stderr, "Unknown option: %s\n", argv[i]);
      print_help();
      return 1;
    }

    interactive = 0;
  }

  if (interactive || a == 0.0 || b == 0.0 || n == 0)
  {
    printf("Enter method (left, right, midpoint): ");
    scanf("%s", method);

    printf("Enter function (x2, sin, exp, 1/x): ");
    scanf("%s", function_name);

    printf("Enter lower limit a: ");
    scanf("%lf", &a);

    printf("Enter upper limit b: ");
    scanf("%lf", &b);

    printf("Enter number of subintervals n: ");
    scanf("%d", &n);
  }

  if (a >= b)
  {
    fprintf(stderr, "Error: a must be less than b.\n");
    return 1;
  }

  if (n <= 0)
  {
    fprintf(stderr, "Error: n must be positive.\n");
    return 1;
  }

  double (*f)(double) = get_function(function_name);
  double error = 0.0;
  double result = compute_integral(method, f, a, b, n, &error);

  printf("\n--- Integration Results ---\n");
  printf("| Method      | %s\n", method);
  printf("| Function    | %s\n", function_name);
  printf("| Interval    | [%.2f, %.2f]\n", a, b);
  printf("| Subintervals| %d\n", n);
  printf("| Result      | %.8f\n", result);
  printf("--------------------------\n");

  return 0;
}
