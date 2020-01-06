#include "RenderObjects.h"

//Textures
Texture::Texture()
{
	id = 0;
	usesAlpha = false;
}

void Texture::BindTexture(int textureNo)
{
	switch (textureNo)
	{
	case 0:
		glActiveTexture(GL_TEXTURE0);
		break;
	case 1:
		glActiveTexture(GL_TEXTURE1);
		break;
	case 2:
		glActiveTexture(GL_TEXTURE2);
		break;
	case 3:
		glActiveTexture(GL_TEXTURE3);
		break;
	case 4:
		glActiveTexture(GL_TEXTURE4);
		break;
	case 5:
		glActiveTexture(GL_TEXTURE5);
		break;
	case 6:
		glActiveTexture(GL_TEXTURE6);
		break;
	case 7:
		glActiveTexture(GL_TEXTURE7);
		break;
	case 8:
		glActiveTexture(GL_TEXTURE8);
		break;
	case 9:
		glActiveTexture(GL_TEXTURE9);
		break;
	case 10:
		glActiveTexture(GL_TEXTURE10);
		break;
	case 11:
		glActiveTexture(GL_TEXTURE11);
		break;
	case 12:
		glActiveTexture(GL_TEXTURE12);
		break;
	case 13:
		glActiveTexture(GL_TEXTURE13);
		break;
	case 14:
		glActiveTexture(GL_TEXTURE14);
		break;
	case 15:
		glActiveTexture(GL_TEXTURE15);
		break;
	default:
		return;
		break;
	}
	glBindTexture(GL_TEXTURE_3D, id);

}
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
	textureID = 0;
	tetraCount = 1;
}

