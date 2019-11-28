#include "RenderObjects.h"

//Camera
Camera::Camera()
{
	position = glm::vec4(0, 0, 0, 0);
	rotation[0] = 0;
	rotation[1] = 0;
	rotation[2] = 0;
	rotation[3] = 0;
	rotation[4] = 0;
	rotation[5] = 0;
}

Camera::Camera(glm::vec4 positionVec, float xy, float yz, float zx, float xw, float yw, float zw)
{
	position = positionVec;
	rotation[0] = xy;
	rotation[1] = yz;
	rotation[2] = zx;
	rotation[3] = xw;
	rotation[4] = yw;
	rotation[5] = zw;
}

glm::mat4x4 Camera::GetTransform()
{
	copyMatrix = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	glm::mat4x4 tempMat;
	float angle;
	//Inverted and column major

	//X->Y rotation
	angle = glm::radians(rotation[0]);
	tempMat = glm::mat4x4(glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	copyMatrix *= tempMat;

	//Y->Z rotation
	angle = glm::radians(rotation[1]);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle), 0, 0, 0, 0, 1);
	copyMatrix *= tempMat;

	//Z->X rotation
	angle = glm::radians(rotation[2]);
	tempMat = glm::mat4x4(glm::cos(angle), 0, glm::sin(angle), 0, 0, 1, 0, 0, -glm::sin(angle), 0, glm::cos(angle), 0, 0, 0, 0, 1);
	copyMatrix *= tempMat;

	//X->W rotation
	angle = glm::radians(rotation[3]);
	tempMat = glm::mat4x4(glm::cos(angle), 0, 0, -glm::sin(angle), 0, 1, 0, 0, 0, 0, 1, 0, glm::sin(angle), 0, 0, glm::cos(angle));
	copyMatrix *= tempMat;

	//Y->W rotation
	angle = glm::radians(rotation[4]);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, glm::cos(angle), 0, -glm::sin(angle), 0, 0, 1, 0, 0, glm::sin(angle), 0, glm::cos(angle));
	copyMatrix *= tempMat;

	//Z->W rotation
	angle = glm::radians(rotation[5]);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle));
	copyMatrix *= tempMat;

	return copyMatrix;
}

float* Camera::GetTransformValuePtr()
{
	GetTransform();
	return glm::value_ptr(copyMatrix);
}

void Camera::RotateXY(float degrees)
{
	rotation[0] += degrees;
}

void Camera::RotateYZ(float degrees)
{
	rotation[1] += degrees;
}

void Camera::RotateZX(float degrees)
{
	rotation[2] += degrees;
}

void Camera::RotateXW(float degrees)
{
	rotation[3] += degrees;
}

void Camera::RotateYW(float degrees)
{
	rotation[4] += degrees;
}

void Camera::RotateZW(float degrees)
{
	rotation[5] += degrees;
}
//Renderable
Renderable::Renderable()
{
	transformation = glm::mat4x4(1);
	offset = glm::vec4(0);
	modelID = 0;
}

Renderable::Renderable(glm::mat4x4 transformationMatrix, glm::vec4 offsetVec, int modelIDInt)
{
	transformation = transformationMatrix;
	offset = offsetVec;
	modelID = modelIDInt;
}
//RenderManager
RenderManager::RenderManager()
{
	renderMode = false;
	crossSection = GPUProgram("Assets\\Kernels\\CrossSection.cl");
	crossSection.SetFunction(0, "CrossSection");
	crossSection.CreateProgram("Assets\\Kernels\\TetraRenderer.cl");
	crossSection.SetFunction(1, "MakeFace");
	pentachoronModel[0] = Tetrahedron(0,0,0,0, 0,0,0, 1,0,0,0, 1,0,0, 0,1,0,0, 0,1,0, 0,0,1,0, 0,0,1);
	pentachoronModel[1] = Tetrahedron(0,0,0,1, 0,0,0, 1,0,0,0, 1,0,0, 0,1,0,0, 0,1,0, 0,0,1,0, 0,0,1);
	pentachoronModel[2] = Tetrahedron(0,0,0,0, 0,0,0, 0,0,0,1, 1,0,0, 0,1,0,0, 0,1,0, 0,0,1,0, 0,0,1);
	pentachoronModel[3] = Tetrahedron(0,0,0,0, 0,0,0, 1,0,0,0, 1,0,0, 0,0,0,1, 0,1,0, 0,0,1,0, 0,0,1);
	pentachoronModel[4] = Tetrahedron(0,0,0,0, 0,0,0, 1,0,0,0, 1,0,0, 0,1,0,0, 0,1,0, 0,0,0,1, 0,0,1);

	Line modelLines[30];
	for (int i = 0; i < 5; i++) 
	{
		for (int l = 0; l < 6; l++)
		{
			modelLines[(i * 6) + l] = pentachoronModel[i].lines[l];
		}
	}
	modelBuffer = cl::Buffer(crossSection.context, CL_MEM_HOST_WRITE_ONLY | CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(Tetrahedron)*5, modelLines);
}

