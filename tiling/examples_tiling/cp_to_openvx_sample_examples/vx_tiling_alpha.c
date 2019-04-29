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
 * \example vx_tiling_alpha.c
 */

/*! \brief A 1x1 to 1x1 tile size.
 * \details This tiling kernel doesn't have an inner set of block loops as the
 * block size is 1x1.
 * This kernel uses this tiling definition.
 * \code
 * vx_block_size_t outSize = {1,1};
 * vx_neighborhood_size_t inNbhd = {0,0,0,0};
 * \endcode
 * \ingroup group_tiling
 */
//! [alpha_tiling_function]
void alpha_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT],
                        void * VX_RESTRICT tile_memory,
                        vx_size tile_memory_size)
{
    vx_uint32 i,j;
    vx_uint32 m,n;
    vx_tile_t *in = (vx_tile_t *)parameters[0];
    vx_float32 *alpha = (vx_float32 *)parameters[1];
    vx_tile_t *out = (vx_tile_t *)parameters[2];
    
    printf("TileHeight: %d \n", vxTileHeight(in, 0));
    printf("TileWidth: %d \n", vxTileWidth(in, 0));
    printf("TileBlockHeight: %d \n",vxTileBlockHeight(in));
    printf("TileBlockWidth: %d \n",vxTileBlockWidth(in));
    for (j = 0u; j < vxTileHeight(in, 0); j+=vxTileBlockHeight(in))
    {
        for (i = 0u; i < vxTileWidth(in, 0); i+=vxTileBlockWidth(in))
        {
            for (n = 0u; n < vxTileBlockHeight(in); n++)
            {
                for (m = 0u; m < vxTileBlockWidth(in); m++)
                {
                    vx_uint8 pixel = vxImagePixel(vx_uint8, in, 0, i+m, j+n, 0, 0);
                    vx_uint8 coeff = (vx_uint8)(255.0f * (*alpha));
                    vx_uint16 value = (coeff * pixel) >> 8;
                    if (value > 255)
                        value = 255;
                    vxImagePixel(vx_uint8, out, 0, i+m, j+n, 0, 0) = (vx_uint8)value;
                }
            }
            
        }
    }

}
//! [alpha_tiling_function]
