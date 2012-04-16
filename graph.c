#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <float.h>
#include "well1024.h"
#include "graph.h"
#include "utils.h"

void graph_init(graph *g, int vertices)
{
    g->num_v = vertices;
    g->num_e = (int*)malloc(vertices * sizeof(int));
    g->capacity = (int*)malloc(vertices * sizeof(int));
    g->adj_list = (int**)malloc(vertices * sizeof(int*));
    g->w_list = (double**)malloc(vertices * sizeof(double*));

    int i = 0;
    for (; i < vertices; ++i) 
    {
        g->num_e[i] = 0;
        g->capacity[i] = GRAPH_INIT_CAPACITY;
        g->adj_list[i] = (int*)malloc(GRAPH_INIT_CAPACITY * sizeof(int));
        g->w_list[i] = (double*)malloc(GRAPH_INIT_CAPACITY * sizeof(double));
    }
}

inline int graph_edges(const graph *g)
{
    const int num_v = g->num_v;
    int sum = 0;
    int v = 0;
    for (; v < num_v; ++v) 
    {
        sum += g->num_e[v];
    }
    return sum;
}

inline int graph_proper_edges(const graph *g)
{
    const int num_v = g->num_v;
    int sum = 0;
    int v = 0;
    for (; v < num_v; ++v) 
    {
        int e = 0;
        for (; e < g->num_e[v]; ++e)
        {
            if (g->adj_list[v][e] != v)
            {
                ++sum;
            }
        }
    }
    return sum;
}

inline int graph_loops(const graph *g)
{
    const int num_v = g->num_v;
    int sum = 0;
    int v = 0;
    for (; v < num_v; ++v) 
    {
        int e = 0;
        for (; e < g->num_e[v]; ++e)
        {
            if (g->adj_list[v][e] == v)
            {
                ++sum;
            }
        }
    }
    return sum;
}

inline int graph_outdegree(const graph *g, int u)
{
    return g->num_e[u];
}

inline int graph_indegree(const graph *g, int u)
{
    const int num_v = g->num_v;
    int sum = 0;
    int v = 0;
    for (; v < num_v; ++v) 
    {
        int e = 0;
        for (; e < g->num_e[v]; ++e)
        {
            if (g->adj_list[v][e] == u)
            {
                ++sum;
            }
        }
    }
    return sum;
}

inline int graph_is_balanced(const graph *g)
{
    const int num_v = g->num_v;
    int u = 0;
    for (; u < num_v; ++u)
    {
        if (graph_indegree(g, u) != graph_outdegree(g, u))
        {
            return FALSE;
        }
    }
    return TRUE;
}

inline void graph_add_edge(graph *g, int u, int v, double weight)
{
    if (g->num_e[u] == g->capacity[u]) 
    {
        graph_grow_lists(g, u);
    }
    g->adj_list[u][g->num_e[u]] = v;
    g->w_list[u][g->num_e[u]] = weight;
    g->num_e[u]++;
}

void graph_add_sym_edges(graph *g, int u, int v, double weight) 
{
    graph_add_edge(g, u, v, weight);
    graph_add_edge(g, v, u, weight);
}

int graph_rmv_edge(graph *g, int u, int v)
{
    const int num_e = g->num_e[u];
    int e = 0;
    for (; e < num_e; ++e) 
    {
        if (g->adj_list[u][e] == v) 
        {
            g->num_e[u]--;
            int f = e;
            for (; f < num_e - 1; ++f) 
            {
                g->adj_list[u][f] = g->adj_list[u][f+1];
                g->w_list[u][f] = g->w_list[u][f+1];
            }
            return TRUE;
        }
    }
    return FALSE;
}

int graph_rmv_sym_edges(graph *g, int u, int v)
{
    return (graph_rmv_edge(g, u, v) && graph_rmv_edge(g, v, u));
}

int graph_has_edge(graph *g, int u, int v)
{
    const int num_e = g->num_e[u];
    int e = 0;
    for (; e < num_e; ++e) 
    {
        if (g->adj_list[u][e] == v) 
        {
            return TRUE;
        }
    }
    return FALSE;
}

int graph_strongly_connected(const graph *g)
{
    const int num_v = g->num_v;
    int *group = (int*)malloc(num_v * sizeof(int));

    int u = 0;
    for (; u < num_v; ++u) 
    {
        int v = 0;
        for (; v < num_v; ++v) 
        {
            group[v] = FALSE;
        }

        group[u] = TRUE;
        graph_test_cc(g, group, u); // Call recursive function

        for (v = 0; v < num_v; ++v) 
        {
            if (group[v] == FALSE) 
            {
                free(group);
                return FALSE;
            }
        }
    }
    free(group);
    return TRUE;
}

