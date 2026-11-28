#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

// Number of roadusers
static const int N[10] = {8, 16, 32, 64, 256, 512, 1024, 2048, 4096, 8192};
static const int threads[5] = {1,2,4,8,16};
static const int H = 12; // Total timesteps
static const float DELTA = 0.1f; // seconds per step
static const int RSEED = 73; // random seed
static const bool IS_ORIENTED = true; // if it is axis aligned or oriented
static const int BASE_MAP_W = 5; // size of the map
static const int BASE_MAP_H = 5;
static const float HALF_BBOX_X = 1.4f; // Halfs of road user
static const float HALF_BBOX_Y = 2.25f;
static const int K = 3; // Dependant on constant BBOX_X and BBOX_Y being true
static const int trials = 5;

#endif