#include <math.h>
#include <stdio.h>
#include <omp.h>


const double PI = 3.14159265358979323846;
const double k = 2;
const double a = -k;
const double b = k;
const int nsteps = 80000000;


double func(double x) {
    return k * pow(x, k);
}

double integrate(double(*func)(double), double a, double b, int n) {
    const double width = (b - a) / n;
    double trapezoidal_integral = 0;
    for (int step = 0; step < n; step++) {
        const double x1 = a + step * width;
        const double x2 = a + (step + 1) * width;

        trapezoidal_integral += 0.5 * (x2 - x1) * (func(x1) + func(x2));
    }
    return trapezoidal_integral;
}

double integrate_omp(double(*func)(double), double a, double b, int n) {
    const double width = (b - a) / n;
    double trapezoidal_integral = 0;
#pragma omp parallel 
    {
        int nthreads = omp_get_num_threads();
        int threadid = omp_get_thread_num();
        int items_per_thread = n / nthreads;
        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (n - 1) : (lb + items_per_thread - 1);
        double trapezoidal_integral_step = 0.0;
        for (int step = lb; step <= ub; step++) {
            const double x1 = a + step * width;
            const double x2 = a + (step + 1) * width;
            trapezoidal_integral_step += 0.5 * (x2 - x1) * (func(x1) + func(x2));
        }
#pragma omp atomic
        trapezoidal_integral += trapezoidal_integral_step;
    }
    return trapezoidal_integral;
}

double run_serial() {
    double t = omp_get_wtime();
    double res = integrate(func, a, b, nsteps);
    t = omp_get_wtime() - t;
    printf("Result (serial): %.12f; error %.12f\n", res, fabs(res - 32/3));
    return t;
}

double run_parallel() {
    double t = omp_get_wtime();
    double res = integrate_omp(func, a, b, nsteps);
    t = omp_get_wtime() - t;
    printf("Result (parallel): %.12f; error %.12f\n", res, fabs(res - 32/3));
    return t;
}

int main(int argc, char** argv) {
    printf("Integration f(x) on [%.12f, %.12f], nsteps = %d\n", a, b, nsteps);
    double tserial = run_serial();
    double tparallel = run_parallel();
    printf("Execution time (serial): %.6f\n", tserial);
    printf("Execution time (parallel): %.6f\n", tparallel);
    printf("Speedup: %.2f\n", tserial / tparallel);
    return 0;
}