unsigned int RenderManager::SetBuffer(Camera& camera, unsigned int VBOaddress)
{
	int pentaSize = pentaRenderables.size();
	int tetraSize = pentaSize * 5;
	int lineSize = (tetraSize * 6);
	if (lineSize == 0)
		return 0;
	vertexPos.clear();
	vertexCol.clear();
#pragma region Kernel setup and launch
	glm::mat4x4* modelMatrices = new glm::mat4x4[pentaSize];
	glm::vec4* modelOffsets = new glm::vec4[pentaSize];

	int* modelIDs = new int[tetraSize];
	int* tetraIDs = new int[tetraSize];

	glm::vec4* points = new glm::vec4[lineSize];
	char* states = new char[lineSize];
	glm::vec4* texCoords = new glm::vec4[lineSize];

	for (int i = 0; i < pentaSize; i++)
	{
		Renderable& renderable = pentaRenderables[i];
		modelMatrices[i] = renderable.transformation;
		modelOffsets[i] = renderable.offset;
		for (int t = 0; t < 5; t++)
		{
			modelIDs[(i * 5) + t] = i;
			tetraIDs[(i * 5) + t] = ((renderable.modelID*5) + t);
		}
	}

	cl::Buffer modelMatricesBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::mat4x4)*pentaSize, modelMatrices);
	cl::Buffer modelOffsetsBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4)*pentaSize, modelOffsets);
	cl::Buffer modelIDBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int) * tetraSize, modelIDs);
	cl::Buffer tetraIDBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int) * tetraSize, tetraIDs);
	cl::Buffer pointBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(glm::vec4) * lineSize);
	cl::Buffer stateBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(char) * lineSize);
	cl::Buffer texCoordBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(glm::vec4) * lineSize);
	cl::Buffer matrixBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::mat4), camera.GetTransformValuePtr());
	cl::Buffer offsetBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4), glm::value_ptr(camera.position));

	crossSection.SetVariable(0, 0, modelMatricesBuffer);
	crossSection.SetVariable(0, 1, modelOffsetsBuffer);
	crossSection.SetVariable(0, 2, modelIDBuffer);
	crossSection.SetVariable(0, 3, tetraIDBuffer);
	crossSection.SetVariable(0, 4, pointBuffer);
	crossSection.SetVariable(0, 5, stateBuffer);
	crossSection.SetVariable(0, 6, texCoordBuffer);
	crossSection.SetVariable(0, 7, matrixBuffer);
	crossSection.SetVariable(0, 8, offsetBuffer);
	crossSection.SetVariable(0, 9, modelBuffer);
	crossSection.LaunchKernel(0, 0, tetraSize);

	delete[]  modelMatrices;
	delete[]  modelOffsets;
	delete[]  modelIDs;
	delete[]  tetraIDs;
