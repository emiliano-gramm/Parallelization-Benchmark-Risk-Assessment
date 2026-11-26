#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cpu_run.h"
#include "openmp_run.h"
#include "config.h"
#include "output_def.h"
#include "scenario.h"
#include "print_run_to_file.h"

int main() {

    srand(RSEED); // once to make deterministic

    Scene *scenes = malloc((size_t)sizeof(N)/sizeof(N[0])*sizeof(Scene)); // memory for all scenes

    for (int i = 0; i < sizeof(N)/sizeof(N[0]); i++) {
        scenes[i] = build_scenario(N[i]); // build array
    }

    // CPU, Serial Run
    Results results_serial;
    cpu_run(scenes, &results_serial); // Baseline with CPU

    // OPENMP, Naive Parallel Run
    Results results_naive_static_openmp;
    openmp_naive_static_run(scenes, &results_naive_static_openmp);

    // OPENMP, Cyclic Parallel Run
    Results results_cyclic_static_openmp;
    openmp_cyclic_static_run(scenes, &results_cyclic_static_openmp);

    print_run_to_file("results/serial_results.txt", results_serial);
    print_run_to_file("results/naive_static_openmp", results_naive_static_openmp);
    print_run_to_file("results/naive_cyclic_openmp", results_cyclic_static_openmp);

    return 0;
}