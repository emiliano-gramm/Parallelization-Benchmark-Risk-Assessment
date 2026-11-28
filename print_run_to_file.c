#include <stdio.h>

#include "output_def.h"
#include "config.h"

void print_run_to_file(char *file_path, Results results) {
    // print serial run
    FILE *clean_results = fopen(file_path, "w");
    fprintf(clean_results, "\n");
    fclose(clean_results);
    FILE *serial_results = fopen(file_path, "a");

    // saves pointer from the all_experiments pointer where each slot is for the head to the contiguos memory for each experiment
    int *current_final_result;

    // First loop through each n number of road users
    for (int i = 0; i < (int)(sizeof(N)/sizeof(N[0])); i++) {
        current_final_result = results.all_experiments[i]; // should save the memory address for each head of the contiguos address of experiments
        for (int j = 0; j < N[i]*(N[i]-1)/2; j++) {
            fprintf(serial_results, "%d, ", current_final_result[j]);
        }
        fprintf(serial_results, "%f, %d\n", results.timer_results[i], results.total_collisions[i]);
    }
    fclose(serial_results);
}