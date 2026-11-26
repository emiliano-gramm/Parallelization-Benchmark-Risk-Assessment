#ifndef COLLISION_MATH_H
#define COLLISION_MATH_H

#include <math.h>
#include "config.h"

static inline // inline eliminate call overhead during experiments, static prevents collision between definitions
int find_collision_step(float *RU1_coords, float *RU2_coords, float *RU1_bbox, float *RU2_bbox) {
    /*
    Determines H step when there is a collision happening between a pair of Road Users
    */

    float x1, y1, theta1, x2, y2, theta2;
    float ufa1[2], ula1[2], ufa2[2], ula2[2]; // Unit forward axis and unit lateral axis
    float c1[2], c2[2];
    float d[2];
    float R1, R2;
    float *axes[4] = {ufa1, ula1, ufa2, ula2};
    float bbox_x1 = RU1_bbox[0], bbox_y1 = RU1_bbox[1];
    float bbox_x2 = RU2_bbox[0], bbox_y2 = RU2_bbox[1];
    for (int i = 0; i < H; i++) {
        // Define center points of two objects
        x1 = RU1_coords[i*K];
        y1 = RU1_coords[i*K + 1];
        theta1 = RU1_coords[i*K + 2];
        x2 = RU2_coords[i*K];
        y2 = RU2_coords[i*K + 1];
        theta2 = RU2_coords[i*K + 2];

        // Orientation for bounding boxes
        ufa1[0] = cos(theta1), ufa1[1] = sin(theta1); // how much it points along cos and sin
        ula1[0] = -sin(theta1), ula1[1] = cos(theta1);
        ufa2[0] = cos(theta2), ufa2[1] = sin(theta2);
        ula2[0] = -sin(theta2), ula2[1] = cos(theta2);

        for (int t = 0; t < 4; t++){
            axes[0] = ufa1;
        }

        // Center vectors for RUs
        c1[0] = x1, c1[1] = y1;
        c2[0] = x2, c2[1] = y2;
        // vector between the centers
        d[0] = c1[0] - c2[0];
        d[1] = c1[1] - c2[1];
        // cheap test, passes based on a circular radius, not rectangular bbox
        R1 = sqrt(pow(bbox_x1,2) + pow(bbox_y1, 2));
        R2 = sqrt(pow(bbox_x2,2) + pow(bbox_y2, 2));
        if ((R1+R2)<=(sqrt(pow(d[0],2) + pow(d[1],2)))) {
            continue;
        }

        /*
        In Axis-aligned bboxes if the gap between the gaps of x and y in the pair
        is less than the sum of half of their bboxes (already halved by default)
        then the road users are colliding. fabsf(x1-x2) <= bbox_x1+bbox_x2 && fabsf(y1-y2) <= bbox_y1+bbox_y2

        For Oriented bboxes I check the 2 axis of each road user, total of 4, if any of them are not overlapping
        then the objects are not colliding (separating axis theorem)
        */
        float *L;
        int gap = 0;
        for (int j = 0; j < sizeof(axes)/sizeof(axes[0]); j++) { // 4 total axis
            L = axes[j];
            // dot product to find projection of distance vector of the centers projected onto L
            float dist = fabsf(d[0]*L[0] + d[1]*L[1]);
            // equation to get radiuses projected
            float r1 = bbox_y1*fabsf(ufa1[0]*L[0] + ufa1[1]*L[1]) + bbox_x1*fabsf(ula1[0]*L[0] + ula1[1]*L[1]);
            float r2 = bbox_y2*fabsf(ufa2[0]*L[0] + ufa2[1]*L[1]) + bbox_x2*fabsf(ula2[0]*L[0] + ula2[1]*L[1]);
            // if the distance of the radius projections is less than the projected distance between the centers
            if (dist > r1+r2) {
                gap = 1;
                break;
            }
        }
        if (!gap) {
            return i;
        }
    }
    return -1;
}
#endif