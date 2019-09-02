typedef struct
{
	//Typically read where columns are unit vectors
	float4 col1;
	float4 col2;
	float4 col3;
	float4 col4;
} Mat4;

__kernel void CrossSection(__global float4* lineVertices, __global float4* directionVectors, __global char* outStates, __constant Mat4* rotationMatrix, __constant float4* offset)
{
	//Setting up local variables
	int thisID = get_global_id(0);
	float4 posVec = lineVertices[thisID];
	float4 dirVec = directionVectors[thisID];
	Mat4 transformMat = *rotationMatrix;

	posVec.x -= offset->x;
	posVec.y -= offset->y;
	posVec.z -= offset->z;
	posVec.w -= offset->w;

	float4 tempVec;
	//Rotating the point so the camera is at 0,0,0 facing OpenGL viewport
	tempVec.x = 0;
	tempVec.x += posVec.x * transformMat.col1.x;
	tempVec.x += posVec.y * transformMat.col2.x;
	tempVec.x += posVec.z * transformMat.col3.x;
	tempVec.x += posVec.w * transformMat.col4.x;

	tempVec.y = 0;
	tempVec.y += posVec.x * transformMat.col1.y;
	tempVec.y += posVec.y * transformMat.col2.y;
	tempVec.y += posVec.z * transformMat.col3.y;
	tempVec.y += posVec.w * transformMat.col4.y;

	tempVec.z = 0;
	tempVec.z += posVec.x * transformMat.col1.z;
	tempVec.z += posVec.y * transformMat.col2.z;
	tempVec.z += posVec.z * transformMat.col3.z;
	tempVec.z += posVec.w * transformMat.col4.z;

	tempVec.w = 0;
	tempVec.w += posVec.x * transformMat.col1.w;
	tempVec.w += posVec.y * transformMat.col2.w;
	tempVec.w += posVec.z * transformMat.col3.w;
	tempVec.w += posVec.w * transformMat.col4.w;

	posVec = tempVec;

	//Rotating the first direction vector to camera space
	tempVec.x = 0;
	tempVec.x += dirVec.x * transformMat.col1.x;
	tempVec.x += dirVec.y * transformMat.col2.x;
	tempVec.x += dirVec.z * transformMat.col3.x;
	tempVec.x += dirVec.w * transformMat.col4.x;

	tempVec.y = 0;
	tempVec.y += dirVec.x * transformMat.col1.y;
	tempVec.y += dirVec.y * transformMat.col2.y;
	tempVec.y += dirVec.z * transformMat.col3.y;
	tempVec.y += dirVec.w * transformMat.col4.y;

	tempVec.z = 0;
	tempVec.z += dirVec.x * transformMat.col1.z;
	tempVec.z += dirVec.y * transformMat.col2.z;
	tempVec.z += dirVec.z * transformMat.col3.z;
	tempVec.z += dirVec.w * transformMat.col4.z;

	tempVec.w = 0;
	tempVec.w += dirVec.x * transformMat.col1.w;
	tempVec.w += dirVec.y * transformMat.col2.w;
	tempVec.w += dirVec.z * transformMat.col3.w;
	tempVec.w += dirVec.w * transformMat.col4.w;

	dirVec = tempVec;
	//Cross-sectioning the lines with identity 2-space
	if (dirVec.w <= 0.0001f && dirVec.w >= -0.0001) //ROUNDING ERRORS!!!
	{
		outStates[thisID] = -1;
		/*
		if (posVec.w == 0)
		{
			directionVectors[thisID] = dirVec;
			outStates[thisID] = 1;
		}
		else
		{
			outStates[thisID] = -1;
		}
		*/
	}
	else
	{
		float magnitude = -posVec.w/dirVec.w;
		if (magnitude >= 0 && magnitude <= 1)
		{
			posVec += magnitude * dirVec;
			outStates[thisID] = 0;
		}
		else
		{
			outStates[thisID] = -1;
		}
	}

	lineVertices[thisID] = posVec;
}