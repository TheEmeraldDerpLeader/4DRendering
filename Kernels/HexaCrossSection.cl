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

// Work item for each hexa, but they share model matrices and offsets
// hexas use modelID to get matrix and offset. hexas use hexaID to get their lines
// Model lines is premade
// 12 lines per hexa are made and crosected to each be put into their own work items.

__kernel void HexaCrossSection(__global Mat4* modelMatrices, __global float4* modelOffsets, //1 per model
	__global int* modelIDs, __global int* hexaIDs, //1 per hexa
	__global float4* outPoints, __global char* outStates, __global float3* outTexCoords, //12 per hexa
	__constant Mat4* cameraMatrix, __constant float4* cameraOffset, //1
	__constant Line* modelLines)

{ //glm matrices are stored by down rows then right columns
	//Setting up local variables
	int thisID = get_global_id(0);
	int thisModelID = modelIDs[thisID];
	int thisHexaID = hexaIDs[thisID];
	Mat4 rotationMatrix = *cameraMatrix;
	Mat4 modelMatrix = modelMatrices[thisModelID];
	Line hexaLines[12] = {modelLines[(thisHexaID * 12) + 0], modelLines[(thisHexaID * 12) + 1], modelLines[(thisHexaID * 12) + 2], modelLines[(thisHexaID * 12) + 3],
	modelLines[(thisHexaID * 12) + 4], modelLines[(thisHexaID * 12) + 5], modelLines[(thisHexaID * 12) + 6], modelLines[(thisHexaID * 12) + 7],
	modelLines[(thisHexaID * 12) + 8], modelLines[(thisHexaID * 12) + 9], modelLines[(thisHexaID * 12) + 10], modelLines[(thisHexaID * 12) + 11]};
	float4 offset = *cameraOffset - modelOffsets[thisModelID];

	for (int i = 0; i < 12; i++)
	{
		float4 posVec;
		posVec.x = (hexaLines[i].point.x * modelMatrix.col1.x) + (hexaLines[i].point.y * modelMatrix.col2.x) + (hexaLines[i].point.z * modelMatrix.col3.x) + (hexaLines[i].point.w * modelMatrix.col4.x);
		posVec.y = (hexaLines[i].point.x * modelMatrix.col1.y) + (hexaLines[i].point.y * modelMatrix.col2.y) + (hexaLines[i].point.z * modelMatrix.col3.y) + (hexaLines[i].point.w * modelMatrix.col4.y);
		posVec.z = (hexaLines[i].point.x * modelMatrix.col1.z) + (hexaLines[i].point.y * modelMatrix.col2.z) + (hexaLines[i].point.z * modelMatrix.col3.z) + (hexaLines[i].point.w * modelMatrix.col4.z);
		posVec.w = (hexaLines[i].point.x * modelMatrix.col1.w) + (hexaLines[i].point.y * modelMatrix.col2.w) + (hexaLines[i].point.z * modelMatrix.col3.w) + (hexaLines[i].point.w * modelMatrix.col4.w);

		float4 dirVec;
		//Rotating the first direction vector to model space
		dirVec.x = (hexaLines[i].direction.x * modelMatrix.col1.x) + (hexaLines[i].direction.y * modelMatrix.col2.x) + (hexaLines[i].direction.z * modelMatrix.col3.x) + (hexaLines[i].direction.w * modelMatrix.col4.x);
		dirVec.y = (hexaLines[i].direction.x * modelMatrix.col1.y) + (hexaLines[i].direction.y * modelMatrix.col2.y) + (hexaLines[i].direction.z * modelMatrix.col3.y) + (hexaLines[i].direction.w * modelMatrix.col4.y);
		dirVec.z = (hexaLines[i].direction.x * modelMatrix.col1.z) + (hexaLines[i].direction.y * modelMatrix.col2.z) + (hexaLines[i].direction.z * modelMatrix.col3.z) + (hexaLines[i].direction.w * modelMatrix.col4.z);
		dirVec.w = (hexaLines[i].direction.x * modelMatrix.col1.w) + (hexaLines[i].direction.y * modelMatrix.col2.w) + (hexaLines[i].direction.z * modelMatrix.col3.w) + (hexaLines[i].direction.w * modelMatrix.col4.w);

		posVec.x -= offset.x;
		posVec.y -= offset.y;
		posVec.z -= offset.z;
		posVec.w -= offset.w;

		hexaLines[i].point = posVec;
		hexaLines[i].direction = dirVec;

		//Rotating the point so the camera is at 0,0,0 facing OpenGL viewport
		posVec.x = (hexaLines[i].point.x * rotationMatrix.col1.x) + (hexaLines[i].point.y * rotationMatrix.col2.x) + (hexaLines[i].point.z * rotationMatrix.col3.x) + (hexaLines[i].point.w * rotationMatrix.col4.x);
		posVec.y = (hexaLines[i].point.x * rotationMatrix.col1.y) + (hexaLines[i].point.y * rotationMatrix.col2.y) + (hexaLines[i].point.z * rotationMatrix.col3.y) + (hexaLines[i].point.w * rotationMatrix.col4.y);
		posVec.z = (hexaLines[i].point.x * rotationMatrix.col1.z) + (hexaLines[i].point.y * rotationMatrix.col2.z) + (hexaLines[i].point.z * rotationMatrix.col3.z) + (hexaLines[i].point.w * rotationMatrix.col4.z);
		posVec.w = (hexaLines[i].point.x * rotationMatrix.col1.w) + (hexaLines[i].point.y * rotationMatrix.col2.w) + (hexaLines[i].point.z * rotationMatrix.col3.w) + (hexaLines[i].point.w * rotationMatrix.col4.w);

		//Rotating the first direction vector to camera space
		dirVec.x = (hexaLines[i].direction.x * rotationMatrix.col1.x) + (hexaLines[i].direction.y * rotationMatrix.col2.x) + (hexaLines[i].direction.z * rotationMatrix.col3.x) + (hexaLines[i].direction.w * rotationMatrix.col4.x);
		dirVec.y = (hexaLines[i].direction.x * rotationMatrix.col1.y) + (hexaLines[i].direction.y * rotationMatrix.col2.y) + (hexaLines[i].direction.z * rotationMatrix.col3.y) + (hexaLines[i].direction.w * rotationMatrix.col4.y);
		dirVec.z = (hexaLines[i].direction.x * rotationMatrix.col1.z) + (hexaLines[i].direction.y * rotationMatrix.col2.z) + (hexaLines[i].direction.z * rotationMatrix.col3.z) + (hexaLines[i].direction.w * rotationMatrix.col4.z);
		dirVec.w = (hexaLines[i].direction.x * rotationMatrix.col1.w) + (hexaLines[i].direction.y * rotationMatrix.col2.w) + (hexaLines[i].direction.z * rotationMatrix.col3.w) + (hexaLines[i].direction.w * rotationMatrix.col4.w);

		//Cross-sectioning the lines with identity 2-space
		if (dirVec.w <= 0.0001f && dirVec.w >= -0.0001)
		{
			outStates[(thisID*12) + i] = -1;
		}
		else
		{
			float magnitude = -posVec.w/dirVec.w; //I've rechecked multiple times, it should be negetive ;)
			if (magnitude >= 0 && magnitude <= 1)
			{
				posVec += magnitude * dirVec;
				outStates[(thisID*12) + i] = 0;
				outPoints[(thisID*12) + i] = posVec;
				outTexCoords[(thisID*12) + i] = hexaLines[i].texCoords + (magnitude * hexaLines[i].texDirection);
			}
			else
			{
				outStates[(thisID*12) + i] = -1;
			}
		}
	}
}