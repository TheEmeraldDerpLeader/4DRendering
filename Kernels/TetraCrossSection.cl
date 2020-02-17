typedef struct
{
	//Typically read where columns are unit vectors
	float4 col1;
	float4 col2;
	float4 col3;
	float4 col4;
} Mat4;

typedef struct
{
	float4 point;
	float4 direction;
	float3 texCoords; //alignment 4
	float3 texDirection; //alignment 4
} Line;

// Work item for each tetra, but they share model matrices and offsets
// Tetras use modelID to get matrix and offset. Tetras use tetraID to get their lines
// Model lines is premade
// 6 lines per tetra are made and crosected to each be put into their own work items.

__kernel void CrossSection(__global Mat4* modelMatrices, __global float4* modelOffsets, //1 per model
	__global int* modelIDs, __global int* tetraIDs, //1 per tetra
	__global float4* outPoints, __global char* outStates, __global float3* outTexCoords, //6 per tetra
	__constant Mat4* cameraMatrix, __constant float4* cameraOffset, //1
	__constant Line* modelLines)

{ //glm matrices are stored by down rows then right columns
	//Setting up local variables
	int thisID = get_global_id(0);
	int thisModelID = modelIDs[thisID];
	int thisTetraID = tetraIDs[thisID];
	Mat4 rotationMatrix = *cameraMatrix;
	Mat4 modelMatrix = modelMatrices[thisModelID];
	Line tetraLines[6] = {modelLines[(thisTetraID * 6) + 0], modelLines[(thisTetraID * 6) + 1], modelLines[(thisTetraID * 6) + 2], modelLines[(thisTetraID * 6) + 3], modelLines[(thisTetraID * 6) + 4], modelLines[(thisTetraID * 6) + 5]};
	float4 offset = *cameraOffset - modelOffsets[thisModelID];

	for (int i = 0; i < 6; i++)
	{
		float4 posVec;
		posVec.x = (tetraLines[i].point.x * modelMatrix.col1.x) + (tetraLines[i].point.y * modelMatrix.col2.x) + (tetraLines[i].point.z * modelMatrix.col3.x) + (tetraLines[i].point.w * modelMatrix.col4.x);
		posVec.y = (tetraLines[i].point.x * modelMatrix.col1.y) + (tetraLines[i].point.y * modelMatrix.col2.y) + (tetraLines[i].point.z * modelMatrix.col3.y) + (tetraLines[i].point.w * modelMatrix.col4.y);
		posVec.z = (tetraLines[i].point.x * modelMatrix.col1.z) + (tetraLines[i].point.y * modelMatrix.col2.z) + (tetraLines[i].point.z * modelMatrix.col3.z) + (tetraLines[i].point.w * modelMatrix.col4.z);
		posVec.w = (tetraLines[i].point.x * modelMatrix.col1.w) + (tetraLines[i].point.y * modelMatrix.col2.w) + (tetraLines[i].point.z * modelMatrix.col3.w) + (tetraLines[i].point.w * modelMatrix.col4.w);

		float4 dirVec;
		//Rotating the first direction vector to model space
		dirVec.x = (tetraLines[i].direction.x * modelMatrix.col1.x) + (tetraLines[i].direction.y * modelMatrix.col2.x) + (tetraLines[i].direction.z * modelMatrix.col3.x) + (tetraLines[i].direction.w * modelMatrix.col4.x);
		dirVec.y = (tetraLines[i].direction.x * modelMatrix.col1.y) + (tetraLines[i].direction.y * modelMatrix.col2.y) + (tetraLines[i].direction.z * modelMatrix.col3.y) + (tetraLines[i].direction.w * modelMatrix.col4.y);
		dirVec.z = (tetraLines[i].direction.x * modelMatrix.col1.z) + (tetraLines[i].direction.y * modelMatrix.col2.z) + (tetraLines[i].direction.z * modelMatrix.col3.z) + (tetraLines[i].direction.w * modelMatrix.col4.z);
		dirVec.w = (tetraLines[i].direction.x * modelMatrix.col1.w) + (tetraLines[i].direction.y * modelMatrix.col2.w) + (tetraLines[i].direction.z * modelMatrix.col3.w) + (tetraLines[i].direction.w * modelMatrix.col4.w);

		posVec.x -= offset.x;
		posVec.y -= offset.y;
		posVec.z -= offset.z;
		posVec.w -= offset.w;

		tetraLines[i].point = posVec;
		tetraLines[i].direction = dirVec;

		//Rotating the point so the camera is at 0,0,0 facing OpenGL viewport
		posVec.x = (tetraLines[i].point.x * rotationMatrix.col1.x) + (tetraLines[i].point.y * rotationMatrix.col2.x) + (tetraLines[i].point.z * rotationMatrix.col3.x) + (tetraLines[i].point.w * rotationMatrix.col4.x);
		posVec.y = (tetraLines[i].point.x * rotationMatrix.col1.y) + (tetraLines[i].point.y * rotationMatrix.col2.y) + (tetraLines[i].point.z * rotationMatrix.col3.y) + (tetraLines[i].point.w * rotationMatrix.col4.y);
		posVec.z = (tetraLines[i].point.x * rotationMatrix.col1.z) + (tetraLines[i].point.y * rotationMatrix.col2.z) + (tetraLines[i].point.z * rotationMatrix.col3.z) + (tetraLines[i].point.w * rotationMatrix.col4.z);
		posVec.w = (tetraLines[i].point.x * rotationMatrix.col1.w) + (tetraLines[i].point.y * rotationMatrix.col2.w) + (tetraLines[i].point.z * rotationMatrix.col3.w) + (tetraLines[i].point.w * rotationMatrix.col4.w);

		//Rotating the first direction vector to camera space
		dirVec.x = (tetraLines[i].direction.x * rotationMatrix.col1.x) + (tetraLines[i].direction.y * rotationMatrix.col2.x) + (tetraLines[i].direction.z * rotationMatrix.col3.x) + (tetraLines[i].direction.w * rotationMatrix.col4.x);
		dirVec.y = (tetraLines[i].direction.x * rotationMatrix.col1.y) + (tetraLines[i].direction.y * rotationMatrix.col2.y) + (tetraLines[i].direction.z * rotationMatrix.col3.y) + (tetraLines[i].direction.w * rotationMatrix.col4.y);
		dirVec.z = (tetraLines[i].direction.x * rotationMatrix.col1.z) + (tetraLines[i].direction.y * rotationMatrix.col2.z) + (tetraLines[i].direction.z * rotationMatrix.col3.z) + (tetraLines[i].direction.w * rotationMatrix.col4.z);
		dirVec.w = (tetraLines[i].direction.x * rotationMatrix.col1.w) + (tetraLines[i].direction.y * rotationMatrix.col2.w) + (tetraLines[i].direction.z * rotationMatrix.col3.w) + (tetraLines[i].direction.w * rotationMatrix.col4.w);

		//Cross-sectioning the lines with identity 2-space
		if (dirVec.w <= 0.0001f && dirVec.w >= -0.0001)
		{
			outStates[(thisID*6) + i] = -1;
		}
		else
		{
			float magnitude = -posVec.w/dirVec.w; //I've rechecked multiple times, it should be negetive ;)
			if (magnitude >= 0 && magnitude <= 1)
			{
				posVec += magnitude * dirVec;
				outStates[(thisID*6) + i] = 0;
				outPoints[(thisID*6) + i] = posVec;
				outTexCoords[(thisID*6) + i] = tetraLines[i].texCoords + (magnitude * tetraLines[i].texDirection);
			}
			else
			{
				outStates[(thisID*6) + i] = -1;
			}
		}
	}
}