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
 * \example vx_tiling_channel_extract.c
 */

// Not sure how passing arguments here should be modified if needed for tiling. Everything that returned something is now void to just perform the algorithm.
void vxCopyPlaneToImage(vx_image src,
                        vx_uint32 src_plane,
                        vx_uint8 src_component,
                        vx_uint32 x_subsampling,
                        vx_image dst)
{
    void *src_base = NULL;
    void *dst_base = NULL;
    vx_imagepatch_addressing_t src_addr = {0};
    vx_imagepatch_addressing_t dst_addr = {0};
    vx_rectangle_t src_rect, dst_rect;
    vx_uint32 x, y;
    vx_status status = VX_SUCCESS;

    status = vxGetValidRegionImage(src, &src_rect);
    status |= vxAccessImagePatch(src, &src_rect, src_plane, &src_addr, &src_base, VX_READ_ONLY);
    if (status == VX_SUCCESS)
    {
        dst_rect = src_rect;
        dst_rect.start_x /= VX_SCALE_UNITY / src_addr.scale_x * x_subsampling;
        dst_rect.start_y /= VX_SCALE_UNITY / src_addr.scale_y;
        dst_rect.end_x /= VX_SCALE_UNITY / src_addr.scale_x * x_subsampling;
        dst_rect.end_y /= VX_SCALE_UNITY / src_addr.scale_y;

        status = vxAccessImagePatch(dst, &dst_rect, 0, &dst_addr, &dst_base, VX_WRITE_ONLY);
        if (status == VX_SUCCESS)
        {
            for (y = 0; y < dst_addr.dim_y; y++)
            {
                for (x = 0; x < dst_addr.dim_x; x++)
                {
                    vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base,
                        x * VX_SCALE_UNITY / src_addr.scale_x * x_subsampling,
                        y * VX_SCALE_UNITY / src_addr.scale_y,
                        &src_addr);
                    vx_uint8 *dstp = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    *dstp = srcp[src_component];
                }
            }
            vxCommitImagePatch(dst, &dst_rect, 0, &dst_addr, dst_base);
        }
        vxCommitImagePatch(src, NULL, src_plane, &src_addr, src_base);
    }
}

void channel_extract_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT],
                      void * VX_RESTRICT tile_memory,
                      vx_size tile_memory_size)
{
    vx_uint32 x, y;
    vx_tile_t *in = (vx_tile_t *)parameters[0];
    vx_scalar *channel = (vx_tile_t *)parameters[1];
    vx_tile_t *out = (vx_tile_t *)parameters[2];

    printf("TileHeight: %d \n", vxTileHeight(out,0));
    printf("TileWidth: %d \n", vxTileWidth(out,0));
    printf("TileBlockHeight: %d \n",vxTileBlockHeight(out));
    printf("TileBlockWidth: %d \n",vxTileBlockWidth(out));

    for (y = 0; y < vxTileHeight(out, 0); y+=vxTileBlockHeight(out))
    {
        for (x = 0; x < vxTileWidth(out, 0); x+=vxTileBlockWidth(out))
        {
            vx_enum chan = -1;
            vx_df_image format = 0;
            vx_uint32 cidx = 0;
            //vx_status status = VX_ERROR_INVALID_PARAMETERS;

            vxAccessScalarValue((*channel), &chan); // Access channel similar to vx_tiling_alpha for accessing vx_float32 *alpha argument (does not use vxImagePixel for non-image arguments)
            vxQueryImage(vxImagePixel(uint8, in, 0, i, j, 0, 0), VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));

            switch (chan)
            {
                case VX_CHANNEL_0:
                    cidx = 0; break;
                case VX_CHANNEL_1:
                    cidx = 1; break;
                case VX_CHANNEL_2:
                    cidx = 2; break;
                case VX_CHANNEL_3:
                    cidx = 3; break;
                default:
                    return VX_ERROR_INVALID_PARAMETERS;
            }

            switch (format)
            {
                case VX_DF_IMAGE_RGB:
                case VX_DF_IMAGE_RGBX:
                    vxCopyPlaneToImage(vxImagePixel(uint8, in, 0, i, j, 0, 0), 0, cidx, 1, vxImagePixel(vx_uint8, out, 0, i, j, 0, 0));
                    break;
                case VX_DF_IMAGE_NV12:
                case VX_DF_IMAGE_NV21:
                    if (chan == VX_CHANNEL_Y)
                        vxCopyPlaneToImage(vxImagePixel(uint8, in, 0, i, j, 0, 0), 0, 0, 1, vxImagePixel(vx_uint8, out, 0, i, j, 0, 0));
                    else if ((chan == VX_CHANNEL_U && format == VX_DF_IMAGE_NV12) ||
                            (chan == VX_CHANNEL_V && format == VX_DF_IMAGE_NV21))
                        vxCopyPlaneToImage(vxImagePixel(uint8, in, 0, i, j, 0, 0), 1, 0, 1, vxImagePixel(vx_uint8, out, 0, i, j, 0, 0));
                    else
                        vxCopyPlaneToImage(vxImagePixel(uint8, in, 0, i, j, 0, 0), 1, 1, 1, vxImagePixel(vx_uint8, out, 0, i, j, 0, 0));
                    break;
                case VX_DF_IMAGE_IYUV:
                case VX_DF_IMAGE_YUV4:
                    vxCopyPlaneToImage(vxImagePixel(uint8, in, 0, i, j, 0, 0), cidx, 0, 1, vxImagePixel(vx_uint8, out, 0, i, j, 0, 0));
                    break;
                case VX_DF_IMAGE_YUYV:
                case VX_DF_IMAGE_UYVY:
                    if (chan == VX_CHANNEL_Y)
                        vxCopyPlaneToImage(vxImagePixel(uint8, in, 0, i, j, 0, 0), 0, format == VX_DF_IMAGE_YUYV ? 0 : 1, 1, vxImagePixel(vx_uint8, out, 0, i, j, 0, 0));
                    else
                        vxCopyPlaneToImage(src, 0,
                            (format == VX_DF_IMAGE_YUYV ? 1 : 0) + (chan == VX_CHANNEL_U ? 0 : 2),
                            2, vxImagePixel(vx_uint8, out, 0, i, j, 0, 0));
                    break;
            }
        }
    }
}