Renderable::Renderable(glm::mat4x4 transformationMatrix, glm::vec4 offsetVec, int modelIDInt, int textureIDInt, int tetraCountInt)
{
	transformation = transformationMatrix;
	offset = offsetVec;
	modelID = modelIDInt;
	textureID = textureIDInt;
	tetraCount = tetraCountInt;
}
//RenderManager
RenderManager::RenderManager()
{
	crossSection = GPUProgram("Assets\\Kernels\\CrossSection.cl");
	crossSection.SetFunction(0, "CrossSection");
	crossSection.CreateProgram("Assets\\Kernels\\TetraRenderer.cl");
	crossSection.SetFunction(1, "MakeFace");
	models.resize(24);
	modelStarts.resize(3);
	//Red Gradient
	//pentachoronModel[0] = Tetrahedron(0,0,0,0, 0,0,0, 1,0,0,0, 0,0,0, 0,1,0,0, 0,0,0, 0,0,1,0, 0,0,0); 
	//pentachoronModel[1] = Tetrahedron(0,0,0,1, 1,0,0, 1,0,0,0, 0,0,0, 0,1,0,0, 0,0,0, 0,0,1,0, 0,0,0);
	//pentachoronModel[2] = Tetrahedron(0,0,0,0, 0,0,0, 0,0,0,1, 1,0,0, 0,1,0,0, 0,0,0, 0,0,1,0, 0,0,0);
	//pentachoronModel[3] = Tetrahedron(0,0,0,0, 0,0,0, 1,0,0,0, 0,0,0, 0,0,0,1, 1,0,0, 0,0,1,0, 0,0,0);
	//pentachoronModel[4] = Tetrahedron(0,0,0,0, 0,0,0, 1,0,0,0, 0,0,0, 0,1,0,0, 0,0,0, 0,0,0,1, 1,0,0);
	float phi = 0.5f + sqrtf(1.25f);
	//celestial Colors
	models[5] = Tetrahedron(2,0,0,0, 0,0,0, 0,2,0,0, 1,0,0, 0,0,2,0, 0,1,0, 0,0,0,2, 0,0,1);
	models[6] = Tetrahedron(phi,phi,phi,phi, 0,0,0, 0,2,0,0, 1,0,0, 0,0,2,0, 0,1,0, 0,0,0,2, 0,0,1);
	models[7] = Tetrahedron(2,0,0,0, 0,0,0, phi,phi,phi,phi, 1,0,0, 0,0,2,0, 0,1,0, 0,0,0,2, 0,0,1);
	models[8] = Tetrahedron(2,0,0,0, 0,0,0, 0,2,0,0, 1,0,0, phi,phi,phi,phi, 0,1,0, 0,0,0,2, 0,0,1);
	models[9] = Tetrahedron(2,0,0,0, 0,0,0, 0,2,0,0, 1,0,0, 0,0,2,0, 0,1,0, phi,phi,phi,phi, 0,0,1);

	//RGB Colors
	//pentachoronModel[0] = Tetrahedron(0,0,0,0, 1,1,1, 1,0,0,0, 1,0,0, 0,1,0,0, 0,1,0, 0,0,1,0, 0,0,1);
	//pentachoronModel[1] = Tetrahedron(0,0,0,1, 1,1,1, 1,0,0,0, 1,0,0, 0,1,0,0, 0,1,0, 0,0,1,0, 0,0,1);
	//pentachoronModel[2] = Tetrahedron(0,0,0,0, 1,1,1, 0,0,0,1, 1,0,0, 0,1,0,0, 0,1,0, 0,0,1,0, 0,0,1);
	//pentachoronModel[3] = Tetrahedron(0,0,0,0, 1,1,1, 1,0,0,0, 1,0,0, 0,0,0,1, 0,1,0, 0,0,1,0, 0,0,1);
	//pentachoronModel[4] = Tetrahedron(0,0,0,0, 1,1,1, 1,0,0,0, 1,0,0, 0,1,0,0, 0,1,0, 0,0,0,1, 0,0,1);

	//Stripes
	//pentachoronModel[0] = Tetrahedron(0,0,0,0, 0,0,0, 1,0,0,0, 0,1,0, 0,1,0,0, 0,0,1, 0,0,1,0, 0,0,0);
	//pentachoronModel[1] = Tetrahedron(0,0,0,1, 1,0,0, 1,0,0,0, 0,1,0, 0,1,0,0, 0,0,1, 0,0,1,0, 0,0,0);
	//pentachoronModel[2] = Tetrahedron(0,0,0,0, 0,1,0, 0,0,0,1, 1,0,0, 0,1,0,0, 0,0,1, 0,0,1,0, 0,0,0);
	//pentachoronModel[3] = Tetrahedron(0,0,0,0, 0,0,1, 1,0,0,0, 0,1,0, 0,0,0,1, 1,0,0, 0,0,1,0, 0,0,0);
	//pentachoronModel[4] = Tetrahedron(0,0,0,0, 0,0,0, 1,0,0,0, 0,1,0, 0,1,0,0, 0,0,1, 0,0,0,1, 1,0,0);

	//0.5 Stripes
	//pentachoronModel[0] = Tetrahedron(0,0,0,0, 0,0,0, 1,0,0,0, 0,0.5f,0, 0,1,0,0, 0,0,0.5f, 0,0,1,0, 0,0,0);
	//pentachoronModel[1] = Tetrahedron(0,0,0,1, 0.5f,0,0, 1,0,0,0, 0,0.5f,0, 0,1,0,0, 0,0,0.5f, 0,0,1,0, 0,0,0);
	//pentachoronModel[2] = Tetrahedron(0,0,0,0, 0,0.5f,0, 0,0,0,1, 0.5f,0,0, 0,1,0,0, 0,0,0.5f, 0,0,1,0, 0,0,0);
	//pentachoronModel[3] = Tetrahedron(0,0,0,0, 0,0,0.5f, 1,0,0,0, 0,0.5f,0, 0,0,0,1, 0.5f,0,0, 0,0,1,0, 0,0,0);
	//pentachoronModel[4] = Tetrahedron(0,0,0,0, 0,0,0, 1,0,0,0, 0,0.5f,0, 0,1,0,0, 0,0,0.5f, 0,0,0,1, 0.5f,0,0);

	//2 Stripes
	//pentachoronModel[0] = Tetrahedron(0,0,0,0, 0,0,0, 1,0,0,0, 0,2,0, 0,1,0,0, 0,0,2, 0,0,1,0, 0,0,0);
	//pentachoronModel[1] = Tetrahedron(0,0,0,1, 2,0,0, 1,0,0,0, 0,2,0, 0,1,0,0, 0,0,2, 0,0,1,0, 0,0,0);
	//pentachoronModel[2] = Tetrahedron(0,0,0,0, 0,2,0, 0,0,0,1, 2,0,0, 0,1,0,0, 0,0,2, 0,0,1,0, 0,0,0);
	//pentachoronModel[3] = Tetrahedron(0,0,0,0, 0,0,2, 1,0,0,0, 0,2,0, 0,0,0,1, 2,0,0, 0,0,1,0, 0,0,0);
	//pentachoronModel[4] = Tetrahedron(0,0,0,0, 0,0,0, 1,0,0,0, 0,2,0, 0,1,0,0, 0,0,2, 0,0,0,1, 2,0,0);

	//Cell Color
	models[0] = Tetrahedron(0,0,0,0, 1/7.0f,0,0, 2,0,0,0, 1/7.0f,0,0, 0,2,0,0, 1/7.0f,0,0, 0,0,2,0, 1/7.0f,0,0);
	models[1] = Tetrahedron(0,0,0,2, 2/7.0f,0,0, 2,0,0,0, 2/7.0f,0,0, 0,2,0,0, 2/7.0f,0,0, 0,0,2,0, 2/7.0f,0,0);
	models[2] = Tetrahedron(0,0,0,0, 4/7.0f,0,0, 0,0,0,2, 4/7.0f,0,0, 0,2,0,0, 4/7.0f,0,0, 0,0,2,0, 4/7.0f,0,0);
	models[3] = Tetrahedron(0,0,0,0, 3/7.0f,0,0, 2,0,0,0, 3/7.0f,0,0, 0,0,0,2, 3/7.0f,0,0, 0,0,2,0, 3/7.0f,0,0);
	models[4] = Tetrahedron(0,0,0,0, 5/7.0f,0,0, 2,0,0,0, 5/7.0f,0,0, 0,2,0,0, 5/7.0f,0,0, 0,0,0,2, 5/7.0f,0,0);

	std::vector<Tetrahedron> holdTetra = std::vector<Tetrahedron>(3);
	Line baseLines[4] = {Line(0,0,0,0,0,0,0), Line(1,0,0,0,0,1,0), Line(0,1,0,0,0,0,1), Line(0,0,1,0,0,0,0)};
	models[10] = Tetrahedron(0,0,0,0, 0,0,0, 1,0,0,0, 0,1,0, 0,1,0,0, 0,0,1, 0,0,1,0, 0,0,0);
	models[11] = Tetrahedron(0,0,0,0, 0,0,0, 1,0,0,0, 0,1,0, 0,1,0,0, 0,0,1, 0,0,1,0, 0,0,0);
	models[11].Translate(0, 0, 0, 1);
	PrismifyByVec(baseLines[0], baseLines[1], baseLines[2], glm::vec4(0, 0, 0, 1), glm::vec3(1, 0, 0), holdTetra);
	models[12] = holdTetra[0];
	models[13] = holdTetra[1];
	models[14] = holdTetra[2];
	PrismifyByVec(baseLines[3], baseLines[1], baseLines[2], glm::vec4(0, 0, 0, 1), glm::vec3(1, 0, 0), holdTetra);
	models[15] = holdTetra[0];
	models[16] = holdTetra[1];
	models[17] = holdTetra[2];
	PrismifyByVec(baseLines[0], baseLines[3], baseLines[2], glm::vec4(0, 0, 0, 1), glm::vec3(1, 0, 0), holdTetra);
	models[18] = holdTetra[0];
	models[19] = holdTetra[1];
	models[20] = holdTetra[2];
	PrismifyByVec(baseLines[0], baseLines[1], baseLines[3], glm::vec4(0, 0, 0, 1), glm::vec3(1, 0, 0), holdTetra);
	models[21] = holdTetra[0];
	models[22] = holdTetra[1];
	models[23] = holdTetra[2];

	modelStarts[0] = 0;
	modelStarts[1] = 5;
	modelStarts[2] = 10;

	Line* modelLines = new Line[models.size()*6];
	for (int i = 0; i < models.size(); i++) 
	{
		for (int l = 0; l < 6; l++)
		{
			modelLines[(i * 6) + l] = models[i].lines[l];
		}
	}
	modelBuffer = cl::Buffer(crossSection.context, CL_MEM_HOST_WRITE_ONLY | CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(Tetrahedron)*models.size(), modelLines);
	delete[] modelLines;
}

