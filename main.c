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
#include "print_simple_csv.h"

int main() {

    srand(RSEED); // once to make deterministic

    Scene *scenes = malloc((size_t)sizeof(N)/sizeof(N[0])*sizeof(Scene)); // memory for all scenes

    for (int i = 0; i < (int)(sizeof(N)/sizeof(N[0])); i++) {
        scenes[i] = build_scenario(N[i]); // build array
    }

    // CPU, Serial Run
    Results results_serial;
    cpu_run(scenes, &results_serial); // Baseline with CPU

    // Warmup-run
    Results warmup;
    openmp_naive_static_run(scenes, &warmup, 8);
    for (int n = 0; n < (int)(sizeof(N)/sizeof(N[0])); n++) {
        free(warmup.all_experiments[n]);
    }
    free(warmup.all_experiments);
    free(warmup.total_collisions);
    free(warmup.num_pairs);
    free(warmup.timer_results);
    free(warmup.backend);
    free(warmup.schedule);
    free(warmup.verified);
    print_run_to_file("results/serial_results.csv", results_serial);

    int num_threads = sizeof(threads)/sizeof(threads[0]);
    Results all_results[num_threads*trials*3]; // This will store the Results for all strategies across all num of threads tested
    bool verified; // each strat is compared to serial to verify correctness
    for (int t = 0; t < num_threads*trials; t++) {

        // OPENMP, Naive Parallel Run
        Results results_naive_static_openmp;
        openmp_naive_static_run(scenes, &results_naive_static_openmp, threads[t%num_threads]);
        for (int i = 0; i < (int)(sizeof(N)/sizeof(N[0])); i++) {
            verified = true;
            for (int j=0; j < results_naive_static_openmp.num_pairs[i]; j++) {
                // verifies match between openmp and serial runs
                if (results_naive_static_openmp.all_experiments[i][j] != results_serial.all_experiments[i][j]) {
                    verified = false;
                }
            }
            results_naive_static_openmp.verified[i] = verified;
        }
        all_results[t*3] = results_naive_static_openmp; // saves results, each result has 10 experiments
        for (int n = 0; n < (int)(sizeof(N)/sizeof(N[0])); n++) {
            free(all_results[t*3].all_experiments[n]); // free memory from scene
        }
        free(all_results[t*3].all_experiments);

        // OPENMP, Cyclic Parallel Run
        Results results_cyclic_static_openmp;
        openmp_cyclic_static_run(scenes, &results_cyclic_static_openmp, threads[t%num_threads]);
        for (int i = 0; i < (int)(sizeof(N)/sizeof(N[0])); i++) {
            verified = true;
            for (int j=0; j < results_cyclic_static_openmp.num_pairs[i]; j++) {
                if (results_cyclic_static_openmp.all_experiments[i][j] != results_serial.all_experiments[i][j]) {
                    verified = false;
                }
            }
            results_cyclic_static_openmp.verified[i] = verified;
        }
        all_results[t*3+1] = results_cyclic_static_openmp;
        for (int n = 0; n < (int)(sizeof(N)/sizeof(N[0])); n++) {
            free(results_cyclic_static_openmp.all_experiments[n]);
        }
        free(results_cyclic_static_openmp.all_experiments);

        // OPENMP, 1D Parallel Run
        Results results_1d_static_openmp;
        openmp_1d_static_run(scenes, &results_1d_static_openmp, threads[t%num_threads]);
        for (int i = 0; i < (int)(sizeof(N)/sizeof(N[0])); i++) {
            verified = true;
            for (int j=0; j < results_1d_static_openmp.num_pairs[i]; j++) {
                if (results_1d_static_openmp.all_experiments[i][j] != results_serial.all_experiments[i][j]) {
                    verified = false;
                }
            }
            results_1d_static_openmp.verified[i] = verified;
        }
        all_results[t*3+2] = results_1d_static_openmp;
        for (int n = 0; n < (int)(sizeof(N)/sizeof(N[0])); n++) {
            free(results_1d_static_openmp.all_experiments[n]);
        }
        free(results_1d_static_openmp.all_experiments);
    }

    for (int n = 0; n < (int)(sizeof(N)/sizeof(N[0])); n++) {
        free(results_serial.all_experiments[n]);
    }

    print_simple_csv(all_results, "results/openmp.csv");
    return 0;
}