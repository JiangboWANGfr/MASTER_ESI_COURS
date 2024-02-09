#ifndef IMG_MGMT_H_GUARD
#define IMG_MGMT_H_GUARD

#include "utils.h"
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>

typedef char pixel_t[3];


typedef struct image_s {
    pixel_t *data_in, 
            *data_out;
    size_t   width, height;
} image_t;

void img_load(const char* filename, const size_t width, const size_t height, image_t* img);
void img_write(char* filein, image_t* img);

#endif // IMG_MGMT_H_GUARD 
