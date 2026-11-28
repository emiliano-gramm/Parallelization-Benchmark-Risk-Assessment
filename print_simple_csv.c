#include <stdio.h>

#include "print_simple_csv.h"
#include "config.h"
#include "output_def.h"

void print_simple_csv(Results *results, char *path) {

    int num_threads = sizeof(threads)/sizeof(threads[0]);

    int num_tested = num_threads*trials*3;

    // clean before run
    FILE *file = fopen(path, "w");
    fprintf(file, "trial,backend,schedule,threads,N,H,pairs,conflicts,verified,time_ms\n");
    fclose(file);
    file = fopen(path, "a");
    for (int i = 0; i<num_tested; i++) {
        for (int j = 0; j<(int)(sizeof(N)/sizeof(N[0])); j++) {
            fprintf(file, "%d,%s,%s,%d,%d,%d,%d,%d,%d,%f\n",
                (i/3)/num_threads,
                results[i].backend,
                results[i].schedule,
                results[i].threads,
                N[j],
                H,
                results[i].num_pairs[j],
                results[i].total_collisions[j],
                results[i].verified[j],
                results[i].timer_results[j]*1000
            );
        }
    }
}