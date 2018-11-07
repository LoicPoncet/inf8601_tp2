/*
 * color.c
 *
 *  Created on: 2011-08-27
 *      Author: francis
 *
 * Code related to color management
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "color.h"

const struct rgb white = { .r = 255, .g = 255, .b = 255 };
const struct rgb black = { .r = 0, .g = 0, .b = 0 };

int save_image(char *path, struct rgb *image, int width, int height)
{
    FILE *f = NULL;

    if (image == NULL || path == NULL)
        return -1;

    if ((f = fopen(path, "wb")) == NULL) {
        char *msg;
        if (asprintf(&msg, "Failed to open %s", path) < 0)
            perror("Failed to open output file");
        else
            perror(msg);
        return -1;
    }

    fprintf(f, "P6\n%d %d\n%d\n", width, height, 255);
    fwrite(image, sizeof(struct rgb), width * height, f);
    fclose(f);
    return 0;
}

int save_image_uchar(char *path, unsigned char *image, int width, int height)
{
    FILE *f = NULL;

    if (image == NULL || path == NULL)
        return -1;

    if ((f = fopen(path, "wb")) == NULL) {
        char *msg;
        if (asprintf(&msg, "Failed to open %s", path) < 0)
            perror("Failed to open output file");
        else
            perror(msg);
        return -1;
    }

    fprintf(f, "P6\n%d %d\n%d\n", width, height, 255);
    fwrite(image, sizeof(unsigned char), width * height * 3, f);
    fclose(f);
    return 0;
}
/*
 * Set the maximum value for the color scale
 * The minimum color scale is 0
 * The minimum value is 4, the number of color intervals */
int get_color_interval(float max)
{
    if (max < 4.0f)
        max = 4.0f;
    return (int) max / 4;
}

float get_color_interval_inv(float max)
{
    if (max < 4.0f)
        max = 4.0f;
    return (float) 4 / max;
}

void value_color(struct rgb *color, float value, int interval, float interval_inv)
{
    if (isnan(value)) {
        *color = black;
        return;
    }
    int x = (((int)value % interval) * 255) * interval_inv;
    int i = value * interval_inv;
    switch(i) {
    case 0:
        color->r = 0;
        color->g = x;
        color->b = 255;
        break;
    case 1:
        color->r = 0;
        color->g = 255;
        color->b = 255 - x;
        break;
    case 2:
        color->r = x;
        color->g = 255;
        color->b = 0;
        break;
    case 3:
        color->r = 255;
        color->g = 255 - x;
        color->b = 0;
        break;
    case 4:
        color->r = 255;
        color->g = 0;
        color->b = x;
        break;
    default:
        *color = white;
        break;
    }
}

void hue(struct rgb **image, int width, int height)
{
    int i, j;

    *image = (struct rgb*) calloc(width * height, sizeof(struct rgb));
    struct rgb *img = *image;
    int interval = get_color_interval((float) height);
    float interval_inv = get_color_interval_inv((float) height);
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            value_color(&img[j * width + i], (float) j, interval, interval_inv);
        }
    }
}
