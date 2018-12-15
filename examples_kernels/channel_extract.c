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

static void vx_print_log(vx_reference ref)
{
    char message[VX_MAX_LOG_MESSAGE_LEN];
    vx_uint32 errnum = 1;
    vx_status status = VX_SUCCESS;
    do {
        status = vxGetLogEntry(ref, message);
        if (status != VX_SUCCESS)
            printf("[%05u] error=%d %s", errnum++, status, message);
    } while (status != VX_SUCCESS);
}

int main(int argc, char *argv[])
{
	vx_status status = VX_SUCCESS;
	vx_uint32 width = 640;
	vx_uint32 height = 480;
	vx_uint32 i;
	vx_uint32 diffs = 0u;
	// step 1: Create context
	vx_context context = vxCreateContext();
	if (context)
	{
	//vxLoadKernels(context, "openvx-c_model");
		vxLoadKernels(context, "openvx-debug");

	// step 2: Images (in/output)
		vx_image images[] = {
			vxCreateImage(context, width, height, VX_DF_IMAGE_RGB),    /* 0: rgb */
			vxCreateImage(context, width, height, VX_DF_IMAGE_U8),     /* 1: r */
			vxCreateImage(context, width, height, VX_DF_IMAGE_U8),     /* 2: g */
			vxCreateImage(context, width, height, VX_DF_IMAGE_U8),     /* 3: b */
		};
		vxuFReadImage(context, "../raw/colorbars_640x480_I444.rgb", images[0]);

	// step 3: Graph init
		vx_graph graph = vxCreateGraph(context);
	
	// step 4: Graph nodes construstion	
		if (graph)
		{
			vx_node nodes[] = {
				vxChannelExtractNode(graph, images[0], VX_CHANNEL_R, images[1]),
				vxChannelExtractNode(graph, images[0], VX_CHANNEL_G, images[2]),
				vxChannelExtractNode(graph, images[0], VX_CHANNEL_B, images[3]),
			};

			for (i = 0; i < dimof(nodes); i++)
			{
				if (nodes[i] == 0)
				{
					printf("Failed to make nodes[%u]\n",i);
				}
			}

	// step 5: GRAPH VERIFICATION
			status = vxVerifyGraph(graph);
			vx_print_log((vx_reference)graph);
			printf("AFTER VERIFICATION (status: %d)\n", status);
			if (status == VX_SUCCESS)
			{
				status = vxProcessGraph(graph);
				assert(status == VX_SUCCESS);
			}
			else
			{
				vx_print_log((vx_reference)graph);
			}

	// step 6: GRAPH EXECUTION
			if (status == VX_SUCCESS)
			{
				status = vxProcessGraph(graph);
			// WRITE RESULT
				vxuFWriteImage(context, images[1], "../raw/channel_R.bw"); 
				vxuFWriteImage(context, images[2], "../raw/channel_G.bw"); 
				vxuFWriteImage(context, images[3], "../raw/channel_B.bw"); 

				printf("AFTER GRAPH EXECUTION (status: %d)\n", status);
			}
			else
			{
				printf("Graph failed (%d)\n", status);
				for (i = 0; i < dimof(nodes); i++)
				{
					status = VX_SUCCESS;
					vxQueryNode(nodes[i], VX_NODE_ATTRIBUTE_STATUS, &status, sizeof(status));
					if (status != VX_SUCCESS)
					{
					printf("nodes[%u] failed with %d\n", i, status);
					}
				}
				status = VX_ERROR_NOT_SUFFICIENT;
			}
	// step 7: Release: nodes, graph, images, context
			for (i = 0; i < dimof(nodes); i++)
			{
				vxReleaseNode(&nodes[i]);
			}
			vxReleaseGraph(&graph);
		}

		for (i = 0; i < dimof(images); i++)
		{
			vxReleaseImage(&images[i]);
		}
		vxReleaseContext(&context);
	}
	return status;
}

