#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "config.h"
#include "openmp_run.h"
#include "scenario.h"
#include "collision_math.h"

void openmp_naive_static_run(Scene *scenes, Results *results) {
    // SERIAL RUN
    // All experiments just need the initial pointer to the contiguos memory, this is an array of pointers
    
    results->all_experiments = malloc((size_t)sizeof(N)/sizeof(N[0]) * sizeof(int *)); // -> gets member of pointer object
    results->total_collisions = malloc((size_t)sizeof(N)/sizeof(N[0]) * sizeof(int));

    for (int n = 0; n < sizeof(N)/sizeof(N[0]); n++) {
        int total_collisions = 0;
        // assign memory for current experiment, total results is N*(N-1)/2
        int *current_experiment = malloc((size_t)(N[n]*(N[n]-1))/2.0 *sizeof(int));
        // build current scene for n number of road users
        Scene current_scene = scenes[n];

        #pragma omp parallel for schedule(static) reduction(+:total_collisions)
        for (int i = 0; i < H*K*N[n]; i+=H*K) {
            for (int j = i + H*K; j < H*K*N[n]; j+=H*K) {
                /*  
                Get index to write to for current experiment
                ind_exp was causing clashes between the threads
                Having i and j explicitly decide where to write to in the result array
                allows each thread to avoid clashing
                */
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
        results->all_experiments[n] = &current_experiment[0];
        results->total_collisions[n] = total_collisions;
    }
}

void openmp_cyclic_static_run(Scene *scenes, Results *results) {
    // SERIAL RUN
    // All experiments just need the initial pointer to the contiguos memory, this is an array of pointers
    
    results->all_experiments = malloc((size_t)sizeof(N)/sizeof(N[0]) * sizeof(int *)); // -> gets member of pointer object
    results->total_collisions = malloc((size_t)sizeof(N)/sizeof(N[0]) * sizeof(int));

    for (int n = 0; n < sizeof(N)/sizeof(N[0]); n++) {
        int total_collisions = 0;
        // assign memory for current experiment, total results is N*(N-1)/2
        int *current_experiment = malloc((size_t)(N[n]*(N[n]-1))/2.0 *sizeof(int));
        // build current scene for n number of road users
        Scene current_scene = scenes[n];

        #pragma omp parallel for schedule(static, 16) reduction(+:total_collisions)
        for (int i = 0; i < H*K*N[n]; i+=H*K) {
            for (int j = i + H*K; j < H*K*N[n]; j+=H*K) {
                /*  
                Get index to write to for current experiment
                ind_exp was causing clashes between the threads
                Having i and j explicitly decide where to write to in the result array
                allows each thread to avoid clashing
                */
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
        results->all_experiments[n] = &current_experiment[0];
        results->total_collisions[n] = total_collisions;
    }
}


void openmp_1d_static_run(Scene *scenes, Results *results) {
    // SERIAL RUN
    // All experiments just need the initial pointer to the contiguos memory, this is an array of pointers
    
    results->all_experiments = malloc((size_t)sizeof(N)/sizeof(N[0]) * sizeof(int *)); // -> gets member of pointer object
    results->total_collisions = malloc((size_t)sizeof(N)/sizeof(N[0]) * sizeof(int));

    for (int n = 0; n < sizeof(N)/sizeof(N[0]); n++) {
        int total_collisions = 0;
        int num_pairs = (N[n]*(N[n]-1))/2;
        // assign memory for current experiment, total results is N*(N-1)/2
        int *current_experiment = malloc((size_t)num_pairs *sizeof(int));
        // build current scene for n number of road users
        Scene current_scene = scenes[n];

        #pragma omp parallel for schedule(static) reduction(+:total_collisions)
        for (int i = 0; i < num_pairs; i++) {
            /*
            Formula from https://en.wikipedia.org/wiki/Triangular_number#Triangular_roots_and_tests_for_triangular_numbers
            */
            int backward_leftover = num_pairs-i-1; // count backwards to get correct placements
            int current_row = floor((sqrt((8*backward_leftover)+1)-1)/2); // the current count of elements in the row
            int a = N[n]-2-current_row; // first RU
            int ru_before_a = a * (2*N[n] - a - 1) / 2; // vehicles before 'a' to avoid duplicates
            int b = a+1+(i-ru_before_a); //second RU


            current_experiment[i] = find_collision_step(
                &current_scene.p_coords[a*H*K],
                &current_scene.p_coords[b*H*K],
                &current_scene.p_bbox[a*2],
                &current_scene.p_bbox[b*2]
            );

            if (current_experiment[i] >= 0) {
                total_collisions++;
            }
        }
        results->all_experiments[n] = &current_experiment[0];
        results->total_collisions[n] = total_collisions;
    }
}