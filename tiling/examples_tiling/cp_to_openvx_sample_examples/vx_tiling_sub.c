/* 
 * Copyright (c) 2012-2017 The Khronos Group Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <VX/vx_khr_tiling.h>
#include <stdio.h>
/*! \file
 * \example vx_tiling_sub.c
 */

/*! \brief A 1x1 to 1x1 image substract kernel.
 * \details This tiling kernel doesn't have an inner set of block loops as the
 * block size is 1x1.
 * This kernel uses this tiling definition.
 * \code
 * vx_block_size_t outSize = {1,1};
 * vx_neighborhood_size_t inNbhd = {0,0,0,0};
 * \endcode
 * \ingroup group_tiling
 */
//! [sub_tiling_function]
void sub_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT],
                      void * VX_RESTRICT tile_memory,
                      vx_size tile_memory_size)
{
    vx_uint32 i,j;
    vx_uint32 m,n;
    vx_int32 res;
    vx_tile_t *in0 = (vx_tile_t *)parameters[0];
    vx_tile_t *in1 = (vx_tile_t *)parameters[1];
    vx_tile_t *out = (vx_tile_t *)parameters[2];

    printf("TileHeight: %d \n", vxTileHeight(out,0));
    printf("TileWidth: %d \n", vxTileWidth(out,0));
    printf("TileBlockHeight: %d \n",vxTileBlockHeight(out));
    printf("TileBlockWidth: %d \n",vxTileBlockWidth(out));

    for (j = 0u; j < vxTileHeight(out,0); j+=vxTileBlockHeight(out))
    {
        for (i = 0u; i < vxTileWidth(out,0); i+=vxTileBlockWidth(out))
        {
            for (n = 0u; n < vxTileBlockHeight(out); n++)
            {
                for (m = 0u; m < vxTileBlockWidth(out); m++)
                {
                    /* this math covers a 1x1 block and has no neighborhood */
                    vx_int32 pixel = vxImagePixel(vx_uint8, in0, 0, i+m, j+n, 0, 0) -
                                      vxImagePixel(vx_uint8, in1, 0, i+m, j+n, 0, 0);
                    /* this mainly works for unit8 */
                    if (pixel > UINT8_MAX)
                        res = UINT8_MAX;
                    else if (pixel < 0)
                        res = 0;
                    else
                        res = pixel;
                    vxImagePixel(vx_uint8, out, 0, i+m, j+n, 0, 0) = (vx_uint8)res;
                }
            }
        }
    }
}
//! [sub_tiling_function]