#pragma endregion
	crossSection.ReadKernel(0, pointBuffer, GL_TRUE, 0, sizeof(glm::vec4) * lineSize, points);
	crossSection.ReadKernel(0, stateBuffer, GL_TRUE, 0, sizeof(char) * lineSize, states); // 8 ms
	crossSection.ReadKernel(0, texCoordBuffer, GL_TRUE, 0, sizeof(glm::vec4) * lineSize, texCoords);
	if (renderMode)
	{
#pragma region tetrahedron kernel
		if (tetraSize > 0)
		{
			char* tetraStates = new char[tetraSize];
			cl::Buffer tetraStateBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(char)*tetraSize);
			crossSection.SetVariable(1, 0, pointBuffer);
			crossSection.SetVariable(1, 1, stateBuffer);
			crossSection.SetVariable(1, 2, tetraStateBuffer);
			crossSection.LaunchKernel(1, 0, tetraSize);
			crossSection.ReadKernel(1, pointBuffer, GL_TRUE, 0, sizeof(glm::vec4) * lineSize, points);
			crossSection.ReadKernel(1, tetraStateBuffer, GL_TRUE, 0, sizeof(char)*tetraSize, tetraStates);
			faceColor = 0;
			for (int i = 0; i < tetraSize; i++)
			{
				if (tetraStates[i] >= 3)
				{
					int checkSize = tetraStates[i] - 2;
					for (int t = 0; t < checkSize; t++)
					{
						switch (faceColor)
						{
						case 0:
							vertexPos.push_back(points[(i * 6) + 0]);
							vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
							vertexPos.push_back(points[(i * 6) + 1 + t]);
							vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
							vertexPos.push_back(points[(i * 6) + 2 + t]);
							vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
							break;
						case 1:
							vertexPos.push_back(points[(i * 6) + 0]);
							vertexCol.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
							vertexPos.push_back(points[(i * 6) + 1 + t]);
							vertexCol.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
							vertexPos.push_back(points[(i * 6) + 2 + t]);
							vertexCol.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
							break;
						case 2:
							vertexPos.push_back(points[(i * 6) + 0]);
							vertexCol.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
							vertexPos.push_back(points[(i * 6) + 1 + t]);
							vertexCol.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
							vertexPos.push_back(points[(i * 6) + 2 + t]);
							vertexCol.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
							break;
						case 3:
							vertexPos.push_back(points[(i * 6) + 0]);
							vertexCol.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
							vertexPos.push_back(points[(i * 6) + 1 + t]);
							vertexCol.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
							vertexPos.push_back(points[(i * 6) + 2 + t]);
							vertexCol.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
							break;
						case 4:
							vertexPos.push_back(points[(i * 6) + 0]);
							vertexCol.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
							vertexPos.push_back(points[(i * 6) + 1 + t]);
							vertexCol.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
							vertexPos.push_back(points[(i * 6) + 2 + t]);
							vertexCol.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
							break;
						}
					}
					faceColor = Modulo(faceColor + 1, 5);
				}
			} //7 ms
			delete[] tetraStates;
		}
