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
 * \example vx_tiling_channel_combine.c
 */

void channel_combine_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT],
                      void * VX_RESTRICT tile_memory,
                      vx_size tile_memory_size)
{
    vx_uint32 x, y;
    vx_tile_t *in0 = (vx_tile_t *)parameters[0];
    vx_tile_t *in1 = (vx_tile_t *)parameters[1];
    vx_tile_t *in2 = (vx_tile_t *)parameters[2];
    vx_tile_t *in3 = (vx_tile_t *)parameters[3];
    vx_tile_t *out = (vx_tile_t *)parameters[4];

    printf("TileHeight: %d \n", vxTileHeight(out,0));
    printf("TileWidth: %d \n", vxTileWidth(out,0));
    printf("TileBlockHeight: %d \n",vxTileBlockHeight(out));
    printf("TileBlockWidth: %d \n",vxTileBlockWidth(out));

    for (y = 0; y < vxTileHeight(out, 0); y+=vxTileBlockHeight(out))
    {
        for (x = 0; x < vxTileWidth(out, 0); x+=vxTileBlockWidth(out))
        {
            vx_df_image format = 0;
            vx_rectangle_t rect;
            vxQueryImage(vxImagePixel(vx_uint8, out, 0, i, j, 0, 0), VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
            vxGetValidRegionImage(vxImagePixel(vx_uint8, in0, 0, i, j, 0, 0), &rect);
            if ((format == VX_DF_IMAGE_RGB) || (format == VX_DF_IMAGE_RGBX))
            {
                /* write all the channels back out in interleaved format */
                vx_imagepatch_addressing_t src_addrs[4];
                vx_imagepatch_addressing_t dst_addr;
                void *base_src_ptrs[4] = {NULL, NULL, NULL, NULL};
                void *base_dst_ptr = NULL;
                uint32_t x, y, p;
                uint32_t numplanes = 3;

                if (format == VX_DF_IMAGE_RGBX)
                {
                    numplanes = 4;
                }

                // get the planes
                vxAccessImagePatch(vxImagePixel(vx_uint8, in0, 0, i, j, 0, 0), &rect, 0, &src_addrs[0], &base_src_ptrs[0], VX_READ_ONLY);
                vxAccessImagePatch(vxImagePixel(vx_uint8, in1, 0, i, j, 0, 0), &rect, 0, &src_addrs[1], &base_src_ptrs[1], VX_READ_ONLY);
                vxAccessImagePatch(vxImagePixel(vx_uint8, in2, 0, i, j, 0, 0), &rect, 0, &src_addrs[2], &base_src_ptrs[2], VX_READ_ONLY);
                if (numplanes == 4)
                    vxAccessImagePatch(vxImagePixel(vx_uint8, in3, 0, i, j, 0, 0), &rect, 0, &src_addrs[3], &base_src_ptrs[3], VX_READ_ONLY);

                vxAccessImagePatch(vxImagePixel(vx_uint8, out, 0, i, j, 0, 0), &rect, 0, &dst_addr, &base_dst_ptr, VX_WRITE_ONLY);
                for (y = 0; y < dst_addr.dim_y; y+=dst_addr.step_y)
                {
                    for (x = 0; x < dst_addr.dim_x; x+=dst_addr.step_x)
                    {
                        uint8_t *planes[4] = {
                            vxFormatImagePatchAddress2d(base_src_ptrs[0], x, y, &src_addrs[0]),
                            vxFormatImagePatchAddress2d(base_src_ptrs[1], x, y, &src_addrs[1]),
                            vxFormatImagePatchAddress2d(base_src_ptrs[2], x, y, &src_addrs[2]),
                            NULL,
                        };
                        uint8_t *dst = vxFormatImagePatchAddress2d(base_dst_ptr, x, y, &dst_addr);
                        dst[0] = planes[0][0];
                        dst[1] = planes[1][0];
                        dst[2] = planes[2][0];
                        if (format == VX_DF_IMAGE_RGBX)
                        {
                            planes[3] = vxFormatImagePatchAddress2d(base_src_ptrs[3], x, y, &src_addrs[3]);
                            dst[3] = planes[3][0];
                        }
                    }
                }
                // write the data back
                vxCommitImagePatch(vxImagePixel(vx_uint8, out, 0, i, j, 0, 0), &rect, 0, &dst_addr, base_dst_ptr);
                // release the planes
                vxCommitImagePatch(vxImagePixel(vx_uint8, in0, 0, i, j, 0, 0), 0, 0, &src_addrs[0], &base_src_ptrs[0]);
                vxCommitImagePatch(vxImagePixel(vx_uint8, in1, 0, i, j, 0, 0), 0, 0, &src_addrs[1], &base_src_ptrs[2]);
                vxCommitImagePatch(vxImagePixel(vx_uint8, in2, 0, i, j, 0, 0), 0, 0, &src_addrs[2], &base_src_ptrs[2]);
                if (numplanes == 4)
                    vxCommitImagePatch(vxImagePixel(vx_uint8, in3, 0, i, j, 0, 0), 0, 0, &src_addrs[3], &base_src_ptrs[3]);
            }
            else if ((format == VX_DF_IMAGE_YUV4) || (format == VX_DF_IMAGE_IYUV))
            {
                uint32_t x, y, p;

                for (p = 0; p < 3; p++)
                {
                    vx_imagepatch_addressing_t src_addr;
                    vx_imagepatch_addressing_t dst_addr;
                    void *base_src_ptr = NULL;
                    void *base_dst_ptr = NULL;

                    // get the plane
                    if(p==0)
                        vxAccessImagePatch(vxImagePixel(vx_uint8, in0, 0, i, j, 0, 0), &rect, 0, &src_addr, &base_src_ptr, VX_READ_ONLY);
                    else if(p==1)
                        vxAccessImagePatch(vxImagePixel(vx_uint8, in1, 0, i, j, 0, 0), &rect, 0, &src_addr, &base_src_ptr, VX_READ_ONLY);
                    else if(p==2)
                        vxAccessImagePatch(vxImagePixel(vx_uint8, in2, 0, i, j, 0, 0), &rect, 0, &src_addr, &base_src_ptr, VX_READ_ONLY);
                    vxAccessImagePatch(vxImagePixel(vx_uint8, out, 0, i, j, 0, 0), &rect, p, &dst_addr, &base_dst_ptr, VX_WRITE_ONLY);

                    for (y = 0; y < dst_addr.dim_y; y += dst_addr.step_y)
                    {
                        for (x = 0; x < dst_addr.dim_x; x += dst_addr.step_x)
                        {
                            uint32_t x1 = x * src_addr.step_x / dst_addr.step_x;
                            uint32_t y1 = y * src_addr.step_y / dst_addr.step_y;
                            uint8_t *src = vxFormatImagePatchAddress2d(base_src_ptr, x1, y1, &src_addr);
                            uint8_t *dst = vxFormatImagePatchAddress2d(base_dst_ptr, x, y, &dst_addr);
                            *dst = *src;
                        }
                    }

                    // write the data back
                    vxCommitImagePatch(vxImagePixel(vx_uint8, out, 0, i, j, 0, 0), &rect, p, &dst_addr, base_dst_ptr);
                    // release the input
                    if(p==0)
                        vxCommitImagePatch(vxImagePixel(vx_uint8, in0, 0, i, j, 0, 0), 0, 0, &src_addr, &base_src_ptr);
                    else if(p==1)   
                        vxCommitImagePatch(vxImagePixel(vx_uint8, in1, 0, i, j, 0, 0), 0, 0, &src_addr, &base_src_ptr);
                    else if(p==2)
                        vxCommitImagePatch(vxImagePixel(vx_uint8, in2, 0, i, j, 0, 0), 0, 0, &src_addr, &base_src_ptr);
                }
            }
            else if ((format == VX_DF_IMAGE_NV12) || (format == VX_DF_IMAGE_NV21))
            {
                uint32_t x, y;
                int vidx = (format == VX_DF_IMAGE_NV12) ? 1 : 0;

                //plane 0
                {
                    vx_imagepatch_addressing_t src_addr;
                    vx_imagepatch_addressing_t dst_addr;
                    void *base_src_ptr = NULL;
                    void *base_dst_ptr = NULL;

                    // get the plane
                    vxAccessImagePatch(vxImagePixel(vx_uint8, in0, 0, i, j, 0, 0), &rect, 0, &src_addr, &base_src_ptr, VX_READ_ONLY);
                    vxAccessImagePatch(vxImagePixel(vx_uint8, out, 0, i, j, 0, 0), &rect, 0, &dst_addr, &base_dst_ptr, VX_WRITE_ONLY);

                    for (y = 0; y < dst_addr.dim_y; y += dst_addr.step_y)
                    {
                        for (x = 0; x < dst_addr.dim_x; x += dst_addr.step_x)
                        {
                            uint8_t *src = vxFormatImagePatchAddress2d(base_src_ptr, x, y, &src_addr);
                            uint8_t *dst = vxFormatImagePatchAddress2d(base_dst_ptr, x, y, &dst_addr);
                            *dst = *src;
                        }
                    }

                    // write the data back
                    vxCommitImagePatch(vxImagePixel(vx_uint8, out, 0, i, j, 0, 0), &rect, 0, &dst_addr, base_dst_ptr);
                    // release the input
                    vxCommitImagePatch(vxImagePixel(vx_uint8, in0, 0, i, j, 0, 0), 0, 0, &src_addr, &base_src_ptr);
                }

                // plane 1
                {
                    vx_imagepatch_addressing_t src0_addr;
                    vx_imagepatch_addressing_t src1_addr;
                    vx_imagepatch_addressing_t dst_addr;
                    void *base_src0_ptr = NULL;
                    void *base_src1_ptr = NULL;
                    void *base_dst_ptr = NULL;

                    // get the plane
                    vxAccessImagePatch(vxImagePixel(vx_uint8, in1, 0, i, j, 0, 0), &rect, 0, &src0_addr, &base_src0_ptr, VX_READ_ONLY);
                    vxAccessImagePatch(vxImagePixel(vx_uint8, in2, 0, i, j, 0, 0), &rect, 0, &src1_addr, &base_src1_ptr, VX_READ_ONLY);
                    vxAccessImagePatch(vxImagePixel(vx_uint8, out, 0, i, j, 0, 0), &rect, 1, &dst_addr, &base_dst_ptr, VX_WRITE_ONLY);

                    for (y = 0; y < dst_addr.dim_y; y += dst_addr.step_y)
                    {
                        for (x = 0; x < dst_addr.dim_x; x += dst_addr.step_x)
                        {
                            uint32_t x1 = x * src0_addr.step_x / dst_addr.step_x;
                            uint32_t y1 = y * src0_addr.step_y / dst_addr.step_y;
                            uint8_t *src0 = vxFormatImagePatchAddress2d(base_src0_ptr, x1, y1, &src0_addr);
                            uint8_t *src1 = vxFormatImagePatchAddress2d(base_src1_ptr, x1, y1, &src1_addr);
                            uint8_t *dst = vxFormatImagePatchAddress2d(base_dst_ptr, x, y, &dst_addr);
                            dst[1-vidx] = *src0;
                            dst[vidx]   = *src1;
                        }
                    }

                    // write the data back
                    vxCommitImagePatch(vxImagePixel(vx_uint8, out, 0, i, j, 0, 0), &rect, 1, &dst_addr, base_dst_ptr);
                    // release the input
                    vxCommitImagePatch(vxImagePixel(vx_uint8, in1, 0, i, j, 0, 0), 0, 0, &src0_addr, &base_src0_ptr);
                    vxCommitImagePatch(vxImagePixel(vx_uint8, in2, 0, i, j, 0, 0), 0, 0, &src1_addr, &base_src1_ptr);
                }
            }
            else if ((format == VX_DF_IMAGE_YUYV) || (format == VX_DF_IMAGE_UYVY))
            {
                uint32_t x, y;
                int yidx = (format == VX_DF_IMAGE_UYVY) ? 1 : 0;
                vx_imagepatch_addressing_t src0_addr;
                vx_imagepatch_addressing_t src1_addr;
                vx_imagepatch_addressing_t src2_addr;
                vx_imagepatch_addressing_t dst_addr;
                void *base_src0_ptr = NULL;
                void *base_src1_ptr = NULL;
                void *base_src2_ptr = NULL;
                void *base_dst_ptr = NULL;

                vxAccessImagePatch(vxImagePixel(vx_uint8, in0, 0, i, j, 0, 0), &rect, 0, &src0_addr, &base_src0_ptr, VX_READ_ONLY);
                vxAccessImagePatch(vxImagePixel(vx_uint8, in1, 0, i, j, 0, 0), &rect, 0, &src1_addr, &base_src1_ptr, VX_READ_ONLY);
                vxAccessImagePatch(vxImagePixel(vx_uint8, in2, 0, i, j, 0, 0), &rect, 0, &src2_addr, &base_src2_ptr, VX_READ_ONLY);
                vxAccessImagePatch(vxImagePixel(vx_uint8, out, 0, i, j, 0, 0), &rect, 0, &dst_addr, &base_dst_ptr, VX_WRITE_ONLY);

                for (y = 0; y < dst_addr.dim_y; y += dst_addr.step_y)
                {
                    for (x = 0; x < dst_addr.dim_x; x += dst_addr.step_x * 2)
                    {
                        uint32_t x1 = x * src0_addr.step_x / dst_addr.step_x;
                        uint32_t y1 = y * src0_addr.step_y / dst_addr.step_y;
                        uint32_t x2 = x * src1_addr.step_x / (dst_addr.step_x * 2);
                        uint32_t y2 = y * src1_addr.step_y / dst_addr.step_y;
                        uint8_t *srcy0 = vxFormatImagePatchAddress2d(base_src0_ptr, x1, y1, &src0_addr);
                        uint8_t *srcy1 = vxFormatImagePatchAddress2d(base_src0_ptr, x1 + src0_addr.step_x, y1, &src0_addr);
                        uint8_t *srcu = vxFormatImagePatchAddress2d(base_src1_ptr, x2, y2, &src1_addr);
                        uint8_t *srcv = vxFormatImagePatchAddress2d(base_src2_ptr, x2, y2, &src2_addr);
                        uint8_t *dst0 = vxFormatImagePatchAddress2d(base_dst_ptr, x, y, &dst_addr);
                        uint8_t *dst1 = vxFormatImagePatchAddress2d(base_dst_ptr, x + dst_addr.step_x, y, &dst_addr);
                        dst0[yidx] = *srcy0;
                        dst1[yidx] = *srcy1;
                        dst0[1-yidx] = *srcu;
                        dst1[1-yidx] = *srcv;
                    }
                }

                vxCommitImagePatch(vxImagePixel(vx_uint8, out, 0, i, j, 0, 0), &rect, 0, &dst_addr, base_dst_ptr);
                vxCommitImagePatch(vxImagePixel(vx_uint8, in0, 0, i, j, 0, 0), 0, 0, &src0_addr, &base_src0_ptr);
                vxCommitImagePatch(vxImagePixel(vx_uint8, in1, 0, i, j, 0, 0), 0, 0, &src1_addr, &base_src1_ptr);
                vxCommitImagePatch(vxImagePixel(vx_uint8, in2, 0, i, j, 0, 0), 0, 0, &src2_addr, &base_src2_ptr);
            }
        }
    }
}
