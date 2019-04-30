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
 * \example vx_tiling_phase.c
 */

void phase_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT],
                      void * VX_RESTRICT tile_memory,
                      vx_size tile_memory_size)
{
    vx_uint32 x, y;
    vx_tile_t *in0 = (vx_tile_t *)parameters[0];
    vx_tile_t *in1 = (vx_tile_t *)parameters[1];
    vx_tile_t *out = (vx_tile_t *)parameters[2];

    printf("TileHeight: %d \n", vxTileHeight(out,0));
    printf("TileWidth: %d \n", vxTileWidth(out,0));
    printf("TileBlockHeight: %d \n",vxTileBlockHeight(out));
    printf("TileBlockWidth: %d \n",vxTileBlockWidth(out));

    for (y = 0; y < vxTileHeight(out, 0); y+=vxTileBlockHeight(out))
    {
        for (x = 0; x < vxTileWidth(out, 0); x+=vxTileBlockWidth(out))
        {
            /* -M_PI to M_PI */
            vx_float64 arct = atan2(vxImagePixel(vx_float64, in0, 0, i, j, 0, 0),vxImagePixel(vx_float64, in1, 0, i, j, 0, 0));
            /* 0 - TAU */
            vx_float64 norm = arct;
            if (arct < 0.0f)
            {
                norm = VX_TAU + arct;
            }
            /* 0.0 - 1.0 */
            norm = norm / VX_TAU;
            /* 0 - 255 */
            vxImagePixel(vx_uint8, out, 0, i, j, 0, 0) = (vx_uint8)((vx_uint32)(norm * 256u + 0.5) & 0xFFu);
        }
    }
}