void RenderManager::SetBuffer(Camera& camera, unsigned int* VBOaddress, int textureCount, unsigned int* bufferSizes)
{
	int modelSize = renderables.size();
	if (modelSize == 0)
	{
		for (int i = 0; i < textureCount; i++)
		{
			bufferSizes[i] = 0;
		}
	}
	std::deque<glm::vec4>* vertexPos = new std::deque<glm::vec4>[textureCount];
	std::deque<glm::vec3>* vertexCol = new std::deque<glm::vec3>[textureCount];
	std::deque<glm::vec3>* vertexTex = new std::deque<glm::vec3>[textureCount];
#pragma region Kernel setup and launch
	glm::mat4x4* modelMatrices = new glm::mat4x4[modelSize];
	glm::vec4* modelOffsets = new glm::vec4[modelSize];

	int tetraSize = 0;
	for (int i = 0; i < modelSize; i++)
	{
		tetraSize += renderables[i].tetraCount;
	}
	int lineSize = tetraSize * 6;

	int* modelIDs = new int[tetraSize]; ///Used later as well
	int* tetraIDs = new int[tetraSize];

	glm::vec4* points = new glm::vec4[lineSize];
	char* states = new char[lineSize];
	glm::vec4* texCoords = new glm::vec4[lineSize];

	{
		int index = 0;
		for (int i = 0; i < modelSize; i++)
		{
			Renderable& renderable = renderables[i];
			modelMatrices[i] = renderable.transformation;
			modelOffsets[i] = renderable.offset;
			for (int t = 0; t < renderable.tetraCount; t++)
			{
				modelIDs[index] = i;
				tetraIDs[index] = modelStarts[renderable.modelID] + t;
				index++;
			}
		}
	}
	cl::Buffer modelMatricesBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::mat4x4)*modelSize, modelMatrices);
	cl::Buffer modelOffsetsBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4)*modelSize, modelOffsets);
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
	delete[]  tetraIDs;
