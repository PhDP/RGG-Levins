#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "well1024.h"
#include "utils.h"

// From Julienne Walker.
unsigned int time_seed()
{
    time_t now = time(0);
    unsigned char *p = (unsigned char*)&now;
    unsigned int seed = 0;

    // Knuth's method (TAOCP vol 2).
    for (size_t i = 0; i < sizeof(now); ++i)
    {
        seed = seed * (UCHAR_MAX + 2U) + p[i];
    }
    return seed;
}

void well1024_init(well1024 *rng, unsigned int seed)
{
    assert(seed != 0);
    rng->have_next_normal = FALSE;
    rng->seed = seed;
    rng->state_n = 0;
    rng->state[0] = rng->seed & 0xffffffffU;

    for (int i = 1; i < 32; ++i)
    {
        rng->state[i] = (69069 * rng->state[i - 1]) & 0xffffffffU;
    }
}

unsigned int well1024_init_devrandom(well1024 *rng)
{
    unsigned int seed = devrandom_get_uint();
    well1024_init(rng, seed);
    return seed;
}

unsigned int well1024_init_devurandom(well1024 *rng)
{
    unsigned int seed = devurandom_get_uint();
    well1024_init(rng, seed);
    return seed;
}

unsigned int well1024_init_time(well1024 *rng)
{
    unsigned int seed = 0;

    do
    {
        seed = time_seed();
    }
    while (seed == 0);

    well1024_init(rng, seed);

    return seed;
}

//////////////////////////////////////////
// CORE FUNCTION                        //
//////////////////////////////////////////

inline double well1024_next_double(well1024 *rng)
{
    unsigned int *const state = rng->state;
    const unsigned int state_n = rng->state_n;
    //const unsigned int z0 = state[(state_n + 31) & 0x0000001fUL];
    const unsigned int z1 = WELL_IDEN(state[state_n]) ^ WELL_MAT3POS(8, state[(state_n + 3) & 0x0000001fUL]);
    const unsigned int z2 = WELL_MAT3NEG(-19, state[(state_n+24) & 0x0000001fUL]) ^ WELL_MAT3NEG(-14, state[(state_n + 10) & 0x0000001fUL]);

    state[state_n] = z1 ^ z2;
    state[(state_n + 31) & 0x0000001fUL] = WELL_MAT3NEG(-11, (state[(state_n + 31) & 0x0000001fUL])) ^ WELL_MAT3NEG(-7, z1) ^ WELL_MAT3NEG(-13, z2);
    rng->state_n = (state_n + 31) & 0x0000001fUL;

    return ((double)state[rng->state_n] * 2.32830643653869628906e-10);
}

//////////////////////////////////////////
// UNIFORM DISTRIBUTION                 //
//////////////////////////////////////////

inline int well1024_next_int(well1024 *rng, int b)
{
    return (int)(well1024_next_double(rng) * b);
}

inline unsigned int well1024_next_uint(well1024 *rng, int b)
{
    return (unsigned int)(well1024_next_double(rng) * b);
}

inline int well1024_next_max_int(well1024 *rng)
{
    return (int)(well1024_next_double(rng) * INT_MAX);
}

inline unsigned int well1024_next_max_uint(well1024 *rng)
{
    return (int)(well1024_next_double(rng) * UINT_MAX);
}

//////////////////////////////////////////
// NON-UNIFORM DISTRIBUTIONS            //
//////////////////////////////////////////

inline double well1024_next_exp(well1024 *rng)
{
    double r = well1024_next_double(rng);
    while (r == 0.0)
    {
        r = well1024_next_double(rng);
    }
    return -log(r);
}

inline double well1024_next_normal(well1024 *rng)
{
    if (rng->have_next_normal)
    {
        rng->have_next_normal = FALSE;
        return rng->next_normal;
    }
    else
    {
        double n1, n2, s;
        do
        {
            // Two numbers in the -1.0, 1.0 range
            n1 = 2.0 * well1024_next_double(rng) - 1.0;
            n2 = 2.0 * well1024_next_double(rng) - 1.0;
            s = n1 * n1 + n2 * n2; // s = v1^2 + v2^2
        }
        while (s >= 1 || s == 0);

        const double m = sqrt(-2.0 * log(s) / s);
        rng->next_normal = n2 * m;
        rng->have_next_normal = TRUE;

        return n1 * m;
    }
}

inline int well1024_next_poisson(well1024 *rng, double lambda)
{
    const double l = pow(2.71828182845904523536, -lambda);
    double p = 1.0;
    int k = 0;

    do
    {
        ++k;
        p *= well1024_next_double(rng);
    }
    while (p > l);

    return k - 1;
}

//////////////////////////////////////////
// UTILS                                //
//////////////////////////////////////////

char* well1024_to_string(well1024 *rng)
{
    char* to_string = (char*)malloc(12 * 32);
    char buffer[12];

    for (int i = 0; i < 32; ++i)
    {
        sprintf(buffer,"%u ", rng->state[i]);
        strcat(to_string, buffer);
    }
    return to_string;
}
