# Introdução

Decidi criar uma calculadora de integral em C para aprimorar meus conhecimentos em cálculo e melhorar a minha lógica de programação. O projeto é simples, mas o objetivo é entender de fato como funciona o conceito e abstrair isso para a camada de programação.

# Primeira parte: Definindo as funções básicas

Aqui eu decidi colocar as principais funções para serem calculadas. Como o projeto tem como objetivo exercitar apenas o conceito, eu decidi colocar somente as funções mais simples e deixar para o usuário definir os limites de integração e a quantidade de subintervalos.

```c
// Função x^2
double function_x2(double x)
{
  return x * x;
}

// Função seno
double function_sin(double x)
{
  return sin(x);
}

// Função exponencial
double function_exp(double x)
{
  return exp(x);
}

// Função 1/x (lnx)
double function_one_over_x(double x)
{
  if (x == 0.0)
  {
    fprintf(stderr, "Error: Division by zero in function_one_over_x\n");
    exit(EXIT_FAILURE);
  }
  return 1.0 / x;
}
```

# Segunda parte: Definindo qual função será usada

Aqui eu usei um método que retorna a função de acordo com o nome passado pelo usuário. Ele recebe um ponteiro que é uma string, e retorna um dobro do double, isto é, ele a ele chama uma função que retorna um double e essa função retorna outro double (valor da função a ser calculada).

```c
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
```

# Terceira parte: Soma de Riemann

A soma de Riemann pode ser feita pegando diversos valores de x_i em qualquer ponto de subintervalos de uma partição P. Ou seja, podemos pegar qualquer valores, tanto pela direita, quanto pela esquerda. A ideia é dividir o intervalo em n subintervalos e somar áreas de retângulos que representam a função em pontos específicos de cada subintervalo.

## Soma de Riemann pela esquerda

Se escolhe o ponto mais a direita do subintervalo. Em funções positivas, isso culmina em valores aproximados menores do que a área sob a curva real do do gráfico. 

Imagine uma curva crescente. Pegando o valor da função na esquerda do intervalo, cada retângulo ficará abaixo da curva, resultando em uma subestimação.

**Fórmula:**

$$
Sesq=∑i=0n−1f(a+iΔx)⋅Δx
S
esq
	​

=
i=0
∑
n−1
	​

f(a+iΔx)⋅Δx
$$

```c
double left_riemann_sum(double (*f)(double), double a, double b, int n)
{
// Calculando a largura de cada retângulo (dx)
  double dx = (b - a) / n;

// Inicializando a variável soma
  double sum = 0.0;
  
// Percorrendo cada retângulo de acordo com o número de subintervalos n
  for (int i = 0; i < n; i++)
  {
	// Definindo o valor de x, que vai ser sempre o primeiro limite de integração 
	// mais o index (próximo retângulo) mais a largura do retângulo. (e.g.: 1dx, 2dx, 3dx...)
    double x = a + i * dx;
		
	// Calculando a área e adicionando na variável soma
    sum += f(x) * dx;
  }
  return sum;
}
```

## Soma de Riemann pela direita

Escolhe-se **o ponto mais à direita de cada subintervalo**: $xi∗=xix_i^* = x_ixi∗​=xi​$. Ela também pode superestimar ou subestimar o valor da integral dependendo da função.

**Fórmula:**

$$
Sdir=∑i=1nf(a+iΔx)⋅ΔxS_{\text{dir}} = \sum_{i=1}^{n} f(a + i \Delta x) \cdot \Delta xSdir​=i=1∑n​f(a+iΔx)⋅Δx
$$


Para a mesma função crescente, os retângulos ficarão **acima** da curva, gerando uma **superestimação**.

```c
double right_riemann_sum(double (*f)(double), double a, double b, int n)
{
  double dx = (b - a) / n;
  double sum = 0.0;
  // Aqui se começa do índice 1 e vai até o n, isso significa pegar os valores à direita.
  for (int i = 1; i <= n; i++)
  {
    double x = a + i * dx;
    sum += f(x) * dx;
  }
  return sum;
}
```

## Soma de Riemann pelo ponto médio

Escolhe-se **o ponto central de cada subintervalo**. Normalmente dá uma aproximação mais **precisa** do que as somas pela esquerda ou direita. Minimiza os erros de subestimação e superestimação, pois o ponto médio tende a equilibrar a diferença entre retângulo e área real da curva.

**Fórmula:**
$$
Smid=∑i=1nf(a+(i−12)Δx)⋅ΔxS_{\text{mid}} = \sum_{i=1}^{n} f\Big(a + \big(i - \frac{1}{2}\big)\Delta x\Big) \cdot \Delta xSmid​=i=1∑n​f(a+(i−21​)Δx)⋅Δx
$$
Para funções suaves, o retângulo central tende a representar a área real com mais fidelidade. A fórmula parece bem complicada, mas na prática basta calcular o valor da base de cada retângulo e somar meio, isso é, dividir por dois.

```c
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
```

O restante do código é somente para melhorar a experiência do usuário, a lógica base é essa.

## Source Code

```c
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
```
