#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"
#include "scenario.h"


Scene build_scenario(int numRU) {
    /*
    build a synthetic box containing 'road user' objects with random coordinates for each timestep
    Scene is the struct containing the coords and the bboxes of every road user through every timestamp
    First allocate the memory to the heap with malloc
    Then fill the array with for loops. (i*H*K) -> decides the # object, (j*K) -> decides the timestep
    Size of each road uses bbox are constant, filled array of 2*N since bbox is 2 dimensions per object.
    */

    Scene scene;

    /*
    Memory allocated for 1D array of whole scenary
    N[x]*12*2*sizeof(float), ex x=0 -> 8*12*2*4=768, x=9 -> 8_192*12*2*4=786_432 bytes in memory.
    malloc returns pointer to first float in contiguos memory
    */
    scene.p_coords = malloc((size_t)numRU*H*K*sizeof(float));
    scene.p_bbox = malloc((size_t)numRU*2*sizeof(float));

    //Map grows according to sqrt(N)
    float map_w = BASE_MAP_W * sqrt(numRU);
    float map_h = BASE_MAP_H * sqrt(numRU);

    for (int i = 0; i < numRU; i++) {
        // Random initial coordinate per RU
        float x = (double)(rand())/(double)(RAND_MAX) * (map_w);
        float y = (double)(rand())/(double)(RAND_MAX) * (map_h);

        // Physics values for movement, random
        float theta = (double)(rand())/(double)(RAND_MAX) * 6.2831853f; // 6.2831853 is pi*2 which is 2pi radians
        float speed = 3.0 + (double)rand()/(double)RAND_MAX * 12.0; // min 3.0, max 15
        float yaw = (double)rand()/(double)RAND_MAX * 0.8 - 0.4; // min -0.4, max 0.4

        // coords move for H steps
        for (int j = 0; j < H; j++) {
            // save coords to struct
            scene.p_coords[(i*H*K)+(j*K)] = x;
            scene.p_coords[(i*H*K)+(j*K)+1] = y;
            scene.p_coords[(i*H*K)+(j*K)+2] = theta; // allows oriented bboxes

            // movement is constant
            x += speed*cosf(theta) * DELTA;
            y += speed*sinf(theta) * DELTA;
            theta += yaw * DELTA; // turning rate
        }
        scene.p_bbox[i*2] = HALF_BBOX_X;
        scene.p_bbox[i*2+1] = HALF_BBOX_Y;
    }
    
    // Memory freed in main func
    return scene;
}