void graph_svg(const graph *g, double *x, double *y, double size, double offset, FILE *out)
{
    fprintf(out, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">");
    const int num_v = g->num_v;
    for (int i = 0; i < num_v; ++i)
    {
        const int num_e = g->num_e[i];
        for (int j = 0; j < num_e; ++j)
        {
            const int e = g->adj_list[i][j];
            fprintf(out, "  <line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" style=\"stroke:rgb(0,0,0);stroke-width:2\"/>\n", offset + x[i] * size, offset + y[i] * size, offset + x[e] * size, offset + y[e] * size);
        }
    }
    for (int i = 0; i < num_v; ++i)
    {
        fprintf(out, "  <circle id=\"%d\" cx=\"%f\" cy=\"%f\" r=\"8\" fill=\"rgb(0,0,0)\"/>\n", i, offset + x[i] * size, offset + y[i] * size);
    }
    fprintf(out, "</svg>");
}

void graph_svg_abun(const graph *g, double *x, double *y, double size, double offset, double *abun, FILE *out)
{
    fprintf(out, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">");
    const int num_v = g->num_v;
    for (int i = 0; i < num_v; ++i)
    {
        const int num_e = g->num_e[i];
        for (int j = 0; j < num_e; ++j)
        {
            const int e = g->adj_list[i][j];
            fprintf(out, "  <line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" style=\"stroke:rgb(0,0,0);stroke-width:1\"/>\n", offset + x[i] * size, offset + y[i] * size, offset + x[e] * size, offset + y[e] * size);
        }
    }
    int clr[3] = {0, 0, 0};
    for (int i = 0; i < num_v; ++i)
    {
        clr[0] = (int)(abun[i] * 255);
        clr[2] = (int)((1.0 - abun[i]) * 255);
        fprintf(out, "  <circle id=\"%d\" cx=\"%f\" cy=\"%f\" r=\"%u\" stroke=\"black\" stroke-width=\"1\" fill=\"rgb(%d,%d,%d)\"/>\n", i, offset + x[i] * size, offset + y[i] * size, CIRCLE_RADIUS, clr[0], clr[1], clr[2]);
    }
    fprintf(out, "</svg>");
}

