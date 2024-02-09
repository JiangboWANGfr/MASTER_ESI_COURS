#include "img_mgmt.h"
void convolution(image_t *img, float coefficient)
{
    pixel_t(*bitmap_in)[img->height] = (pixel_t(*)[img->height])img->data_in,
    (*bitmap_out)[img->height] = (pixel_t(*)[img->height])img->data_out;

    uint32_t X = img->width,
             Y = img->height;

    // convolution operation
    //  define the kernel
    float Kernel[3][3] = {
        {coefficient, coefficient, coefficient},
        {coefficient, coefficient, coefficient},
        {coefficient, coefficient, coefficient}};
    float sum = 0.0;
    for (size_t x = 1; x < X - 1; x++)
    {
        for (size_t y = 1; y < Y - 1; y++)
        {
            float s = 0.0;
            for (int k = -1; k <= 1; k++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    uint32_t px = 0;
                    for (int l = 0; l < 3; ++l)
                        px |= (bitmap_in[x - k][y - k][l] << 8 * l);
                    s = s + Kernel[j + 1][k + 1] * px;
                }
            }
            sum += s;
            for (int l = 0; l < 3; ++l)
            { // bitmap_out[x,y] = sum;
                uint32_t s = (uint32_t)sum;
                bitmap_out[x][y][l] = (s >> 8 * l) & 0xFF;
            }
        }
    }
}
