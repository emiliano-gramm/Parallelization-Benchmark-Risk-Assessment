#include "collision_math.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "config.h"


void fill_individual_RU(float *RU, int H, int K, float x, float y, float vx, float vy, float theta, float DELTA) {
    /*
    Fills individual road user objects, it has determined velocity for each object, they move in the X axis.
    */
    for (int i = 0; i < H; i++) {
        RU[i*K] = x;
        RU[i*K+1] = y;
        RU[i*K+2] = theta;
        x += vx*DELTA;
        y += vy*DELTA;
    }

}

void run_test(){
        //TESTS
        float bbox1[2] = {HALF_BBOX_X, HALF_BBOX_Y};
        float bbox2[2] = {HALF_BBOX_X, HALF_BBOX_Y};
        float *a = malloc((size_t)H*K*sizeof(float));
        float *b = malloc((size_t)H*K*sizeof(float));
    
        // Tests for collision_math 1, overlapping at step 0
        fill_individual_RU(a, H, K, 0, 0, 0, 0, 0, DELTA);
        fill_individual_RU(b, H, K, 0, 0, 0, 0, 0, DELTA);
        int res1 = find_collision_step(a, b, bbox1, bbox2);
        printf("%d\n", res1);
    
        // Tests for collision_math 2, approaching object check, should print 6
        fill_individual_RU(a, H, K, -5, 0, 5, 0, 0, DELTA);
        fill_individual_RU(b, H, K, 5, 0, -5, 0, 0, DELTA);
        int res2 = find_collision_step(a, b, bbox1, bbox2);
        printf("%d\n", res2);
    
        // Tests for collision_math 3, Parallel close objects check, should print -1 across timesteps
        fill_individual_RU(a, H, K, -5, 0, 5, 0, 0, DELTA);
        fill_individual_RU(b, H, K, -5, 3.5, 5, 0, 0, DELTA);
        int res3 = find_collision_step(a, b, bbox1, bbox2);
        printf("%d\n", res3);
    
        // Tests for collision_math 4
        fill_individual_RU(a, H, K, -5, 0, 5, 5, 3.14159/4.0, DELTA);
        fill_individual_RU(b, H, K, -7, 2, 5, 5, 3.14159/4.0, DELTA);
        int res4 = find_collision_step(a, b, bbox1, bbox2);
        printf("%d\n", res4);

        free(a);
        free(b);
}