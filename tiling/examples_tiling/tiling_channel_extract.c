/*
 * Copyright (c) 2013-2017 The Khronos Group Inc.
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

#include <stdio.h>
#include <VX/vx.h>
#include <VX/vx_khr_tiling.h>
#include <VX/vx_lib_debug.h>
#include <VX/vx_helper.h>
#include "vx_tiling_ext.h"
#define img_w 512
#define img_h 512
/*! \file
 * \brief An example of how to call the tiling nodes.
 * \example vx_tiling_main.c
 */

vx_node vxTilingChannelExtractNode(vx_graph graph, vx_image in, vx_scalar channel, vx_image out) // Not sure how to include scalar as a param properly
{
    vx_reference params[] = {
        (vx_reference)in,
        (vx_reference)channel,
        (vx_reference)out,
    };
    vx_node node = vxCreateNodeByStructure(graph,
                                    VX_KERNEL_CHANNEL_EXTRACT_MxN_TILING,
                                    params,
                                    dimof(params));
    return node;
}

int main(int argc, char *argv[])
{
    vx_status status = VX_SUCCESS;
    vx_context context = vxCreateContext();
    if (vxGetStatus((vx_reference)context) == VX_SUCCESS)
    {
        vx_rectangle_t rect = {1, 1, img_w+1, img_h+1}; // 512x512
        vx_uint32 i = 0;
        vx_image images[] = {
                vxCreateImage(context, img_w+2, img_h+2, VX_DF_IMAGE_RGB), // 0:input
                vxCreateImageFromROI(images[0], &rect),       // 1:ROI input
                vxCreateImage(context, img_w, img_h, VX_DF_IMAGE_U8), //2:r
                vxCreateImage(context, img_w, img_h, VX_DF_IMAGE_U8), //3:g
                vxCreateImage(context, img_w, img_h, VX_DF_IMAGE_U8), //4:b
        };

        status |= vxLoadKernels(context, "openvx-tiling");
        status |= vxLoadKernels(context, "openvx-debug");
        if (status == VX_SUCCESS)
        {
            vx_graph graph = vxCreateGraph(context);
            if (vxGetStatus((vx_reference)graph) == VX_SUCCESS)
            {
                vx_node nodes[] = {
                    vxFReadImageNode(graph, "lena_512x512.pgm", images[1]),
                    vxTilingChannelExtractNode(graph, images[1], VX_CHANNEL_R, images[2]),
                    vxTilingChannelExtractNode(graph, images[1], VX_CHANNEL_G, images[3]),
                    vxTilingChannelExtractNode(graph, images[1], VX_CHANNEL_B, images[4]),
                    vxFWriteImageNode(graph, images[2], "tiling_channel_extract_r_lena_512x512.pgm"),
                    vxFWriteImageNode(graph, images[3], "tiling_channel_extract_g_lena_512x512.pgm"),
                    vxFWriteImageNode(graph, images[4], "tiling_channel_extract_b_lena_512x512.pgm"),
                };
                for (i = 0; i < dimof(nodes); i++)
                {
                    if (nodes[i] == 0)
                    {
                        printf("Failed to create node[%u]\n", i);
                        status = VX_ERROR_INVALID_NODE;
                        break;
                    }
                }
                if (status == VX_SUCCESS)
                {
                    status = vxVerifyGraph(graph);
                    if (status == VX_SUCCESS)
                    {
                        status = vxProcessGraph(graph);
                    }
                }
                for (i = 0; i < dimof(nodes); i++)
                {
                    vxReleaseNode(&nodes[i]);
                }
                vxReleaseGraph(&graph);
            }
        }
        // status |= vxUnloadKernels(context, "openvx-debug");
        // status |= vxUnloadKernels(context, "openvx-tiling");
        for (i = 0; i < dimof(images); i++)
        {
            vxReleaseImage(&images[i]);
        }
        vxReleaseContext(&context);
    }
    printf("%s::main() returns = %d\n", argv[0], status);
    return (int)status;
}
