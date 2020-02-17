typedef struct
{
	int id;
	float value;
} SortItem;

__kernel void HexaMakeFace(__global float4* points, __global char* inStates, __global float4* texCoords, __global char* outStates)
{
	int thisID = get_global_id(0);
	float4 posVec[12];
	float4 texVec[12];
	int checkSize = 0;
	for (int i = 0; i < 12; i++)
	{
		if (inStates[(thisID*12) + i] == 0)
		{
			posVec[checkSize] = points[(thisID*12) + i];
			texVec[checkSize] = texCoords[(thisID*12) + i];
			checkSize++;
		}
	}
	if (checkSize < 3){
		outStates[thisID] = -1;
		return;
	}
	outStates[thisID] = checkSize;
	float2 planedVertices[12];
	{
		float4 origin = posVec[0];
		float4 planeVec1 = posVec[1] - origin;
		float4 planeVec2 = posVec[2] - origin;
		float4 crossProduct;
		crossProduct.x = (planeVec1.y*planeVec2.z)-(planeVec1.z*planeVec2.y);
		crossProduct.y = (planeVec1.z*planeVec2.x)-(planeVec1.x*planeVec2.z);
		crossProduct.z = (planeVec1.x*planeVec2.y)-(planeVec1.y*planeVec2.x);

		float4* vertex; 
		if (crossProduct.z != 0)
		{
			for (int i = 0; i < checkSize; i++)
			{
				vertex = posVec + i;
				planedVertices[i].x = vertex->x;
				planedVertices[i].y = vertex->y;
			}
		}
		else
		{
			if (crossProduct.x != 0)
			{
				for (int i = 0; i < checkSize; i++)
				{
					vertex = posVec + i;
					planedVertices[i].x = vertex->y;
					planedVertices[i].y = vertex->z;
				}
			}
			else
			{
				for (int i = 0; i < checkSize; i++)
				{
					vertex = posVec + i;
					planedVertices[i].x = vertex->z;
					planedVertices[i].y = vertex->x;
				}
			}
		}
	}
	float2 centroid;
	centroid.x = 0;
	centroid.y = 0;
	SortItem rightVertices[12];
	SortItem leftVertices[12];
	int rcount = 0;
	int lcount = 0;
	for (int i = 0; i < checkSize; i++)
	{
		centroid += planedVertices[i];
	}
	centroid /= (float)checkSize;
	for (int i = 0; i < checkSize; i++)
	{
		float2* vertex;
		vertex = planedVertices + i;
		*vertex -= centroid;
		float mag = (vertex->x*vertex->x) + (vertex->y*vertex->y);
		if (vertex->x > 0)
		{
			if (vertex->y < 0) //Sin(theta) = cross/magnitude. Cross of point and (1,0). Sin is invertable between -90 and 90 deg. Cross is y * |y| 
			{
				rightVertices[rcount].id = i;
				rightVertices[rcount].value = -(vertex->y * vertex->y) / mag;
			}
			else
			{
				rightVertices[rcount].id = i;
				rightVertices[rcount].value = (vertex->y * vertex->y) / mag;
			}
			rcount++;
		}
		else
		{
			if (vertex->y < 0)
			{
				leftVertices[lcount].id = i;
				leftVertices[lcount].value = -(vertex->y * vertex->y) / mag;
			}
			else
			{
				leftVertices[lcount].id = i;
				leftVertices[lcount].value = (vertex->y * vertex->y) / mag;
			}
			lcount++;
		}
		//Add to sides with values and ID
	}

	//Sort each side
	for (int i = 1; i < rcount; i++)
	{
		SortItem item = rightVertices[i];
		for (int b = 1; b <= i; b++)
		{
			SortItem* checkItem = &(rightVertices[i - b]);
			if (item.value > checkItem->value)
			{
				SortItem swapper = *checkItem;
				*checkItem = rightVertices[i - (b-1)];
				rightVertices[i - (b-1)] = swapper;
			}
			else
			{
				break;
			}
		}
	}
	for (int i = 1; i < lcount; i++)
	{
		SortItem item = leftVertices[i];
		for (int b = 1; b <= i; b++)
		{
			SortItem* checkItem = &(leftVertices[i - b]);
			if (item.value > checkItem->value)
			{
				SortItem swapper = *checkItem;
				*checkItem = leftVertices[i - (b-1)];
				leftVertices[i - (b-1)] = swapper;
			}
			else
			{
				break;
			}
		}
	}

	int ocount = 0;
	checkSize = rcount;
	for (int i = 0; i < checkSize; i++)
	{
		points[(thisID * 12) + ocount] = posVec[rightVertices[i].id];
		texCoords[(thisID * 12) + ocount] = texVec[rightVertices[i].id];
		ocount++;
	}
	checkSize = lcount;
	for (int i = checkSize - 1; i >= 0; i--)
	{
		points[(thisID * 12) + ocount] = posVec[leftVertices[i].id];
		texCoords[(thisID * 12) + ocount] = texVec[leftVertices[i].id];
		ocount++;
	}
}