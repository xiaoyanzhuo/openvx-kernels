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
	// step 1: Create context
	vx_context context = vxCreateContext();
	if (context)
	{
	//vxLoadKernels(context, "openvx-c_model");
		vxLoadKernels(context, "openvx-debug");

	// step 2: Images (in/output)
		vx_image images[] = {
			vxCreateImage(context, width, height, VX_DF_IMAGE_U8),   /* 0: input 1 */
			vxCreateImage(context, width, height, VX_DF_IMAGE_U8),   /* 1: input 2 */
			vxCreateImage(context, width, height, VX_DF_IMAGE_U8),   /* 2: output 1 - policy1 */
			vxCreateImage(context, width, height, VX_DF_IMAGE_U8),   /* 3: output 2 - policy2 */
		};

		vx_enum overflow_policy = VX_CONVERT_POLICY_SATURATE;
		vx_enum round_policy_1 = VX_ROUND_POLICY_TO_NEAREST_EVEN;
		vx_enum round_policy_2 = VX_ROUND_POLICY_TO_ZERO;

		vx_float32 scale_1 = 1.0 / 255;
		vx_float32 scale_2 = 1.0 / 128;

		vx_scalar scalars[] = {
		        vxCreateScalar(context, VX_TYPE_FLOAT32, &scale_1),
		        vxCreateScalar(context, VX_TYPE_FLOAT32, &scale_2),
		};		


		vxuFReadImage(context, "../raw/bikegray_640x480.pgm", images[0]);
		vxuFReadImage(context, "../raw/absdiff_640x480.pgm", images[1]);

	// step 3: Graph init
		vx_graph graph = vxCreateGraph(context);
	
	// step 4: Graph nodes construstion	
		if (graph)
		{
			vx_node nodes[] = {
				vxMultiplyNode(graph, images[0], images[1], scalars[0], overflow_policy, round_policy_1, images[2]),
				vxMultiplyNode(graph, images[0], images[1], scalars[1], overflow_policy, round_policy_2, images[3]),
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
				vxuFWriteImage(context, images[2], "../raw/mult_ne_640x480.pgm"); 
				vxuFWriteImage(context, images[3], "../raw/mult_zero_640x480.pgm"); 
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

