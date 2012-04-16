#ifndef UTILS_H_
#define UTILS_H_

#ifndef UTILS_SEPARATOR
#define UTILS_SEPARATOR ' '
#endif

#include <stdbool.h>

////////////////////////////////////////////////////////////////////
// Read options supplied by the user

/** Return the substring in a string of the form [something]=[substring]. */
char *get_opt_s(const char *arg);

/** Return the int in a string of the form [something]=[int]. */
int get_opt_i(const char *arg);

/** Return the double in a string of the form [something]=[double]. */
double get_opt_d(const char *arg);

/**
* Send an array of strings and the name of an option. The function will
* look for an string of the form -OPTION=X and send X to param. e.g.:
* <br>
* <br>int i;
* <br>read_opt_i("x", argv, 5, &i);
* <br>
* <br>will read the 5 strings in argv, seach for a string of the form 
* "-x=A" and copy A to i.
*/
bool read_opt_i(const char *option, const char *argv[], int n, int *param);

/**
* Send an array of strings and the name of an option. The function will
* look for an string of the form -OPTION=X and send X to param. e.g.:
* <br>
* <br>double s;
* <br>read_opt_d("selection", argv, 5, &s);
* <br>
* <br>will read the 5 strings in argv, seach for a string of the form 
* "-selection=A" and copy A to s.
*/
bool read_opt_d(const char *option, const char *argv[], int n, double *param);

/**
* Send an array of strings and the name of an option. The function will
* look for an string with the form -OPTION=X and send X to param. e.g.:
* <br>
* <br>char *s;
* <br>read_opt_i("x", argv, 5, s);
* <br>
* <br>Will read the 5 strings in argv, seach for a string of the form 
* "-x=A" and copy A to s.
*/
bool read_opt_s(const char *option, const char *argv[], int n, char *param);

////////////////////////////////////////////////////////////////////
// Formatting

/** Transform a number of seconds into a more detailed string (days, hours, minutes, seconds). */
char *sec_to_string(long seconds);

/** Transform an integer to a string with spaces (e.g.: 19939200 => "19 939 200"). */
char *int_with_space(int integer);

/** Transform a long to a string with spaces (e.g.: 72036854775808 => "72 036 854 775 808"). */
char *long_with_space(long integer);

/** Convert the string's uppercase characters to lowercase. */
void convert_to_lower_case(char *str);

/** Convert the string's lowercase characters to uppercase. */
void convert_to_upper_case(char *str);

/** Return a copy of the string with upper case characters converted to lower case. */
char *get_lower_case(const char *str);

/** Return a copy of the string with lower case characters converted to upper case. */
char *get_upper_case(const char *str);

////////////////////////////////////////////////////////////////////
// Operations on matrices and arrays

/**
* Allocates the memory for a rectangular matrix. e.g.:
* <br>
* <br>int **x = (int**)mat_alloc(800, 100, sizeof(int)); // a 800 by 100 matrix of int.
* <br>char **y = (char**)mat_alloc(50, 50, 1); // A 50 by 50 matrix of char.
* <br>double **z = (double**)mat_alloc(1000, 1000, sizeof(double)); // A 1000 by 1000 matrix of double.
*/
void **mat_alloc(int nrows, int ncols, size_t size);

/**
* Free the memory of a matrix, e.g.:
* <br>
* <br>double **x = (double**)mat_alloc(1000, 500, sizeof(double));
* <br>// ...
* <br>mat_free((void***)(&x), 1000);
*/
void mat_free(void **mat, int nrows);

////////////////////////////////////////////////////////////////////
// Simple

/** Find the min and the max values in an array of double. */
void dmin_max(double *x, int length, double *min, double *max);

/** Find the min and the max values in an array of int. */
void imin_max(int *x, int length, int *min, int *max);

/** Scale all the values in the [0, 1] range. */
void scale_0_1(double *x, int length);

/** Scale all the values in the [0, 100] range. */
void scale_0_100(double *x, int length);

/** Scale all the values in the [a, b] range. */
void scale_a_b(double *x, int length, double a, double b);

/** Remove from the array all the elements smaller than "smallest". The function will reallocate memory & return the number of elements removed. */
int trim_small(int **x, int elements, int smallest);

/** Remove from the array all the elements larger than "largest". The function will reallocate memory & return the number of elements removed. */
int trim_large(int **x, int elements, int largest);

////////////////////////////////////////////////////////////////////
// Other functions

/** Get a non-zero unsigned integer from dev/random. */
unsigned int devrandom_get_uint();

/** Get a non-zero unsigned integer from dev/urandom. */
unsigned int devurandom_get_uint();

#endif
