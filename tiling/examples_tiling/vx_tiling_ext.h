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

#ifndef _VX_EXAMPLE_TILING_H_
#define _VX_EXAMPLE_TILING_H_

/*! \brief The list of example tiling kernels */
enum vx_tiling_kernels_e {
    /*! \brief a re-implementation of a gaussian blur */
    VX_KERNEL_GAUSSIAN_3x3_TILING = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x100,
    /*! \brief a re-implementation of a generic box filter of any size */
    VX_KERNEL_BOX_MxN_TILING = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x101,
    /*! \brief a re-implementation of a add kernel */
    VX_KERNEL_ADD_TILING = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x102,
    /*! \brief an alpha multiply kernel */
    VX_KERNEL_ALPHA_TILING = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x103,
    /*! \brief an erode kernel */
    VX_KERNEL_ERODE_MxN_TILING = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x104,
    /*! \brief a dilate kernel */
    VX_KERNEL_DILATE_MxN_TILING = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x105,
    /*! \brief a median kernel */
    VX_KERNEL_MEDIAN_MxN_TILING = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x106,
    /*! \brief a sobel kernel */
    VX_KERNEL_SOBEL_MxN_TILING = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x107,
    /*! \brief a absdiff kernel */
    VX_KERNEL_ABSDIFF_TILING = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x108, // Add MXN here for uniformity
    /*! \brief a magnitude kernel */
    VX_KERNEL_MAGNITUDE_MxN_TILING = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x109,
    /*! \brief a phase kernel */
    VX_KERNEL_PHASE_MxN_TILING = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x10A,
    /*! \brief a color convert kernel */
    VX_KERNEL_COLOR_CONVERT_MxN_TILING = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x10B,
    /*! \brief a channel extract kernel */
    VX_KERNEL_CHANNEL_EXTRACT_MxN_TILING = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x10C,
    /*! \brief a channel combine kernel */
    VX_KERNEL_CHANNEL_COMBINE_MxN_TILING = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x10D,
};

#ifdef _VX_TILING_EXT_INTERNAL_
void add_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT], void * VX_RESTRICT tile_memory, vx_size tile_memory_size);
void alpha_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT], void * VX_RESTRICT tile_memory, vx_size tile_memory_size);
void box_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT], void * VX_RESTRICT tile_memory, vx_size tile_memory_size);
void gaussian_image_tiling_fast(void * VX_RESTRICT parameters[VX_RESTRICT], void * VX_RESTRICT tile_memory, vx_size tile_memory_size);
void erode_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT], void * VX_RESTRICT tile_memory, vx_size tile_memory_size);
void dilate_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT], void * VX_RESTRICT tile_memory, vx_size tile_memory_size);
void median_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT], void * VX_RESTRICT tile_memory, vx_size tile_memory_size);
void sobel_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT], void * VX_RESTRICT tile_memory, vx_size tile_memory_size);
void absdiff_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT], void * VX_RESTRICT tile_memory, vx_size tile_memory_size);
void magnitude_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT], void * VX_RESTRICT tile_memory, vx_size tile_memory_size);
void phase_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT], void * VX_RESTRICT tile_memory, vx_size tile_memory_size);
void color_convert_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT], void * VX_RESTRICT tile_memory, vx_size tile_memory_size);
void channel_extract_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT], void * VX_RESTRICT tile_memory, vx_size tile_memory_size);
void channel_combine_image_tiling(void * VX_RESTRICT parameters[VX_RESTRICT], void * VX_RESTRICT tile_memory, vx_size tile_memory_size);
#endif

#endif

