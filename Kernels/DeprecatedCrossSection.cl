typedef struct
{
	//Typically read where columns are unit vectors
	float4 col1;
	float4 col2;
	float4 col3;
	float4 col4;
} Mat4;

__kernel void CrossSection(__global float4* points, __global float4* directions, __global float* outMagnitude, __constant Mat4* rotationMatrix, __constant float4* offset)
{
	//Setting up local variables
	int thisID = get_global_id(0);
	float4 thisPoint = points[thisID];
	float4 thisDirection = directions[thisID];
	Mat4 transformMat = *rotationMatrix;

	thisPoint.x -= offset->x;
	thisPoint.y -= offset->y;
	thisPoint.z -= offset->z;
	thisPoint.w -= offset->w;

	float4 tempVec;
	//Rotating the point so the camera is at 0,0,0 facing OpenGL viewport
	tempVec.x = 0;
	tempVec.x += thisPoint.x * transformMat.col1.x;
	tempVec.x += thisPoint.y * transformMat.col2.x;
	tempVec.x += thisPoint.z * transformMat.col3.x;
	tempVec.x += thisPoint.w * transformMat.col4.x;

	tempVec.y = 0;
	tempVec.y += thisPoint.x * transformMat.col1.y;
	tempVec.y += thisPoint.y * transformMat.col2.y;
	tempVec.y += thisPoint.z * transformMat.col3.y;
	tempVec.y += thisPoint.w * transformMat.col4.y;

	tempVec.z = 0;
	tempVec.z += thisPoint.x * transformMat.col1.z;
	tempVec.z += thisPoint.y * transformMat.col2.z;
	tempVec.z += thisPoint.z * transformMat.col3.z;
	tempVec.z += thisPoint.w * transformMat.col4.z;

	tempVec.w = 0;
	tempVec.w += thisPoint.x * transformMat.col1.w;
	tempVec.w += thisPoint.y * transformMat.col2.w;
	tempVec.w += thisPoint.z * transformMat.col3.w;
	tempVec.w += thisPoint.w * transformMat.col4.w;

	thisPoint = tempVec;

	//Rotating the direction vector to camera space
	tempVec.x = 0;
	tempVec.x += thisDirection.x * transformMat.col1.x;
	tempVec.x += thisDirection.y * transformMat.col2.x;
	tempVec.x += thisDirection.z * transformMat.col3.x;
	tempVec.x += thisDirection.w * transformMat.col4.x;

	tempVec.y = 0;
	tempVec.y += thisDirection.x * transformMat.col1.y;
	tempVec.y += thisDirection.y * transformMat.col2.y;
	tempVec.y += thisDirection.z * transformMat.col3.y;
	tempVec.y += thisDirection.w * transformMat.col4.y;

	tempVec.z = 0;
	tempVec.z += thisDirection.x * transformMat.col1.z;
	tempVec.z += thisDirection.y * transformMat.col2.z;
	tempVec.z += thisDirection.z * transformMat.col3.z;
	tempVec.z += thisDirection.w * transformMat.col4.z;

	tempVec.w = 0;
	tempVec.w += thisDirection.x * transformMat.col1.w;
	tempVec.w += thisDirection.y * transformMat.col2.w;
	tempVec.w += thisDirection.z * transformMat.col3.w;
	tempVec.w += thisDirection.w * transformMat.col4.w;

	thisDirection = tempVec;

	//Cross-sectioning lines
	float tempMagnitude;
	if (thisDirection.w <= -0.05f || thisDirection.w >= 0.05f)
	{
		tempMagnitude = thisPoint.w / (-thisDirection.w);
	}
	else
	{
		if (thisPoint.w <= -0.05f || thisPoint.w >= 0.05f){
			tempMagnitude = -1;
		}
		else
		{
			tempMagnitude = 0;
		}
	}

	thisPoint.x = thisPoint.x + (thisDirection.x * tempMagnitude);
	thisPoint.y = thisPoint.y + (thisDirection.y * tempMagnitude);
	thisPoint.z = thisPoint.z + (thisDirection.z * tempMagnitude);
	thisPoint.w = thisPoint.w + (thisDirection.w * tempMagnitude);

	points[thisID] = thisPoint;
	outMagnitude[thisID] = tempMagnitude;
}