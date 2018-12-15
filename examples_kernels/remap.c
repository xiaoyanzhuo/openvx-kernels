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
	vx_uint32 w = 640;
	vx_uint32 h = 480;
	vx_uint32 w2 = 300, h2 = 200, x = 0, y = 0;
	vx_uint32 i;
	// step 1: Create context
	vx_context context = vxCreateContext();
	if (context)
	{
	//vxLoadKernels(context, "openvx-c_model");
		vxLoadKernels(context, "openvx-debug");

	// step 2: Images (in/output)
		vx_image images[] = {
			vxCreateImage(context, w, h, VX_DF_IMAGE_U8),           /* 0: orig_luma */
			vxCreateImage(context, w2, h2, VX_DF_IMAGE_U8),         /* 1: Remapped */
			// vxCreateImage(context, w2, h2, VX_DF_IMAGE_U8),         /* 2: Remapped */
			// vxCreateImage(context, w2, h2, VX_DF_IMAGE_U8),         /* 3: Absdiff remap1vs2 */
		};

		vx_remap table = vxCreateRemap(context, w, h, w2, h2);
		vx_float32 dx = (vx_float32)w/w2, dy = (vx_float32)h/h2;

		/* create the remapping for "image scaling" using remap */
		for (y = 0; y < h2; y++)
		{
		    for (x = 0; x < w2; x++)
		    {
		        vx_float32 nx = dx*x;
		        vx_float32 ny = dy*y;
		        //printf("Setting point %lu, %lu from %lf, %lf (dx,dy=%lf,%lf)\n", x, y, nx, ny, dx, dy);
		        vxSetRemapPoint(table, x, y, nx, ny);
		    }
		}		

		vxuFReadImage(context, "../raw/bikegray_640x480.pgm", images[0]);

	// step 3: Graph init
		vx_graph graph = vxCreateGraph(context);
	
	// step 4: Graph nodes construstion	
		if (graph)
		{
			vx_node nodes[] = {
				vxRemapNode(graph, images[0], table, VX_INTERPOLATION_TYPE_NEAREST_NEIGHBOR, images[1]),   //Remapped 1: nearest neighbor
				// vxRemapNode(graph, images[0], table, VX_INTERPOLATION_TYPE_BILINEAR, images[2]),           //Remapped 2: bilinear
				// vxAbsDiffNode(graph, images[1], images[2], images[3]),                                     //Absdiff between remap1 and remap2 */
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
				vxuFWriteImage(context, images[1], "../raw/remap_1_nn_300x200.pgm"); 
				// vxuFWriteImage(context, images[2], "../raw/remap_2_bi_300x200.pgm");
				// vxuFWriteImage(context, images[3], "../raw/remap_diff_300x200.pgm");
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