#pragma endregion
	} //7 ms
	else
	{
#pragma region Kernel tetrahedron return handler

		faceColor = 0;
		SmartArray<glm::vec4> vertexResults(6);
		vertexResults.count = 0;
		for (int t = 0; t < tetraSize; t++)
		{
			//Make an array to hold the results. Sort results to Right and left and add to the lists. Sort the lists then combine them. Create triangles from the combinations

			//Which lines crossected
			int checkSize = 0;
			if (states[(t * 6)] == 0)
			{
				vertexResults.data[checkSize] = points[(t * 6)];
				checkSize++;
			}
			if (states[(t * 6) + 1] == 0)
			{
				vertexResults.data[checkSize] = points[(t * 6) + 1];
				checkSize++;
			}
			if (states[(t * 6) + 2] == 0)
			{
				vertexResults.data[checkSize] = points[(t * 6) + 2];
				checkSize++;
			}
			if (states[(t * 6) + 3] == 0)
			{
				vertexResults.data[checkSize] = points[(t * 6) + 3];
				checkSize++;
			}
			if (states[(t * 6) + 4] == 0)
			{
				vertexResults.data[checkSize] = points[(t * 6) + 4];
				checkSize++;
			}
			if (states[(t * 6) + 5] == 0)
			{
				vertexResults.data[checkSize] = points[(t * 6) + 5];
				checkSize++;
			}
			vertexResults.count = checkSize;
			if (checkSize < 3)
				continue;

			SmartArray<glm::vec4> planedVertices(checkSize);
#pragma region To plane space
			{ //This stuff is explained is some graph paper I worked on. You would need to see it to understand this.
				glm::vec4 origin = vertexResults.data[0]; //i, j, k
				glm::vec4 planeVec1 = vertexResults.data[1] - origin; // f, g, h
				glm::vec4 planeVec2 = vertexResults.data[2] - origin; // m, n, o

				float w = (planeVec2.x * planeVec1.y) - (planeVec2.y * planeVec1.x);
				float gdivw = planeVec1.y / w;
				float fdivw = planeVec1.x / w;
				float kdivh = -(origin.z / planeVec1.z);
				float odivh = planeVec2.z / planeVec1.z;
				for (int i = 0; i < checkSize; i++)
				{
					glm::vec4& vertex = vertexResults.data[i];
					float b = (gdivw*(vertex.x - origin.x)) - (fdivw*(vertex.y - origin.y));
					planedVertices.data[i] = glm::vec4((vertex.z / planeVec1.z) + kdivh - (odivh*b), b, 0, 0); //The x coordinate is called "a"	
				}
			}
#pragma endregion

#pragma region Rotation ordering stuff
			glm::vec4 centroid(0);
			SmartArray<SortItem> rightVertices(checkSize);
			SmartArray<SortItem> leftVertices(checkSize);
			rightVertices.count = 0;
			leftVertices.count = 0;
			for (int i = 0; i < checkSize; i++)
			{
				centroid += planedVertices.data[i];
			}
			centroid /= checkSize;
			for (int i = 0; i < checkSize; i++)
			{
				glm::vec4& vertex = planedVertices.data[i];
				vertex -= centroid;
				if (vertex.x > 0)
				{
					float mag = (vertex.x*vertex.x) + (vertex.y*vertex.y);
					rightVertices.data[rightVertices.count] = SortItem(i, (vertex.y * std::abs(vertex.y)) / mag); //Sin(theta)^2 = cross/magnitude. Cross of point and (1,0). Sin is invertable between -90 and 90 deg. Cross is x * |x| 
					rightVertices.count++;
				}
				else
				{
					float mag = (vertex.x*vertex.x) + (vertex.y*vertex.y);
					leftVertices.data[leftVertices.count] = SortItem(i, (vertex.y * std::abs(vertex.y)) / mag);
					leftVertices.count++;
				}
				//Add to sides with values and ID
			}
#pragma endregion
			//Sort each side
			SmartArray<glm::vec4> orderedVertices(checkSize);
			orderedVertices.count = 0;
			InsertSort(rightVertices);
			InsertSort(leftVertices);
			checkSize = rightVertices.count;
			for (int i = 0; i < checkSize; i++)
			{
				orderedVertices.data[orderedVertices.count] = vertexResults.data[rightVertices.data[i].ID];
				orderedVertices.count++;
			}
			checkSize = leftVertices.count;
			for (int i = checkSize - 1; i >= 0; i--)
			{
				orderedVertices.data[orderedVertices.count] = vertexResults.data[leftVertices.data[i].ID];
				orderedVertices.count++;
			}
			checkSize = orderedVertices.count - 2;
			for (int i = 0; i < checkSize; i++)
			{
				switch (faceColor)
				{
				case 0:
					vertexPos.push_back(orderedVertices.data[0]);
					vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					vertexPos.push_back(orderedVertices.data[i + 1]);
					vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					vertexPos.push_back(orderedVertices.data[i + 2]);
					vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					break;
				case 1:
					vertexPos.push_back(orderedVertices.data[0]);
					vertexCol.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
					vertexPos.push_back(orderedVertices.data[i + 1]);
					vertexCol.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
					vertexPos.push_back(orderedVertices.data[i + 2]);
					vertexCol.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
					break;
				case 2:
					vertexPos.push_back(orderedVertices.data[0]);
					vertexCol.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
					vertexPos.push_back(orderedVertices.data[i + 1]);
					vertexCol.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
					vertexPos.push_back(orderedVertices.data[i + 2]);
					vertexCol.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
					break;
				case 3:
					vertexPos.push_back(orderedVertices.data[0]);
					vertexCol.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
					vertexPos.push_back(orderedVertices.data[i + 1]);
					vertexCol.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
					vertexPos.push_back(orderedVertices.data[i + 2]);
					vertexCol.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
					break;
				case 4:
					vertexPos.push_back(orderedVertices.data[0]);
					vertexCol.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
					vertexPos.push_back(orderedVertices.data[i + 1]);
					vertexCol.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
					vertexPos.push_back(orderedVertices.data[i + 2]);
					vertexCol.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
					break;
				}
			}
			faceColor = Modulo(faceColor + 1, 5);
		}

#pragma endregion
	} //7 ms

#pragma region Buffer setup
	float* vboData = new float[vertexPos.size() * 9];

	for (int i = 0; i < vertexPos.size(); i++)
	{
		vboData[0 + (i * 9)] = vertexPos[i].x;
		vboData[1 + (i * 9)] = vertexPos[i].y;
		vboData[2 + (i * 9)] = vertexPos[i].z;
		vboData[3 + (i * 9)] = 1;
		vboData[4 + (i * 9)] = 1;
		vboData[5 + (i * 9)] = 1;
		vboData[6 + (i * 9)] = texCoords[i].x;
		vboData[7 + (i * 9)] = texCoords[i].y;
		vboData[8 + (i * 9)] = texCoords[i].z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBOaddress);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float) * vertexPos.size(), vboData, GL_DYNAMIC_DRAW);