void graph_graphml(const graph *g, FILE *out, unsigned int id)
{
    const int num_v = g->num_v;
    fprintf(out, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(out, "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\"");
    fprintf(out, " xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"");
    fprintf(out, " xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n");

    fprintf(out, "  <graph id=\"%u\" edgedefault=\"directed\">\n", id);
    for (int i = 0; i < num_v; ++i)
    {
        fprintf(out, "    <node id=\"v%d\"/>\n", i);
    }
    for (int i = 0; i < num_v; ++i)
    {
        for (int e = 0; e < g->num_e[i]; ++e) 
        {
            fprintf(out, "    <edge source=\"v%d\" target=\"v%d\"/>\n", i, g->adj_list[i][e]);
        }
    }
    fprintf(out, "  </graph>\n");
    fprintf(out, "</graphml>\n");
}

void graph_print(const graph *g, FILE *out)
{
    for (int i = 0; i < g->num_v; ++i) 
    {
        fprintf(out, "%5d -> ", i);
        for (int e = 0; e < g->num_e[i]; ++e) 
        {
            fprintf(out, "%d ", g->adj_list[i][e]);
        }
        fprintf(out, "\n");
    }
}

void graph_free(graph *g)
{
    const int num_v = g->num_v;
    for (int i = 0; i < num_v; ++i) 
    {
        free(g->adj_list[i]);
        free(g->w_list[i]);
    }
    free(g->adj_list);
    g->adj_list = NULL;
    free(g->w_list);
    g->w_list = NULL;
    free(g->num_e);
    g->num_e = NULL;
    free(g->capacity);
    g->capacity = NULL;
}

void graph_get_rgg(graph *g, int vertices, double r, double *x, double *y, well1024 *rng)
{
    graph_init(g, vertices);

    for (int i = 0; i < vertices; ++i)
    {
        x[i] = well1024_next_double(rng);
        y[i] = well1024_next_double(rng);
    }	
    double d;
    for (int i = 0; i < vertices; ++i) 
    {
        for (int j = 0; j < vertices; ++j) 
        {
            const double a = x[i] - x[j];
            const double b = y[i] - y[j];
            d = hypot(a, b);
            if (d < r) 
            {
                graph_add_edge(g, i, j, r - d);
            }
        }
    }
}

void graph_get_crgg(graph *g, int vertices, double r, double *x, double *y, well1024 *rng)
{
    graph_init(g, vertices);
    graph_get_rgg(g, vertices, r, x, y, rng);

    while (graph_strongly_connected(g) == FALSE)
    {
        graph_free(g);
        graph_get_rgg(g, vertices, r, x, y, rng);
    }
}

void graph_get_llrgg(graph *g, int vertices, double r, double *x, double *y, well1024 *rng)
{
    graph_init(g, vertices);

    for (int i = 0; i < vertices; ++i)
    {
        x[i] = well1024_next_double(rng);
        y[i] = well1024_next_double(rng);
    }	
    double d;
    for (int i = 0; i < vertices; ++i) 
    {
        for (int j = 0; j < vertices; ++j) 
        {
            if (i != j)
            {
                const double a = x[i] - x[j];
                const double b = y[i] - y[j];
                d = hypot(a, b);
                if (d < r) 
                {
                    graph_add_edge(g, i, j, r - d);
                }
            }
        }
    }
}

void graph_get_llcrgg(graph *g, int vertices, double r, double *x, double *y, well1024 *rng)
{
    graph_init(g, vertices);
    graph_get_llrgg(g, vertices, r, x, y, rng);

    while (graph_strongly_connected(g) == FALSE)
    {
        graph_free(g);
        graph_get_llrgg(g, vertices, r, x, y, rng);
    }
}

void graph_get_rec_rgg(graph *g, int vertices, double width, double r, double *x, double *y, well1024 *rng)
{
    graph_init(g, vertices);

    const double length = 1.0 / width; // A = l * w so l = 1 / w

    for (int i = 0; i < vertices; ++i) 
    {
        x[i] = well1024_next_double(rng) * length;
        y[i] = well1024_next_double(rng) * width;
    }
    double d;
    for (int i = 0; i < vertices; ++i) 
    {
        for (int j = 0; j < vertices; ++j) 
        {
            const double a = x[i] - x[j];
            const double b = y[i] - y[j];
            d = hypot(a, b);
            if (d < r) 
            {
                graph_add_edge(g, i, j, r - d);
            }
        }
    }
}

void graph_get_rec_crgg(graph *g, int vertices, double width, double r, double *x, double *y, well1024 *rng)
{
    graph_init(g, vertices);
    graph_get_rec_rgg(g, vertices, width, r, x, y, rng);

    while (graph_strongly_connected(g) == FALSE) 
    {
        graph_free(g);
        graph_get_rec_rgg(g, vertices, width, r, x, y, rng);
    }
}

void graph_get_complete(graph *g, int vertices)
{
    graph_init(g, vertices);

    for (int i = 0; i < vertices; ++i) 
    {
        for (int j = 0; j < vertices; ++j) 
        {
            graph_add_edge(g, i, j, 1.0);
        }
    }
}

void graph_get_circle(graph *g, int vertices)
{
    graph_init(g, vertices);

    graph_add_edge(g, vertices - 1, vertices - 1, 1.0);
    graph_add_sym_edges(g, 0, vertices - 1, 1.0);

    for (int u = 0; u < vertices - 1; ++u)
    {
        graph_add_edge(g, u, u, 1.0);
        graph_add_sym_edges(g, u, u + 1, 1.0);
    }
}

void graph_get_star(graph *g, int vertices)
{
    graph_init(g, vertices);

    for (int u = 0; u < vertices; ++u)
    {
        graph_add_edge(g, u, u, 1.0);
        graph_add_sym_edges(g, u, 0, 1.0);
    }
}

///////////////////////////////////////////////////////////////
// 'Private' functions

void graph_test_cc(const graph *g, int *group, int u)
{
    const int num_e = g->num_e[u];
    int e = 0;
    for (; e < num_e; ++e) 
    {
        const int head = g->adj_list[u][e];
        if (head != u && group[head] == FALSE)
        {
            group[head] = TRUE;
            graph_test_cc(g, group, head);
        }
    }
}

// Grow the edge list of some vertex v.
void graph_grow_lists(graph *g, int u)
{
    g->capacity[u] <<= 1;

    int *tmp_adj = (int*)malloc(g->capacity[u] * sizeof(int));
    double *tmp_w = (double*)malloc(g->capacity[u] * sizeof(double));

    const int num_e = g->num_e[u];
    int e = 0;
    for (; e < num_e; ++e) 
    {
        tmp_adj[e] = g->adj_list[u][e];
        tmp_w[e] = g->w_list[u][e];
    }
    int *swap_adj = g->adj_list[u];
    double *swap_w = g->w_list[u];

    g->adj_list[u] = tmp_adj;
    g->w_list[u] = tmp_w;

    free(swap_adj);
    free(swap_w);
}
