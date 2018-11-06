/*
 * sinoscope_openmp.c
 *
 *  Created on: 2011-10-14
 *      Author: francis
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#include "sinoscope.h"
#include "color.h"
#include "util.h"

int sinoscope_image_openmp(sinoscope_t *ptr)
{
    //TODO("sinoscope_image_openmp");

    if (ptr == NULL)
        return -1;

    sinoscope_t sino = *ptr;
    int width = sino.width;
    int height = sino.height;
    int x, y;

    #pragma omp parallel for private(x)
    for (x = 1; x < width - 1; x++) {
        
        int sino_taylor = sino.taylor;
        float sino_phase0 = sino.phase0;
        float sino_phase1 = sino.phase1;
        int sino_interval = sino.interval;
        float sino_interval_inv = sino.interval_inv;
        float sino_dx = sino.dx;
        float sino_dy = sino.dy;
        float sino_time = sino.time;
        struct rgb c;
        int index, taylor;
        float val;

        for (y = 1; y < height - 1; y++) { 
            float px = sino_dx * y - 2 * M_PI;
            float py = sino_dy * x - 2 * M_PI;

            val = 0.0f;
            
            for (taylor = 1; taylor <= sino_taylor; taylor += 2) {
                val += sin(px * taylor * sino_phase1 + sino_time) / taylor + cos(py * taylor * sino_phase0) / taylor;
            }
            val = (atan(1.0 * val) - atan(-1.0 * val)) / (M_PI);
            val = (val + 1) * 100;

            value_color(&c, val, sino_interval, sino_interval_inv);
            index = (y * 3) + (x * 3) * width;
            
            sino.buf[index + 0] = c.r;
            sino.buf[index + 1] = c.g;
            sino.buf[index + 2] = c.b;
        }
    }
    return 0;
}
