Goal:
Determine at which N, the number of road users, a parallel collision check beats a serial CPU scan. Each unordered pair is tested over a short forecast, and the result is the earliest timestep where their boxes overlap.

The same scenes and the same geometry are used for the serial scan, the OpenMP scans, and later the GPU scan. N runs from 8 to 8192. There are 12 steps of 0.1 s. The random seed is 73.

Files:
config.h
- The shared constants: the N sweep, H, DELTA, the seed, the map size, the box half-sizes, and whether the boxes are oriented.

scenario.c and scenario.h
- Build one deterministic scene per N. Coordinates and box sizes are flat arrays. main builds every scene once and passes that array to each scanner.

collision_math.h
- The overlap test for one pair. Serial, OpenMP, and CUDA call this same function.

cpu_run.c and cpu_run.h
- The serial scan. It checks each unordered pair once and stores the earliest overlap step.

openmp_run.c and openmp_run.h
- The same pair scan with OpenMP. One version splits the outer object loop into contiguous blocks. Another hands the rows out in chunks of 16 so long and short rows are mixed.

output_def.h
- The Results struct: one pair-result array per N, and the conflict count for that N.

print_run_to_file.c and print_run_to_file.h
- Write a finished Results struct to a text file.

test_collision.c and test_collision.h
- Four hand-checked pairs for the geometry: an overlap at step 0, a head-on overlap at step 6, and two misses.

main.c
- Builds the scenes, runs the serial scan and the OpenMP scans on those scenes, and writes the result files.