#pragma endregion
	crossSection.ReadKernel(0, pointBuffer, GL_TRUE, 0, sizeof(glm::vec4) * lineSize, points);
	crossSection.ReadKernel(0, stateBuffer, GL_TRUE, 0, sizeof(char) * lineSize, states); // 8 ms
	crossSection.ReadKernel(0, texCoordBuffer, GL_TRUE, 0, sizeof(glm::vec4) * lineSize, texCoords);
#pragma region tetrahedron kernel
	if (tetraSize > 0)
	{
		char* tetraStates = new char[tetraSize];
		cl::Buffer tetraStateBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(char)*tetraSize);
		crossSection.SetVariable(1, 0, pointBuffer);
		crossSection.SetVariable(1, 1, stateBuffer);
		crossSection.SetVariable(1, 2, texCoordBuffer);
		crossSection.SetVariable(1, 3, tetraStateBuffer);
		crossSection.LaunchKernel(1, 0, tetraSize);
		crossSection.ReadKernel(1, pointBuffer, GL_TRUE, 0, sizeof(glm::vec4) * lineSize, points);
		crossSection.ReadKernel(1, texCoordBuffer, GL_TRUE, 0, sizeof(glm::vec4) * lineSize, texCoords);
		crossSection.ReadKernel(1, tetraStateBuffer, GL_TRUE, 0, sizeof(char)*tetraSize, tetraStates);
		for (int i = 0; i < tetraSize; i++)
		{
			if (tetraStates[i] >= 3)
			{
				int checkSize = tetraStates[i] - 2;
				for (int t = 0; t < checkSize; t++)
				{
					int tID = renderables[modelIDs[i]].textureID;
					vertexPos[tID].push_back(points[(i * 6) + 0]);
					vertexCol[tID].push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					vertexTex[tID].push_back(texCoords[(i * 6) + 0]);
					vertexPos[tID].push_back(points[(i * 6) + 1 + t]);
					vertexCol[tID].push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					vertexTex[tID].push_back(texCoords[(i * 6) + 1 + t]);
					vertexPos[tID].push_back(points[(i * 6) + 2 + t]);
					vertexCol[tID].push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					vertexTex[tID].push_back(texCoords[(i * 6) + 2 + t]);
				}
			}
		} //7 ms
		delete[] tetraStates;
	}
#pragma endregion

#pragma region Buffer setup
	int bufferSize = 0;
	for (int i = 0; i < textureCount; i++)
	{
		bufferSizes[i] = vertexPos[i].size();
		if (vertexPos[i].size() > bufferSize)
			bufferSize = vertexPos[i].size();
	}
	float* vboData = new float[bufferSize * 9];

	for (int i = 0; i < textureCount; i++)
	{
		for (int v = 0; v < vertexPos[i].size(); v++)
		{
			vboData[0 + (v * 9)] = vertexPos[i][v].x;
			vboData[1 + (v * 9)] = vertexPos[i][v].y;
			vboData[2 + (v * 9)] = vertexPos[i][v].z;
			vboData[3 + (v * 9)] = vertexCol[i][v].x;
			vboData[4 + (v * 9)] = vertexCol[i][v].y;
			vboData[5 + (v * 9)] = vertexCol[i][v].z;
			vboData[6 + (v * 9)] = vertexTex[i][v].x;
			vboData[7 + (v * 9)] = vertexTex[i][v].y;
			vboData[8 + (v * 9)] = vertexTex[i][v].z;
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBOaddress[i]);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float) * vertexPos[i].size(), vboData, GL_DYNAMIC_DRAW);
	}
#pragma endregion
	delete[]  modelIDs;
	delete[] vboData;
	delete[] points;
	delete[] states;
	delete[] texCoords;
	delete[] vertexPos;
	delete[] vertexCol;
	delete[] vertexTex;
}
//Cube
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
*/