#pragma endregion
	delete[] vboData;
	delete[] points;
	delete[] states;
	return vertexPos.size(); // 14 ms
}
//Cube, 2D, old 3D
/*
#pragma region Kernel cube return handler

	faceColor = 0;
	vertexResults.Release();
	vertexResults.Initialize(12);
	for (int t = 0; t < cubeSize; t++)
	{
		//Which lines crossected
		int checkSize = 0;
		if (states[(t * 12) + tetraOffset] == 0)
		{
			vertexResults.data[checkSize] = points[(t * 12) + tetraOffset];
			checkSize++;
		}
		if (states[(t * 12) + 1 + tetraOffset] == 0)
		{
			vertexResults.data[checkSize] = points[(t * 12) + 1 + tetraOffset];
			checkSize++;
		}
		if (states[(t * 12) + 2 + tetraOffset] == 0)
		{
			vertexResults.data[checkSize] = points[(t * 12) + 2 + tetraOffset];
			checkSize++;
		}
		if (states[(t * 12) + 3 + tetraOffset] == 0)
		{
			vertexResults.data[checkSize] = points[(t * 12) + 3 + tetraOffset];
			checkSize++;
		}
		if (states[(t * 12) + 4 + tetraOffset] == 0)
		{
			vertexResults.data[checkSize] = points[(t * 12) + 4 + tetraOffset];
			checkSize++;
		}
		if (states[(t * 12) + 5 + tetraOffset] == 0)
		{
			vertexResults.data[checkSize] = points[(t * 12) + 5 + tetraOffset];
			checkSize++;
		}
		if (states[(t * 12) + 6 + tetraOffset] == 0)
		{
			vertexResults.data[checkSize] = points[(t * 12) + 6 + tetraOffset];
			checkSize++;
		}
		if (states[(t * 12) + 7 + tetraOffset] == 0)
		{
			vertexResults.data[checkSize] = points[(t * 12) + 7 + tetraOffset];
			checkSize++;
		}
		if (states[(t * 12) + 8 + tetraOffset] == 0)
		{
			vertexResults.data[checkSize] = points[(t * 12) + 8 + tetraOffset];
			checkSize++;
		}
		if (states[(t * 12) + 9 + tetraOffset] == 0)
		{
			vertexResults.data[checkSize] = points[(t * 12) + 9 + tetraOffset];
			checkSize++;
		}
		if (states[(t * 12) + 10 + tetraOffset] == 0)
		{
			vertexResults.data[checkSize] = points[(t * 12) + 10 + tetraOffset];
			checkSize++;
		}
		if (states[(t * 12) + 11 + tetraOffset] == 0)
		{
			vertexResults.data[checkSize] = points[(t * 12) + 11 + tetraOffset];
			checkSize++;
		}
		vertexResults.count = checkSize;
		if (checkSize < 3)
			continue;

		SmartArray<glm::vec4> planedVertices(checkSize);
#pragma region To plane space
		{ //This stuff is explained is some graph paper I worked on. You would need to see it to understand this.
			glm::vec4 origin = vertexResults.data[0]; //i, j, k
			glm::vec4 planeVec1 = vertexResults.data[1] - origin; // f, g, h
			glm::vec4 planeVec2 = vertexResults.data[2] - origin; // m, n, o

			float w = (planeVec2.x * planeVec1.y) - (planeVec2.y * planeVec1.x);
			float gdivw = planeVec1.y / w;
			float fdivw = planeVec1.x / w;
			float kdivh = -(origin.z / planeVec1.z);
			float odivh = planeVec2.z / planeVec1.z;
			for (int i = 0; i < checkSize; i++)
			{
				glm::vec4& vertex = vertexResults.data[i];
				float b = (gdivw*(vertex.x - origin.x)) - (fdivw*(vertex.y - origin.y));
				planedVertices.data[i] = glm::vec4((vertex.z / planeVec1.z) + kdivh - (odivh*b), b, 0, 0); //The x coordinate is called "a"
			}
		}
#pragma endregion

#pragma region Rotation ordering stuff
		glm::vec4 centroid(0);
		SmartArray<SortItem> rightVertices(checkSize);
		SmartArray<SortItem> leftVertices(checkSize);
		rightVertices.count = 0;
		leftVertices.count = 0;
		//vectorSize = vertexResults.size();
		for (int i = 0; i < checkSize; i++)
		{
			centroid += planedVertices.data[i];
		}
		centroid /= checkSize;
		for (int i = 0; i < checkSize; i++)
		{
			glm::vec4& vertex = planedVertices.data[i];
			vertex -= centroid;
			if (vertex.x > 0)
			{
				float mag = (vertex.x*vertex.x) + (vertex.y*vertex.y);
				rightVertices.data[rightVertices.count] = SortItem(i, (vertex.y * std::abs(vertex.y)) / mag); //Sin(theta)^2 = cross/magnitude. Cross of point and (1,0). Sin is invertable between -90 and 90 deg. Cross is x * |x|
				rightVertices.count++;
			}
			else
			{
				float mag = (vertex.x*vertex.x) + (vertex.y*vertex.y);
				leftVertices.data[leftVertices.count] = SortItem(i, (vertex.y * std::abs(vertex.y)) / mag);
				leftVertices.count++;
			}
			//Add to sides with values and ID
		}
#pragma endregion

		//Sort each side
		SmartArray<glm::vec4> orderedVertices(checkSize);
		orderedVertices.count = 0;
		InsertSort(rightVertices);
		InsertSort(leftVertices);
		checkSize = rightVertices.count;
		for (int i = 0; i < checkSize; i++)
		{
			orderedVertices.data[orderedVertices.count] = vertexResults.data[rightVertices.data[i].ID];
			orderedVertices.count++;
		}
		checkSize = leftVertices.count;
		for (int i = checkSize - 1; i >= 0; i--)
		{
			orderedVertices.data[orderedVertices.count] = vertexResults.data[leftVertices.data[i].ID];
			orderedVertices.count++;
		}
		checkSize = orderedVertices.count - 2;
		for (int i = 0; i < checkSize; i++)
		{
			switch (faceColor)
			{
			case 0:
				vertexPos.push_back(orderedVertices.data[0]);
				vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				vertexPos.push_back(orderedVertices.data[i + 1]);
				vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				vertexPos.push_back(orderedVertices.data[i + 2]);
				vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				break;
			case 1:
				vertexPos.push_back(orderedVertices.data[0]);
				vertexCol.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
				vertexPos.push_back(orderedVertices.data[i + 1]);
				vertexCol.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
				vertexPos.push_back(orderedVertices.data[i + 2]);
				vertexCol.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
				break;
			case 2:
				vertexPos.push_back(orderedVertices.data[0]);
				vertexCol.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
				vertexPos.push_back(orderedVertices.data[i + 1]);
				vertexCol.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
				vertexPos.push_back(orderedVertices.data[i + 2]);
				vertexCol.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
				break;
			case 3:
				vertexPos.push_back(orderedVertices.data[0]);
				vertexCol.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
				vertexPos.push_back(orderedVertices.data[i + 1]);
				vertexCol.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
				vertexPos.push_back(orderedVertices.data[i + 2]);
				vertexCol.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
				break;
			case 4:
				vertexPos.push_back(orderedVertices.data[0]);
				vertexCol.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
				vertexPos.push_back(orderedVertices.data[i + 1]);
				vertexCol.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
				vertexPos.push_back(orderedVertices.data[i + 2]);
				vertexCol.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
				break;
			case 5:
				vertexPos.push_back(orderedVertices.data[0]);
				vertexCol.push_back(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
				vertexPos.push_back(orderedVertices.data[i + 1]);
				vertexCol.push_back(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
				vertexPos.push_back(orderedVertices.data[i + 2]);
				vertexCol.push_back(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
				break;
			case 6:
				vertexPos.push_back(orderedVertices.data[0]);
				vertexCol.push_back(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
				vertexPos.push_back(orderedVertices.data[i + 1]);
				vertexCol.push_back(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
				vertexPos.push_back(orderedVertices.data[i + 2]);
				vertexCol.push_back(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
				break;
			case 7:
				vertexPos.push_back(orderedVertices.data[0]);
				vertexCol.push_back(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
				vertexPos.push_back(orderedVertices.data[i + 1]);
				vertexCol.push_back(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
				vertexPos.push_back(orderedVertices.data[i + 2]);
				vertexCol.push_back(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
				break;
			}
		}
		faceColor = Modulo(faceColor + 1, 8);
	}
#pragma endregion

unsigned int RenderManager::SetBuffer(Camera& camera, unsigned int VBOaddress)
{
	faceColor = 0;
	crossSection.SetFunction("CrossSection");

	if (points != nullptr)
		delete[] points;
	if (directions != nullptr)
		delete[] directions;
	if (states != nullptr)
		delete[] states;
	points = new glm::vec4[tetrahedrons.size() * 6];
	directions = new glm::vec4[tetrahedrons.size() * 6];
	states = new char[tetrahedrons.size() * 6];

	std::vector<Line> lines;
	lines.reserve(tetrahedrons.size() * 6);
	{
		int iLimit = tetrahedrons.size();
		for (int i = 0; i < iLimit; i++)
		{
			lines.push_back(tetrahedrons[i].lines[0]);
			lines.push_back(tetrahedrons[i].lines[1]);
			lines.push_back(tetrahedrons[i].lines[2]);
			lines.push_back(tetrahedrons[i].lines[3]);
			lines.push_back(tetrahedrons[i].lines[4]);
			lines.push_back(tetrahedrons[i].lines[5]);
		}
	}
	{
		int iLimit = lines.size();
		for (int i = 0; i < iLimit; i++)
		{
			points[i] = lines[i].point;
			directions[i] = lines[i].direction;
		}
	}
	cl::Buffer pointBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4) * lines.size(), points);
	cl::Buffer directionBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4) * lines.size(), directions);
	cl::Buffer stateBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(char) * lines.size());
	cl::Buffer matrixBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::mat4), camera.GetTransformValuePtr());
	cl::Buffer positionBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4), glm::value_ptr(camera.position));

	crossSection.SetVariable(0, pointBuffer);
	crossSection.SetVariable(1, directionBuffer);
	crossSection.SetVariable(2, stateBuffer);
	crossSection.SetVariable(3, matrixBuffer);
	crossSection.SetVariable(4, positionBuffer);
	crossSection.LaunchKernel(0, 0, lines.size());
	crossSection.ReadKernel(pointBuffer, GL_TRUE, 0, sizeof(glm::vec4) * lines.size(), points);
	crossSection.ReadKernel(stateBuffer, GL_TRUE, 0, sizeof(char) * lines.size(), states);

	vertexPos.clear();
	vertexCol.clear();

	{
		int tLimit = tetrahedrons.size();
		for (int t = 0; t < tLimit; t++)
		{
			//Make a Deque to hold the results. Sort results to Right and left and add to the lists. Sort the lists then combine them. Create triangles from the combinations
			std::deque<glm::vec4> vertexResults;

			if (states[(t * 6)] == 0)
			{
				vertexResults.push_back(points[(t * 6)]);
			}
			if (states[(t * 6) + 1] == 0)
			{
				vertexResults.push_back(points[(t * 6) + 1]);
			}
			if (states[(t * 6) + 2] == 0)
			{
				vertexResults.push_back(points[(t * 6) + 2]);
			}
			if (states[(t * 6) + 3] == 0)
			{
				vertexResults.push_back(points[(t * 6) + 3]);
			}
			if (states[(t * 6) + 4] == 0)
			{
				vertexResults.push_back(points[(t * 6) + 4]);
			}
			if (states[(t * 6) + 5] == 0)
			{
				vertexResults.push_back(points[(t * 6) + 5]);
			}

			glm::vec4 centroid(0);
			std::vector<SortItem> rightVertices;
			std::vector<SortItem> leftVertices;
			int vectorSize = vertexResults.size();
			for (int i = 0; i < vectorSize; i++)
			{
				centroid += vertexResults[i];
			}
			centroid /= vectorSize;
			for (int i = 0; i < vectorSize; i++)
			{
				glm::vec4& vertex = vertexResults[i];
				vertex -= centroid;
				if (vertex.x > 0)
				{
					float mag = (vertex.x*vertex.x) + (vertex.y*vertex.y);
					rightVertices.push_back(SortItem(i, (vertex.y * std::abs(vertex.y)) / mag)); //Sin(theta)^2 = cross/magnitude. Cross of point and (1,0). Sin is invertable between -90 and 90 deg. Cross is x * |x|
				}
				else
				{
					float mag = (vertex.x*vertex.x) + (vertex.y*vertex.y);
					leftVertices.push_back(SortItem(i, (vertex.y * std::abs(vertex.y)) / mag));
				}
				//Add to sides with values and ID
			}

			//Sort each side
			std::vector<glm::vec4> orderedVertices;
			orderedVertices.reserve(vertexResults.size());
			InsertSort(rightVertices);
			InsertSort(leftVertices);
			vectorSize = rightVertices.size();
			for (int i = 0; i < vectorSize; i++)
			{
				orderedVertices.push_back(vertexResults[rightVertices[i].ID] + centroid);
			}
			vectorSize = leftVertices.size();
			for (int i = vectorSize-1; i >= 0; i--)
			{
				orderedVertices.push_back(vertexResults[leftVertices[i].ID] + centroid);
			}
			vectorSize = orderedVertices.size() - 2;
			for (int i = 0; i < vectorSize; i++)
			{
				vertexPos.push_back(orderedVertices[0]);
				vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f,1.0f));
				vertexPos.push_back(orderedVertices[i+1]);
				vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				vertexPos.push_back(orderedVertices[i+2]);
				vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}
	float* vboData = new float[vertexPos.size() * 7];

	for (int i = 0; i < vertexPos.size(); i++)
	{
		vboData[0 + (i * 7)] = vertexPos[i].x;
		vboData[1 + (i * 7)] = vertexPos[i].y;
		vboData[2 + (i * 7)] = vertexPos[i].z;
		vboData[3 + (i * 7)] = 1;
		vboData[4 + (i * 7)] = vertexCol[i].x;
		vboData[5 + (i * 7)] = vertexCol[i].y;
		vboData[6 + (i * 7)] = vertexCol[i].z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBOaddress);
	glBufferData(GL_ARRAY_BUFFER, 7 * sizeof(float) * vertexPos.size(), vboData, GL_DYNAMIC_DRAW);

	delete[] vboData;
	return vertexPos.size();
}
*/
/*
unsigned int RenderManager::SetBuffer(Camera& camera, unsigned int VBOaddress)
{
	faceColor = 0;
	crossSection.SetFunction("CrossSection");

	if (points != nullptr)
		delete[] points;
	if (directions != nullptr)
		delete[] directions;
	if (magnitude != nullptr)
		delete[] magnitude;
	points = new glm::vec4[triangles.size() * 3];
	directions = new glm::vec4[triangles.size() * 3];
	magnitude = new float[triangles.size() * 3];

	std::vector<Line> lines;
	lines.reserve(triangles.size() * 3);
	{
		int iLimit = triangles.size();
		for (int i = 0; i < iLimit; i++)
		{
			lines.push_back(triangles[i].lines[0]);
			lines.push_back(triangles[i].lines[1]);
			lines.push_back(triangles[i].lines[2]);
		}
	}
	{
		int iLimit = lines.size();
		for (int i = 0; i < iLimit; i++)
		{
			points[i] = lines[i].point;
			directions[i] = lines[i].direction;
		}
	}
	//perhaps perspect here?
	cl::Buffer pointBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4) * lines.size(), points);
	cl::Buffer directionBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4) * lines.size(), directions);
	cl::Buffer magnitudeBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(float) * lines.size());
	cl::Buffer matrixBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::mat4), camera.GetTransformValuePtr());
	cl::Buffer positionBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4), glm::value_ptr(camera.position));

	crossSection.SetVariable(0, pointBuffer);
	crossSection.SetVariable(1, directionBuffer);
	crossSection.SetVariable(2, magnitudeBuffer);
	crossSection.SetVariable(3, matrixBuffer);
	crossSection.SetVariable(4, positionBuffer);
	crossSection.LaunchKernel(0, 0, lines.size());
	crossSection.ReadKernel(pointBuffer, GL_TRUE, 0, sizeof(glm::vec4) * lines.size(), points);
	crossSection.ReadKernel(magnitudeBuffer, GL_TRUE, 0, sizeof(float) * lines.size(), magnitude);

	vertexPos.clear();
	vertexCol.clear();

	{
		int tLimit = triangles.size();
		for (int t = 0; t < tLimit; t++)
		{
			if ((magnitude[(t * 3)] >= 0 && magnitude[(t * 3)] <= 1)\
				&& (magnitude[(t * 3) + 1] >= 0 && magnitude[(t * 3) + 1] <= 1) && (magnitude[(t * 3) + 1] >= 0 && magnitude[(t * 3) + 1] <= 1))
			{
				vertexPos.push_back(points[(t * 3)]);
				vertexPos.push_back(points[(t * 3) + 1]);
				vertexPos.push_back(points[(t * 3) + 2]);
				switch (faceColor)
				{
				case 0:
				case 1:
					vertexCol.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
					vertexCol.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
					vertexCol.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
					break;
				case 2:
				case 3:
					vertexCol.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
					vertexCol.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
					vertexCol.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
					break;
				case 4:
				case 5:
					vertexCol.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
					vertexCol.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
					vertexCol.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
					break;
				case 6:
				case 7:
					vertexCol.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
					vertexCol.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
					vertexCol.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
					break;
				case 8:
				case 9:
					vertexCol.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
					vertexCol.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
					vertexCol.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
					break;
				case 10:
				case 11:
					vertexCol.push_back(glm::vec3(0.0f, 1.0f, 1.0f));
					vertexCol.push_back(glm::vec3(0.0f, 1.0f, 1.0f));
					vertexCol.push_back(glm::vec3(0.0f, 1.0f, 1.0f));
					break;
				}
			}
			faceColor = Modulo(faceColor + 2, 12);
		}
	}
	float* vboData = new float[vertexPos.size() * 7];

	for (int i = 0; i < vertexPos.size(); i++)
	{
		vboData[0 + (i * 7)] = vertexPos[i].x;
		vboData[1 + (i * 7)] = vertexPos[i].y;
		vboData[2 + (i * 7)] = vertexPos[i].z;
		vboData[3 + (i * 7)] = 1;
		vboData[4 + (i * 7)] = vertexCol[i].x;
		vboData[5 + (i * 7)] = vertexCol[i].y;
		vboData[6 + (i * 7)] = vertexCol[i].z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBOaddress);
	glBufferData(GL_ARRAY_BUFFER, 7 * sizeof(float) * vertexPos.size(), vboData, GL_DYNAMIC_DRAW);

	delete[] vboData;
	return vertexPos.size();
}
*/
