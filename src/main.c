// Levins model on a spatial network
// Philippe Desjardins-Proulx - 2012.04.15
// MIT license

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "well1024.h"
#include "graph.h"
#include "utils.h"
#include "time.h"

#define PRESENT 1
#define ABSENT  0

#ifndef E
#define E           0.4
#endif
#ifndef C
#define C           0.2
#endif
#ifndef R
#define R           0.08
#endif
#ifndef N
#define N           1024
#endif
#ifndef T
#define T           2048
#endif

// Prototype for the Levins model:
void levins(bool *occ0, bool *occ1, unsigned int *incidence, graph *g, well1024 *rng);
// Compute the occupancy:
double occupancy(bool *occ);

int main(int argc, const char *argv[])
{
    const unsigned int seed = devurandom_get_uint();

    // Random number generator:
    well1024 rng;
    well1024_init(&rng, seed);

    // (x, y) coordinates for the vertices:
    double *x = (double*)malloc(N * sizeof(double));
    double *y = (double*)malloc(N * sizeof(double));
    
    // To keep track of the presence/absence of the population:
    bool *restrict occ_a = (bool*)malloc(N * sizeof(bool));
    bool *restrict occ_b = (bool*)malloc(N * sizeof(bool));

    unsigned int *incidence = (unsigned int*)calloc(N, sizeof(unsigned int));

    for (int n = 0; n < N; ++n)
    {
#ifdef INIT_OCC
        occ_a[n] = (well1024_next_double(&rng) < INIT_OCC)? PRESENT : ABSENT;
#else
        occ_a[n] = PRESENT;
#endif
    }

    // Generate a loopless random geometric graph:
    graph g;
    graph_get_llrgg(&g, N, R, x, y, &rng);
    
    // The simulation:
    for (int t = 0; t < T; t += 2)
    {
        levins(occ_a, occ_b, incidence, &g, &rng);
        levins(occ_b, occ_a, incidence, &g, &rng);
    }
    
    char buffer[50];
    sprintf(buffer, "levins-%u.graphml", seed);

    FILE *outgraph = fopen(buffer, "w");
    graph_graphml(&g, outgraph, seed);
    fclose(outgraph);

    sprintf(buffer, "levins-%u.xml", seed);
    FILE *out = fopen(buffer, "w");
    fprintf(out, "<xml>\n  <e>%.4f</e>\n  <c>%.4f</c>\n  <n>%u</n>\n  <seed>%u</seed>\n  <occupancy>%.4f</occupancy>\n", E, C, N, seed, occupancy(occ_a));
    fprintf(out, "  <incidence>");
    for (int n = 0; n < N - 1; ++n)
    {
        fprintf(out, "%u, ", incidence[n]);
    }
    fprintf(out, "%u</incidence>\n</xml>", incidence[N-1]);
    fclose(out);

    double *dincidence = (double*)malloc(N * sizeof(double));
    for (int n = 0; n < N; ++n)
    {
        dincidence[n] = ((double)incidence[n]) / T;
    }
    scale_0_1(dincidence, N);

    sprintf(buffer, "levins-%u.svg", seed);
    FILE *outsvg = fopen(buffer, "w");
    graph_svg_abun(&g, x, y, 800.0, 20.0, dincidence, outsvg);
    fclose(outsvg);

    return EXIT_SUCCESS;
}

void levins(bool *occ0, bool *occ1, unsigned int *incidence, graph *g, well1024 *rng)
{
    for (int n = 0; n < N; ++n)
    {
        if (occ0[n])
        {
            occ1[n] = (well1024_next_double(rng) < E)? ABSENT : PRESENT;
        }
        else
        {
            unsigned int x = 0;
            for (int v = 0; v < g->num_e[n]; ++v)
            {
                x += (occ0[g->adj_list[n][v]] == PRESENT);
            }
            const double p = 1.0 - pow(1.0 - C, x);
            occ1[n] = (well1024_next_double(rng) < p)? PRESENT : ABSENT;
        }
        if (occ1[n])
        {
            ++incidence[n];
        }
    }
}

double occupancy(bool *occ)
{
    unsigned int sum = 0;
    for (int i = 0; i < N; ++i)
    {
        sum += (occ[i] == PRESENT);
    }
    return ((double)sum) / N;
}
