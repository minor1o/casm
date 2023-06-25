#include <stdio.h>                                      // standart
#include <stdlib.h>                                     // strtod
#include <math.h>                                       // fabs
#include <getopt.h>                                     // input long operators

typedef double afunc(double);


extern double f1(double);

extern double df1(double);

extern double f2(double);

extern double df2(double);

extern double f3(double);

extern double df3(double);

static double f4(double x) {
    return 0.6 * x + 3;
}

static double df4(double x) {
    return 0.6;
}

static double f5(double x) {
    return pow((x - 2), 3) - 1;
}

static double df5(double x) {
    return 3 * (pow(x - 2, 2));
}

static double f6(double x) {
    return 3 / x;
}

static double df6(double x) {
    return -3 / pow(x, 2);
}

static afunc *map_func(int fn) {
    switch (fn) {
        case 1:
            return &f1;
        case 2:
            return &f2;
        case 3:
            return &f3;
        case 4:
            return &f4;
        case 5:
            return &f5;
        default:
            return &f6;
    }
}

static afunc *map_derivative(int fn) {
    switch (fn) {
        case 1:
            return &df1;
        case 2:
            return &df2;
        case 3:
            return &df3;
        case 4:
            return &df4;
        case 5:
            return &df5;
        default:
            return &df6;
    }
}

static int iterations = 0;

static double root(afunc *f, afunc *g, afunc *df, afunc *dg,
                   double a, double b, double eps) {
    double Fa = f(a) - g(a),
            Fb = f(b) - g(b),                                                   // Fa=f(a)-g(a), F(b)=f(b)-g(b)
    Fm = (f((a + b) / 2) - g((a + b) / 2));                                     // Fm=f([a+b]/2)-g([a+b]/2)

    int c1 = Fa < eps, c2 = Fa > eps,
            c3 = Fm - (Fa + Fb) / 2 > eps,
            c4 = Fm - (Fa + Fb) / 2 < eps;
    int a_flag = (c1 && c3) || (c2 && c4);
    // f'(x)f''(x)<0

    double an = a, bn = b;
    while (b - a >= eps) {
        double Fbn = f(bn) - g(bn),
                Fan = f(an) - g(an),
                dFan = df(an) - dg(an),
                dFbn = df(bn) - dg(bn);
        a = an - ((a_flag) ?
                  Fan / dFan :
                  Fan * (bn - an) / (Fbn - Fan));
        b = bn - ((a_flag) ?
                  Fbn * (bn - an) / (Fbn - Fan) :
                  Fbn / dFbn);
        an = a;
        bn = b;
        ++iterations;
    }
    return a;
}

static double integral(afunc *f, double a, double b, double eps2) {
    double n = 10;
    double h = (b - a) / n;
    double Io = (f(a) + f(b)) / 2;
    for (int i = 1; i < n; ++i) {
        Io += f(a + i * h);
    }
    Io *= h;

    n *= 2;
    h /= 2;
    double In = Io / 2, Ia = .0;
    for (int i = 1; i < n; i += 2) {
        Ia += f(a + i * h);
    }
    In += Ia * h;
    Ia = 0;
    n *= 2;
    h /= 2;
    while (fabs(Io - In) / 3 >= eps2) {
        In /= 2;
        for (int i = 1; i < n; i += 2) {
            Ia += f(a + i * h);
        }
        In += Ia * h;
        Ia = 0;
        n *= 2;
        h /= 2;
        Io = In;
    }
    return In;
}

int main(int argc, char **argv) {
    double eps1 = 0.0001;
    double eps2 = 0.0001;

    const char *short_options = "hriR:I:";

    const struct option long_options[] = {
            {"help",          no_argument,       NULL, 'h'},
            {"root",          no_argument,       NULL, 'r'},
            {"iterations",    no_argument,       NULL, 'i'},
            {"test-root",     required_argument, NULL, 'R'},
            {"test-integral", required_argument, NULL, 'I'},
            {NULL, 0,                            NULL, 0}
    };

    int rez;
    int option_index = -1;

    if (argc == 1) {
        double root1 = root(f1, f3, df1, df3, 2.02, 2.3, eps1);
        double root2 = root(f2, f3, df2, df3, -10, 10, eps1);
        double root3 = root(f1, f2, df1, df2, 3.75, 4.5, eps1);

        double s = integral(f3, root1, root2, eps2) +
                   integral(f2, root2, root3, eps2) -
                   integral(f1, root1, root3, eps2);

        printf("%lf\n", s);
    }

    while ((rez = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        switch (rez) {
            case 'h': {
                printf("%-20s %60s", "-h, --help", "Display this information.\n");
                printf("%-20s %60s", "-i, --iterations", "Display count of iterations in root function.\n");
                printf("%-20s %60s", "-r, --root", "Display roots of equation.\n");
                printf("%-20s %60s", "-I, --test-integral", "Test integral function.\n");
                printf("%-20s %60s", "-R, --test-root", "Test root function.\n");
                break;
            }
            case 'r': {
                double root1 = root(f1, f3, df1, df3, 2.02, 2.3, eps1);
                double root2 = root(f2, f3, df2, df3, 3.5, 5.5, eps1);
                double root3 = root(f1, f2, df1, df2, 5.5, 6.5, eps1);
                printf("Functions intersection points: %lf %lf %lf\n", root1, root2, root3);
                break;
            }
            case 'i': {
                root(f1, f3, df1, df3, 2.02, 2.3, eps1);
                root(f2, f3, df2, df3, 3.5, 5.5, eps1);
                root(f1, f2, df1, df2, 5.5, 6.5, eps1);
                printf("%d iterations\n", iterations);
                break;
            }
            case 'R': {
                int fun1 = optarg[0] - '0';
                int fun2 = optarg[2] - '0';
                char *str = optarg + 4;

                double a = strtod(str, &str);
                ++str;
                double b = strtod(str, &str);
                ++str;
                double c = strtod(str, &str);
                ++str;
                double r = strtod(str, &str);

                afunc *f = map_func(fun1);
                afunc *df = map_derivative(fun1);

                afunc *g = map_func(fun2);
                afunc *dg = map_derivative(fun2);

                double ans = root(f, g, df, dg, a, b, c);

                printf("%lf %lf %lf\n", ans, r - ans, fabs((r - ans) / r));
                break;
            }
            case 'I': {
                int fun = optarg[0] - '0';
                char *str = optarg + 2;

                double a = strtod(str, &str);
                ++str;
                double b = strtod(str, &str);
                ++str;
                double e = strtod(str, &str);
                ++str;
                double r = strtod(str, &str);

                afunc *f = map_func(fun);

                double ans = integral(f, a, b, e);
                printf("%lf %lf %lf\n", ans, r - ans, fabs((r - ans) / r));
                break;
            }
            default: {
                printf("error: unrecognized command-line option\n");
            }
        }
        option_index = -1;
    }
    return 0;
}