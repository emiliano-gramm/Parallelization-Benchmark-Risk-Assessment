#ifndef OUTPUT_DEF_H
#define OUTPUT_DEF_H

#include <stdbool.h>

typedef struct {
    int **all_experiments;
    int *total_collisions;
    int *num_pairs; // pointer since it uses diff N inside backends
    double *timer_results;
    char *backend; // openmp, cuda, serial
    char *schedule; // subtype backend
    bool *verified; // same as above
    int threads; // Does not change per run inside backend
} Results;


#endif