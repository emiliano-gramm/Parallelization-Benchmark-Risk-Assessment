#ifndef OPENMP_NAIVE_STATIC_RUN
#define OPENMP_NAIVE_STATIC_RUN

#include "scenario.h"
#include "output_def.h"

void openmp_naive_static_run(Scene *scenes, Results *results);

void openmp_cyclic_static_run(Scene *scenes, Results *results);

void openmp_1d_static_run(Scene *scenes, Results *results);


#endif