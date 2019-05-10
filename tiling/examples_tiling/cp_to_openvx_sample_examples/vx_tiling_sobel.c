/*
 * Copyright (c) 2013-2014 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and/or associated documentation files (the
 * "Materials"), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 */

#include <stdio.h>
#include <stdlib.h>
#include <VX/vx_khr_tiling.h>

/*! \file
 * \example vx_tiling_sobel.c
 */

/*! \brief A 3x3 to 1x1 sobel filter
 * This kernel uses this tiling definition.
 * \code
 * vx_block_size_t outSize = {1,1};
 * vx_neighborhood_size_t inNbhd = {-1,1,-1,1};
 * \endcode
 * \ingroup group_tiling
 */
//! [sobel_tiling_function]
vx_uint8 vx_clamp_u8_i32(vx_int32 value)
{
    vx_uint8 v = 0;
    if (value > 255)
    {
        v = 255;
    }
    else if (value < 0)
    {
        v = 0;
    }
    else
    {
        v = (vx_uint8)value;
    }
    return v;
}

static vx_int16 vx_clamp_s16_i32(vx_int32 value)
{
    vx_int16 v = 0;
    if (value > INT16_MAX)
    {
        v = INT16_MAX;
    }
    else if (value < INT16_MIN)
    {
        v = INT16_MIN;
    }
    else
    {
        v = (vx_int16)value;
    }
    return v;
}

static vx_int16 sobel_x[3][3] = {
    {-1, 0, +1},
    {-2, 0, +2},
    {-1, 0, +1},
};

static vx_int16 sobel_y[3][3] = {
    {-1, -2, -1},
    { 0,  0,  0},
    {+1, +2, +1},
};

void sobel_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT],
                      void * VX_RESTRICT tile_memory,
                      vx_size tile_memory_size)
{
    vx_uint32 x, y;
    vx_uint32 m, n;
    vx_tile_t *in = (vx_tile_t *)parameters[0];
    vx_tile_t *out0 = (vx_tile_t *)parameters[1];
    vx_tile_t *out1 = (vx_tile_t *)parameters[2];

    vx_int32 div_x, div_y;
    div_x = sobel_x[0][0] + sobel_x[0][1] + sobel_x[0][2]+
            sobel_x[1][0] + sobel_x[1][1] + sobel_x[1][2]+
            sobel_x[2][0] + sobel_x[2][1] + sobel_x[2][2];
    if (div_x == 0)
        div_x = 1;

    div_y = sobel_y[0][0] + sobel_y[0][1] + sobel_y[0][2]+
            sobel_y[1][0] + sobel_y[1][1] + sobel_y[1][2]+
            sobel_y[2][0] + sobel_y[2][1] + sobel_y[2][2];
    if (div_y == 0)
        div_y = 1;

    printf("TileHeight: %d \n", vxTileHeight(out0,0));
    printf("TileWidth: %d \n", vxTileWidth(out0,0));
    printf("TileBlockHeight: %d \n",vxTileBlockHeight(out0));
    printf("TileBlockWidth: %d \n",vxTileBlockWidth(out0));

    for (y = 0; y < vxTileHeight(out0, 0); y+=vxTileBlockHeight(out0))
    {
        for (x = 0; x < vxTileWidth(out0, 0); x+=vxTileBlockWidth(out0))
        {
            for (n = 0u; n < vxTileBlockHeight(out0); n++)
            {
                for (m = 0u; m < vxTileBlockWidth(out0); m++)
                { 
                    vx_int32 sum_x, sum_y;
                    sum_x = vxImagePixel(vx_uint8, in, 0, x+m, y+n, -1, -1) * sobel_x[0][0] +
                            vxImagePixel(vx_uint8, in, 0, x+m, y+n,  0, -1) * sobel_x[0][1] +
                            vxImagePixel(vx_uint8, in, 0, x+m, y+n, +1, -1) * sobel_x[0][2] +
                            vxImagePixel(vx_uint8, in, 0, x+m, y+n, -1,  0) * sobel_x[1][0] +
                            vxImagePixel(vx_uint8, in, 0, x+m, y+n,  0,  0) * sobel_x[1][1] +
                            vxImagePixel(vx_uint8, in, 0, x+m, y+n, +1,  0) * sobel_x[1][2] +
                            vxImagePixel(vx_uint8, in, 0, x+m, y+n, -1, +1) * sobel_x[2][0] +
                            vxImagePixel(vx_uint8, in, 0, x+m, y+n,  0, +1) * sobel_x[2][1] +
                            vxImagePixel(vx_uint8, in, 0, x+m, y+n, +1, +1) * sobel_x[2][2];

                    sum_y = vxImagePixel(vx_uint8, in, 0, x+m, y+n, -1, -1) * sobel_y[0][0] +
                            vxImagePixel(vx_uint8, in, 0, x+m, y+n,  0, -1) * sobel_y[0][1] +
                            vxImagePixel(vx_uint8, in, 0, x+m, y+n, +1, -1) * sobel_y[0][2] +
                            vxImagePixel(vx_uint8, in, 0, x+m, y+n, -1,  0) * sobel_y[1][0] +
                            vxImagePixel(vx_uint8, in, 0, x+m, y+n,  0,  0) * sobel_y[1][1] +
                            vxImagePixel(vx_uint8, in, 0, x+m, y+n, +1,  0) * sobel_y[1][2] +
                            vxImagePixel(vx_uint8, in, 0, x+m, y+n, -1, +1) * sobel_y[2][0] +
                            vxImagePixel(vx_uint8, in, 0, x+m, y+n,  0, +1) * sobel_y[2][1] +
                            vxImagePixel(vx_uint8, in, 0, x+m, y+n, +1, +1) * sobel_y[2][2];
                    vx_int32 value_x = sum_x / div_x;
                    vx_int32 value_y = sum_y / div_y;
                    vxImagePixel(vx_int16, out0, 0, x+m, y+n, 0, 0) = vx_clamp_s16_i32(value_x);
                    vxImagePixel(vx_int16, out1, 0, x+m, y+n, 0, 0) = vx_clamp_s16_i32(value_y);
                    //printf("vx_int32/16 values: %d, %d\n", sum_x, vx_clamp_s16_i32(value_x));
               //     vxImagePixel(vx_uint8, out0, 0, x, y, 0, 0) = vx_clamp_u8_i32(value_x);
               //     vxImagePixel(vx_uint8, out1, 0, x, y, 0, 0) = vx_clamp_u8_i32(value_y);
                }
            }
        }
    }
}
//! [sobel_tiling_function]
