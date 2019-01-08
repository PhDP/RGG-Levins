#ifndef WELL1024_H_
#define WELL1024_H_

#define WELL_MAT3POS(t,v) (v^(v>>t))
#define WELL_MAT3NEG(t,v) (v^(v<<(-t)))
#define WELL_IDEN(v) (v)

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

// For C++ compilers:
#ifdef __cplusplus
extern "C" {
#endif

/** The state of a well1024 random number generator. */
typedef struct
{
    unsigned int state[32];
    unsigned int state_n;
    unsigned int seed; // The initial seed used to initiate the generator
    // For non-uniform random numbers;
    int have_next_normal;
    double next_normal;
}
well1024;

// SEEDING

/** Julienne Walker's function (from www.eternallyconfuzzled.com) to get an unsigned int from time(). */
unsigned int time_seed();

/** Seed the generator with an unsigned integer. */
void well1024_init(well1024 *rng, unsigned int seed);

/** Initialize with true random numbers from /dev/random. It works on Linux, Solaris, FreeBSD, Mac OS X, and many other OS, but not MS Windows. Return the seed used. */
unsigned int well1024_init_devrandom(well1024 *rng);

/** Initialize with true random numbers from /dev/urandom. It works on Linux, Solaris, FreeBSD, Mac OS X, and many other OS, but not MS Windows. Return the seed used. */
unsigned int well1024_init_devurandom(well1024 *rng);

/** Initialize with time using time_seed(). Return the seed used. */
unsigned int well1024_init_time(well1024 *rng);

// CORE FUNCTION

/** Return a double in the [0, 1) range. */
double well1024_next_double(well1024 *rng);

// UNIFORM DISTRIBUTION

/** Return an integer in the [0, b) semi-closed range. */
int well1024_next_int(well1024 *rng, int b);

/** Return an unsigned integer in the [0, b) semi-closed range. */
unsigned int well1024_next_uint(well1024 *rng, int b);

/** Return an interger. */
int well1024_next_max_int(well1024 *rng);

/** Number an unsigned interger. */
unsigned int well1024_next_max_uint(well1024 *rng);

// NON-UNIFORM DISTRIBUTIONS

/** Return a double following the exponential distribution (between 0 and 1). */
double well1024_next_exp(well1024 *rng);

/** Return a double following the normal distribution (between 0 and 1). */
double well1024_next_normal(well1024 *rng);

/** Return an integer from a poisson distribution given lambda. \f$O(\lambda)\f$. */
int well1024_next_poisson(well1024 *rng, double lambda);

// UTILS

/** Return the state of the generator as a string. */
char *well1024_to_string(well1024 *rng);

// End the extern for C++ compilers:
#ifdef __cplusplus
}
#endif

#endif
