#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <VX/vx.h>
#include <VX/vx_helper.h>
#include <VX/vx_lib_debug.h>
#include <VX/vx_compatibility.h>
#include <VX/vx_types.h>

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
			vxCreateImage(context, width, height, VX_DF_IMAGE_U8),
			vxCreateImage(context, width, height, VX_DF_IMAGE_U8),
		};
		vxuFReadImage(context, "../raw/bikegray_640x480.pgm", images[0]);

	// step 3: Graph init
		vx_graph graph = vxCreateGraph(context);
	
	// step 4: Graph nodes construstion	
		if (graph)
		{
			vx_node node = vxBox3x3Node(graph, images[0], images[1]);

	// step 5: GRAPH VERIFICATION
			status = vxVerifyGraph(graph);
			printf("AFTER VERIFICATION (status: %d)\n", status);

	// step 6: GRAPH EXECUTION
			if (status == VX_SUCCESS)
			{
				status = vxProcessGraph(graph);
			// WRITE RESULT
				vxuFWriteImage(context, images[1], "../raw/box_640x480.pgm");
			        printf("AFTER GRAPH EXECUTION (status: %d)\n", status);	
			}
		}
	}
	// step 7: Release Resource
	vxReleaseContext(&context);
	return status;
}

