#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "cpu_run.h"
#include "omp.h"
#include "scenario.h"
#include "collision_math.h"

void cpu_run(Scene *scenes, Results *results) {
    // SERIAL RUN
    // All experiments just need the initial pointer to the contiguos memory, this is an array of pointers

    results->all_experiments = malloc((size_t)sizeof(N)/sizeof(N[0]) * sizeof(int *));
    results->total_collisions = malloc((size_t)sizeof(N)/sizeof(N[0]) * sizeof(int));
    results->num_pairs = malloc((size_t)sizeof(N)/sizeof(N[0]) * sizeof(int));
    results->timer_results = malloc((size_t)sizeof(N)/sizeof(N[0]) * sizeof(double));;
    results->backend = malloc((size_t)10*sizeof(char));
    results->schedule = malloc((size_t)15*sizeof(char));
    results->verified = NULL;
    
    for (int n = 0; n < (int)(sizeof(N)/sizeof(N[0])); n++) {
        int total_collisions = 0;
        // assign memory for current experiment, total results is N*(N-1)/2
        int *current_experiment = malloc((size_t)(N[n]*(N[n]-1))/2.0 *sizeof(int));
        // build current scene for n number of road users
        Scene current_scene = scenes[n];
        double current_timer = omp_get_wtime(); // start time before parallelizable loops
        for (int i = 0; i < H*K*N[n]; i+=H*K) {
            for (int j = i + H*K; j < H*K*N[n]; j+=H*K) {
                int a = i/(H*K), b = j/(H*K);
                int p = a*(2*N[n]-a-1)/2+(b-a-1);
                // Find collision for every pair
                current_experiment[p] = find_collision_step(
                    &current_scene.p_coords[i],
                    &current_scene.p_coords[j],
                    &current_scene.p_bbox[i/(H*K)*2],
                    &current_scene.p_bbox[j/(H*K)*2]
                );
                if (current_experiment[p] >= 0) {
                    total_collisions++;
                }
            }
        }
        current_timer = omp_get_wtime() - current_timer;

        results->all_experiments[n] = &current_experiment[0];
        results->total_collisions[n] = total_collisions;
        results->num_pairs[n] = N[n]*(N[n]-1)/2;
        results->timer_results[n] = current_timer;
        results->threads = 1;
        strcpy(results->backend, "serial");
        strcpy(results->schedule, "serial");
    }
}
