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

__kernel void CrossSection(__global Mat4* modelMatrices, __global float4* modelOffsets, //1 per penta
	__global int* modelIDs, __global int* tetraIDs, //1 per tetra
	__global float4* outPoints, __global char* outStates, __global float3* outTexCoords, //6 per tetra
	__constant Mat4* cameraMatrix, __constant float4* cameraOffset, //1
	__constant Line* modelLines)

{ //glm matrices are stored by down rows then right columns
	//Setting up local variables
	int thisID = get_global_id(0);
	int thisModelID = modelIDs[thisID];
	int thisTetraID = tetraIDs[thisID];
	Mat4 transformMat;
	Mat4 rotationMatrix = *cameraMatrix;
	Mat4 modelMatrix = modelMatrices[thisModelID];
	Line tetraLines[6] = {modelLines[(thisTetraID * 6) + 0], modelLines[(thisTetraID * 6) + 1], modelLines[(thisTetraID * 6) + 2], modelLines[(thisTetraID * 6) + 3], modelLines[(thisTetraID * 6) + 4], modelLines[(thisTetraID * 6) + 5]};
	float4 offset = *cameraOffset - modelOffsets[thisModelID];

	//multiplying transformMat and modelMatrix 
	transformMat.col1.x = (rotationMatrix.col1.x * modelMatrix.col1.x) + (rotationMatrix.col2.x * modelMatrix.col1.y) + (rotationMatrix.col3.x * modelMatrix.col1.z) + (rotationMatrix.col4.x * modelMatrix.col1.w);
	transformMat.col1.y = (rotationMatrix.col1.y * modelMatrix.col1.x) + (rotationMatrix.col2.y * modelMatrix.col1.y) + (rotationMatrix.col3.y * modelMatrix.col1.z) + (rotationMatrix.col4.y * modelMatrix.col1.w);
	transformMat.col1.z = (rotationMatrix.col1.z * modelMatrix.col1.x) + (rotationMatrix.col2.z * modelMatrix.col1.y) + (rotationMatrix.col3.z * modelMatrix.col1.z) + (rotationMatrix.col4.z * modelMatrix.col1.w);
	transformMat.col1.w = (rotationMatrix.col1.w * modelMatrix.col1.x) + (rotationMatrix.col2.w * modelMatrix.col1.y) + (rotationMatrix.col3.w * modelMatrix.col1.z) + (rotationMatrix.col4.w * modelMatrix.col1.w);
	transformMat.col2.x = (rotationMatrix.col1.x * modelMatrix.col2.x) + (rotationMatrix.col2.x * modelMatrix.col2.y) + (rotationMatrix.col3.x * modelMatrix.col2.z) + (rotationMatrix.col4.x * modelMatrix.col2.w);
	transformMat.col2.y = (rotationMatrix.col1.y * modelMatrix.col2.x) + (rotationMatrix.col2.y * modelMatrix.col2.y) + (rotationMatrix.col3.y * modelMatrix.col2.z) + (rotationMatrix.col4.y * modelMatrix.col2.w);
	transformMat.col2.z = (rotationMatrix.col1.z * modelMatrix.col2.x) + (rotationMatrix.col2.z * modelMatrix.col2.y) + (rotationMatrix.col3.z * modelMatrix.col2.z) + (rotationMatrix.col4.z * modelMatrix.col2.w);
	transformMat.col2.w = (rotationMatrix.col1.w * modelMatrix.col2.x) + (rotationMatrix.col2.w * modelMatrix.col2.y) + (rotationMatrix.col3.w * modelMatrix.col2.z) + (rotationMatrix.col4.w * modelMatrix.col2.w);
	transformMat.col3.x = (rotationMatrix.col1.x * modelMatrix.col3.x) + (rotationMatrix.col2.x * modelMatrix.col3.y) + (rotationMatrix.col3.x * modelMatrix.col3.z) + (rotationMatrix.col4.x * modelMatrix.col3.w);
	transformMat.col3.y = (rotationMatrix.col1.y * modelMatrix.col3.x) + (rotationMatrix.col2.y * modelMatrix.col3.y) + (rotationMatrix.col3.y * modelMatrix.col3.z) + (rotationMatrix.col4.y * modelMatrix.col3.w);
	transformMat.col3.z = (rotationMatrix.col1.z * modelMatrix.col3.x) + (rotationMatrix.col2.z * modelMatrix.col3.y) + (rotationMatrix.col3.z * modelMatrix.col3.z) + (rotationMatrix.col4.z * modelMatrix.col3.w);
	transformMat.col3.w = (rotationMatrix.col1.w * modelMatrix.col3.x) + (rotationMatrix.col2.w * modelMatrix.col3.y) + (rotationMatrix.col3.w * modelMatrix.col3.z) + (rotationMatrix.col4.w * modelMatrix.col3.w);
	transformMat.col4.x = (rotationMatrix.col1.x * modelMatrix.col4.x) + (rotationMatrix.col2.x * modelMatrix.col4.y) + (rotationMatrix.col3.x * modelMatrix.col4.z) + (rotationMatrix.col4.x * modelMatrix.col4.w);
	transformMat.col4.y = (rotationMatrix.col1.y * modelMatrix.col4.x) + (rotationMatrix.col2.y * modelMatrix.col4.y) + (rotationMatrix.col3.y * modelMatrix.col4.z) + (rotationMatrix.col4.y * modelMatrix.col4.w);
	transformMat.col4.z = (rotationMatrix.col1.z * modelMatrix.col4.x) + (rotationMatrix.col2.z * modelMatrix.col4.y) + (rotationMatrix.col3.z * modelMatrix.col4.z) + (rotationMatrix.col4.z * modelMatrix.col4.w);
	transformMat.col4.w = (rotationMatrix.col1.w * modelMatrix.col4.x) + (rotationMatrix.col2.w * modelMatrix.col4.y) + (rotationMatrix.col3.w * modelMatrix.col4.z) + (rotationMatrix.col4.w * modelMatrix.col4.w);

	for (int i = 0; i < 6; i++)
	{
		tetraLines[i].point.x -= offset.x;
		tetraLines[i].point.y -= offset.y;
		tetraLines[i].point.z -= offset.z;
		tetraLines[i].point.w -= offset.w;

		float4 posVec;
		//Rotating the point so the camera is at 0,0,0 facing OpenGL viewport
		posVec.x = (tetraLines[i].point.x * transformMat.col1.x) + (tetraLines[i].point.y * transformMat.col2.x) + (tetraLines[i].point.z * transformMat.col3.x) + (tetraLines[i].point.w * transformMat.col4.x);
		posVec.y = (tetraLines[i].point.x * transformMat.col1.y) + (tetraLines[i].point.y * transformMat.col2.y) + (tetraLines[i].point.z * transformMat.col3.y) + (tetraLines[i].point.w * transformMat.col4.y);
		posVec.z = (tetraLines[i].point.x * transformMat.col1.z) + (tetraLines[i].point.y * transformMat.col2.z) + (tetraLines[i].point.z * transformMat.col3.z) + (tetraLines[i].point.w * transformMat.col4.z);
		posVec.w = (tetraLines[i].point.x * transformMat.col1.w) + (tetraLines[i].point.y * transformMat.col2.w) + (tetraLines[i].point.z * transformMat.col3.w) + (tetraLines[i].point.w * transformMat.col4.w);

		float4 dirVec;
		//Rotating the first direction vector to camera space
		dirVec.x = (tetraLines[i].direction.x * transformMat.col1.x) + (tetraLines[i].direction.y * transformMat.col2.x) + (tetraLines[i].direction.z * transformMat.col3.x) + (tetraLines[i].direction.w * transformMat.col4.x);
		dirVec.y = (tetraLines[i].direction.x * transformMat.col1.y) + (tetraLines[i].direction.y * transformMat.col2.y) + (tetraLines[i].direction.z * transformMat.col3.y) + (tetraLines[i].direction.w * transformMat.col4.y);
		dirVec.z = (tetraLines[i].direction.x * transformMat.col1.z) + (tetraLines[i].direction.y * transformMat.col2.z) + (tetraLines[i].direction.z * transformMat.col3.z) + (tetraLines[i].direction.w * transformMat.col4.z);
		dirVec.w = (tetraLines[i].direction.x * transformMat.col1.w) + (tetraLines[i].direction.y * transformMat.col2.w) + (tetraLines[i].direction.z * transformMat.col3.w) + (tetraLines[i].direction.w * transformMat.col4.w);

		//Cross-sectioning the lines with identity 2-space
		if (dirVec.w <= 0.0001f && dirVec.w >= -0.0001)
		{
			outStates[(thisID*6) + i] = -1;
			/*
			if (posVec.w == 0)
			{
				directionVectors[(thisID*6) + i] = dirVec;
				outStates[(thisID*6) + i] = 1;
			}
			else
			{
				outStates[(thisID*6) + i] = -1;
			}
			*/
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