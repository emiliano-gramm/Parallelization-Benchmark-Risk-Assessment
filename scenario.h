#ifndef SCENARIO_H
#define SCENARIO_H

typedef struct {
    float *p_coords;
    float *p_bbox;
    float *p_theta;
} Scene;

Scene build_scenario(int numRU);

#endif