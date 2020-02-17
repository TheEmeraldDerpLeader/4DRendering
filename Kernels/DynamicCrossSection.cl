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

__kernel void DynCrossSection(__global Line* inLines, __global float4* outPoints, __global char* outStates, __global float3* outTexCoords, __constant Mat4* cameraMatrix, __constant float4* cameraOffset)
{ //glm matrices are stored by down rows then right columns
	//Setting up local variables
	int thisID = get_global_id(0);
	Line thisLine = inLines[thisID];
	Mat4 transformMat = *cameraMatrix;
	float4 offset = *cameraOffset;
	thisLine.point.x -= offset.x;
	thisLine.point.y -= offset.y;
	thisLine.point.z -= offset.z;
	thisLine.point.w -= offset.w;

	float4 posVec;
	//Rotating the point so the camera is at 0,0,0 facing OpenGL viewport
	posVec.x = (thisLine.point.x * transformMat.col1.x) + (thisLine.point.y * transformMat.col2.x) + (thisLine.point.z * transformMat.col3.x) + (thisLine.point.w * transformMat.col4.x);
	posVec.y = (thisLine.point.x * transformMat.col1.y) + (thisLine.point.y * transformMat.col2.y) + (thisLine.point.z * transformMat.col3.y) + (thisLine.point.w * transformMat.col4.y);
	posVec.z = (thisLine.point.x * transformMat.col1.z) + (thisLine.point.y * transformMat.col2.z) + (thisLine.point.z * transformMat.col3.z) + (thisLine.point.w * transformMat.col4.z);
	posVec.w = (thisLine.point.x * transformMat.col1.w) + (thisLine.point.y * transformMat.col2.w) + (thisLine.point.z * transformMat.col3.w) + (thisLine.point.w * transformMat.col4.w);

	float4 dirVec;
	//Rotating the first direction vector to camera space
	dirVec.x = (thisLine.direction.x * transformMat.col1.x) + (thisLine.direction.y * transformMat.col2.x) + (thisLine.direction.z * transformMat.col3.x) + (thisLine.direction.w * transformMat.col4.x);
	dirVec.y = (thisLine.direction.x * transformMat.col1.y) + (thisLine.direction.y * transformMat.col2.y) + (thisLine.direction.z * transformMat.col3.y) + (thisLine.direction.w * transformMat.col4.y);
	dirVec.z = (thisLine.direction.x * transformMat.col1.z) + (thisLine.direction.y * transformMat.col2.z) + (thisLine.direction.z * transformMat.col3.z) + (thisLine.direction.w * transformMat.col4.z);
	dirVec.w = (thisLine.direction.x * transformMat.col1.w) + (thisLine.direction.y * transformMat.col2.w) + (thisLine.direction.z * transformMat.col3.w) + (thisLine.direction.w * transformMat.col4.w);

	//Cross-sectioning the lines with identity 2-space
	if (dirVec.w <= 0.0001f && dirVec.w >= -0.0001)
	{
		outStates[thisID] = -1;
	}
	else
	{
		float magnitude = -posVec.w/dirVec.w; //I've rechecked multiple times, it should be negetive ;)
		if (magnitude >= 0 && magnitude <= 1)
		{
			posVec += magnitude * dirVec;
			outStates[thisID] = 0;
			outPoints[thisID] = posVec;
			outTexCoords[thisID] = thisLine.texCoords + (magnitude * thisLine.texDirection);
		}
		else
		{
			outStates[thisID] = -1;
		}
	}
}