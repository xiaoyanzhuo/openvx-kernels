#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <VX/vx.h>
#include <VX/vx_helper.h>
#include <VX/vx_lib_debug.h>
#include <VX/vx_compatibility.h>
#include <VX/vx_types.h>
#include <assert.h>
#define img_w 512
#define img_h 512

int main(int argc, char *argv[])
{
    vx_uint32 th = 40;
    vx_status status = VX_SUCCESS;
    vx_context context = vxCreateContext();
    if (vxGetStatus((vx_reference)context) == VX_SUCCESS)
    {
        vx_graph graph = vxCreateGraph(context);
        vx_rectangle_t rect = {1, 1, img_w+1, img_h+1}; // 512x512
        vx_uint32 i = 0;
        vx_image images[] = {
                vxCreateImage(context, img_w+2, img_h+2, VX_DF_IMAGE_U8), // 0:input
                vxCreateImageFromROI(images[0], &rect),       // 1:ROI input
                vxCreateVirtualImage(graph, 0, 0, VX_DF_IMAGE_S16),  // 2:grad_x
                vxCreateVirtualImage(graph, 0, 0, VX_DF_IMAGE_S16),  // 3:grad_y
                vxCreateVirtualImage(graph, 0, 0, VX_DF_IMAGE_S16), // 4:magnitude
                vxCreateImage(context, img_w, img_h, VX_DF_IMAGE_U8),   // 5: threshold
        };
        vx_threshold threshold = vxCreateThreshold(context, VX_THRESHOLD_TYPE_BINARY, VX_TYPE_UINT8);
        vxSetThresholdAttribute(threshold, VX_THRESHOLD_ATTRIBUTE_THRESHOLD_VALUE, &th, sizeof(th));
        status |= vxLoadKernels(context, "openvx-debug");
        if (status == VX_SUCCESS)
        {
//            vx_graph graph = vxCreateGraph(context);
            if (vxGetStatus((vx_reference)graph) == VX_SUCCESS)
            {
                vx_node nodes[] = {
                    vxFReadImageNode(graph, "lena_512x512.pgm", images[1]),
                    vxSobel3x3Node(graph, images[1], images[2], images[3]),
                    vxMagnitudeNode(graph, images[2], images[3], images[4]),
                    vxThresholdNode(graph, images[4], threshold, images[5]),
                    vxFWriteImageNode(graph, images[5], "non_tiling_threshold_lena_512x512.pgm"),
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
