#ifndef STATISTICS_H_
#define STATISTICS_H_

// For C++ compilers:
#ifdef __cplusplus
extern "C" {
#endif

/** Naive 'algorithm' to sum the elements of the array. O(n) time and O(1) space but vulnerable to catastrophic cancellation. */
double dnaivesum(const double *x, int length);

/** Sort the array before summation (reduce the error). O(n log n) time and takes an extra O(n) memory. */
double dsortsum(const double *x, int length);

/** Sum the elements of the array. */
int isum(const int *x, int length);

/** Sum the elements of the array. */
long lsum(const long *x, int length);

/** Return the mean. O(n) time and O(1) space but vulnerable to catastrophic cancellation. */
double dnaivemean(const double *x, int length);

/** Relies on sortsum to get the mean. */
double dmean(const double *x, int length);

/** Return the mean. */
double imean(const int *x, int length);

/** Return the mean. */
double lmean(const long *x, int length);

/** Return the variance & the mean (based on Donald Knuth's TAOCP vol 2). */
void dvarmean(const double *x, int length, double *mean, double *var);

/** Return the variance (based on Donald Knuth's TAOCP vol 2). */
double dvar(const double *x, int length);

/** Timothy B. Terriberry's online algorithm to compute kutorsis. */
double dkutorsis(const double *x, int length);

/** Return the median. Unlike GSL's function, this function works on unsorted arrays. It will copy the array, sort it, compute the median, and free the memory of the copy before returning the median. */
double dmedian(const double *x, int length);

/** Return the median. Unlike GSL's function, this function works on unsorted arrays. It will copy the array, sort it, compute the median, and free the memory of the copy before returning the median. */
double imedian(const int *x, int length);

#ifdef __cplusplus
}
#endif

#endif
