#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include "stats.h"
#include "utils.h"

inline double dnaivesum(const double *x, int length)
{
    double sum = 0.0;

    for (int i = 0; i < length; ++i)
    {
        sum += x[i];
    }
    return sum;
}

inline double dsortsum(const double *x, int length)
{
    // Copy the array & sort it with quicksort.
    double *sorted = (double*)malloc(length * sizeof(double));
    memcpy((void*)sorted, (void*)x, length * sizeof(double));
    qsort((void*)sorted, length, sizeof(double), compare_double_asc);

    const double sum = dnaivesum(sorted, length); // The actual answer
    free(sorted); // Free memory

    return sum;
}

// So simple with integers...
inline int isum(const int *x, int length)
{
    int sum = 0;

    for (int i = 0; i < length; ++i)
    {
        sum += x[i];
    }
    return sum;
}

inline long lsum(const long *x, int length)
{
    long sum = 0;

    for (int i = 0; i < length; ++i)
    {
        sum += x[i];
    }
    return sum;
}

double dnaivemean(const double *x, int length)
{
    return dnaivesum(x, length) / length;
}

double dmean(const double *x, int length)
{
    return dsortsum(x, length) / length;
}

double imean(const int *x, int length)
{
    return ((double)isum(x, length)) / length;
}

double lmean(const long *x, int length)
{
    return ((double)lsum(x, length)) / length;
}

void dvarmean(const double *x, int length, double *mean, double *var)
{
    double m = 0.0;
    double m2 = 0.0;
    double delta;
 
    for (int i = 0; i < length; ++i)
    {
        delta = x[i] - m;
        m += delta / (i + 1);
        m2 += delta * (x[i] - m);
    }
    (*mean) = m;
    (*var) =  m2 / (length - 1);
}

double dvar(const double *x, int length)
{
    double m = 0.0;
    double m2 = 0.0;
    double delta;
 
    for (int i = 0; i < length; ++i)
    {
        delta = x[i] - m;
        m += delta / (i + 1);
        m2 += delta * (x[i] - m);
    } 
    return m2 / (length - 1);
}

double dkutorsis(const double *x, int length)
{
    double m1 = 0.0, m2 = 0.0, m3 = 0.0, m4 = 0.0;
    double i1, term1, delta, delta_n, delta_n2;
 
    for (int i = 0; i < length; ++i)
    {
        i1 = (double)(i + 1);
        delta = x[i] - m1;
        delta_n = delta / i1;
        delta_n2 = delta_n * delta_n;
        term1 = delta * delta_n * i;
        m1 += delta_n;
        m4 += term1 * delta_n2 * (i1 * i1 - 3 * i1 + 3) + 6 * delta_n2 * m2 - 4 * delta_n * m3;
        m3 += term1 * delta_n * (i1 - 2) - 3 * delta_n * m2;
        m2 += term1;
    }
    return (length * m4) / (m2 * m2) - 3;
}

double dmedian(const double *x, int length)
{
    if (length == 0)
    {
        return 0;
    }
    else if (length == 1)
    {
        return x[0];
    }
    double *sorted = (double*)malloc(length * sizeof(double));
    memcpy((void*)sorted, (void*)x, length * sizeof(double));
    qsort((void*)sorted, length, sizeof(double), compare_double_asc);

    const int m = length / 2;
    const double answer = (length % 2 == 1) ? sorted[m] : (sorted[m - 1] + sorted[m]) / 2.0;

    free(sorted);
    return answer;
}

// Better algorithm ?
double imedian(const int *x, int length)
{
    if (length == 0)
    {
        return 0;
    }
    else if (length == 1)
    {
        return x[0];
    }
    int *sorted = (int*)malloc(length * sizeof(int));
    memcpy((void*)sorted, (void*)x, length * sizeof(int));
    qsort((void*)sorted, length, sizeof(int), compare_int_asc);

    const int m = length / 2;
    const double answer = (length % 2 == 1) ? sorted[m] : (sorted[m - 1] + sorted[m]) / 2.0;

    free(sorted);
    return answer;
}
