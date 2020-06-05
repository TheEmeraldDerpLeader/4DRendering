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
Rotater::Rotater()
{
	rotation[0] = 0;
	rotation[1] = 0;
	rotation[2] = 0;
	rotation[3] = 0;
	rotation[4] = 0;
	rotation[5] = 0;
}

Rotater::Rotater(float xy, float yz, float zx, float xw, float yw, float zw)
{
	rotation[0] = xy;
	rotation[1] = yz;
	rotation[2] = zx;
	rotation[3] = xw;
	rotation[4] = yw;
	rotation[5] = zw;
}

glm::mat4x4 Rotater::GetTransform()
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

float* Rotater::GetTransformValuePtr()
{
	GetTransform();
	return glm::value_ptr(copyMatrix);
}

void Rotater::RotateXY(float degrees)
{
	rotation[0] += degrees;
}

void Rotater::RotateYZ(float degrees)
{
	rotation[1] += degrees;
}

void Rotater::RotateZX(float degrees)
{
	rotation[2] += degrees;
}

void Rotater::RotateXW(float degrees)
{
	rotation[3] += degrees;
}

void Rotater::RotateYW(float degrees)
{
	rotation[4] += degrees;
}

void Rotater::RotateZW(float degrees)
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
	polyCount = 1;
	transparent = false;
}

Renderable::Renderable(glm::mat4x4 transformationMatrix, glm::vec4 offsetVec, int modelIDInt, int textureIDInt, int polyCountInt, bool transparentBool)
{
	transformation = transformationMatrix;
	offset = offsetVec;
	modelID = modelIDInt;
	textureID = textureIDInt;
	polyCount = polyCountInt;
	transparent = transparentBool;
}
//RenderManager
RenderManager::RenderManager()
{
	crossSection = GPUProgram();
	crossSection.CreateProgram("Assets\\Kernels\\TetraCrossSection.cl");
	crossSection.SetFunction(0, "CrossSection");
	crossSection.CreateProgram("Assets\\Kernels\\TetraRenderer.cl");
	crossSection.SetFunction(1, "MakeFace");
	crossSection.CreateProgram("Assets\\Kernels\\DynamicCrossSection.cl");
	crossSection.SetFunction(2, "DynCrossSection");
	crossSection.CreateProgram("Assets\\Kernels\\HexaCrossSection.cl");
	crossSection.SetFunction(3, "HexaCrossSection");
	crossSection.CreateProgram("Assets\\Kernels\\HexaRenderer.cl");
	crossSection.SetFunction(4, "HexaMakeFace");

	tetraModels.resize(24);
	tetraModelStarts.resize(3);
#pragma region Tetra Models
	//Red Gradient
	//pentachoronModel[0] = Tetrahedron(0,0,0,0, 0,0,0, 1,0,0,0, 0,0,0, 0,1,0,0, 0,0,0, 0,0,1,0, 0,0,0); 
	//pentachoronModel[1] = Tetrahedron(0,0,0,1, 1,0,0, 1,0,0,0, 0,0,0, 0,1,0,0, 0,0,0, 0,0,1,0, 0,0,0);
	//pentachoronModel[2] = Tetrahedron(0,0,0,0, 0,0,0, 0,0,0,1, 1,0,0, 0,1,0,0, 0,0,0, 0,0,1,0, 0,0,0);
	//pentachoronModel[3] = Tetrahedron(0,0,0,0, 0,0,0, 1,0,0,0, 0,0,0, 0,0,0,1, 1,0,0, 0,0,1,0, 0,0,0);
	//pentachoronModel[4] = Tetrahedron(0,0,0,0, 0,0,0, 1,0,0,0, 0,0,0, 0,1,0,0, 0,0,0, 0,0,0,1, 1,0,0);
	float phi = 0.5f + sqrtf(1.25f);
	//celestial Colors
	tetraModels[5] = Tetrahedron(2,0,0,0, 0,0,0, 0,2,0,0, 1,0,0, 0,0,2,0, 0,1,0, 0,0,0,2, 0,0,1);
	tetraModels[6] = Tetrahedron(phi,phi,phi,phi, 0,0,0, 0,2,0,0, 1,0,0, 0,0,2,0, 0,1,0, 0,0,0,2, 0,0,1);
	tetraModels[7] = Tetrahedron(2,0,0,0, 0,0,0, phi,phi,phi,phi, 1,0,0, 0,0,2,0, 0,1,0, 0,0,0,2, 0,0,1);
	tetraModels[8] = Tetrahedron(2,0,0,0, 0,0,0, 0,2,0,0, 1,0,0, phi,phi,phi,phi, 0,1,0, 0,0,0,2, 0,0,1);
	tetraModels[9] = Tetrahedron(2,0,0,0, 0,0,0, 0,2,0,0, 1,0,0, 0,0,2,0, 0,1,0, phi,phi,phi,phi, 0,0,1);

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
	tetraModels[0] = Tetrahedron(0,0,0,0, 1/7.0f,0,0, 2,0,0,0, 1/7.0f,0,0, 0,2,0,0, 1/7.0f,0,0, 0,0,2,0, 1/7.0f,0,0);
	tetraModels[1] = Tetrahedron(0,0,0,2, 2/7.0f,0,0, 2,0,0,0, 2/7.0f,0,0, 0,2,0,0, 2/7.0f,0,0, 0,0,2,0, 2/7.0f,0,0);
	tetraModels[2] = Tetrahedron(0,0,0,0, 4/7.0f,0,0, 0,0,0,2, 4/7.0f,0,0, 0,2,0,0, 4/7.0f,0,0, 0,0,2,0, 4/7.0f,0,0);
	tetraModels[3] = Tetrahedron(0,0,0,0, 3/7.0f,0,0, 2,0,0,0, 3/7.0f,0,0, 0,0,0,2, 3/7.0f,0,0, 0,0,2,0, 3/7.0f,0,0);
	tetraModels[4] = Tetrahedron(0,0,0,0, 5/7.0f,0,0, 2,0,0,0, 5/7.0f,0,0, 0,2,0,0, 5/7.0f,0,0, 0,0,0,2, 5/7.0f,0,0);

	std::vector<Tetrahedron> holdTetra = std::vector<Tetrahedron>(3);
	Line baseLines1[4] = {Line(0,0,0,0,0,0,0), Line(1,0,0,0,0,1,0), Line(0,1,0,0,0,0,1), Line(0,0,1,0,0,0,0)};
	Line baseLines2[4] = {Line(0,0,0,1,1,0,0), Line(1,0,0,1,1,1,0), Line(0,1,0,1,1,0,1), Line(0,0,1,1,1,0,0)};
	tetraModels[10] = Tetrahedron(0,0,0,0, 0,0,0, 1,0,0,0, 0,1,0, 0,1,0,0, 0,0,1, 0,0,1,0, 0,0,0);
	tetraModels[11] = Tetrahedron(0,0,0,0, 1,0,0, 1,0,0,0, 1,1,0, 0,1,0,0, 1,0,1, 0,0,1,0, 1,0,0);
	tetraModels[11].Translate(0, 0, 0, 1);
	//PrismifyByVec(baseLines[0], baseLines[1], baseLines[2], glm::vec4(0, 0, 0, 1), glm::vec3(1, 0, 0), holdTetra);
	PrismifyByTri(baseLines1[0], baseLines1[1], baseLines1[2], baseLines2[0], baseLines2[1], baseLines2[2], holdTetra);
	tetraModels[12] = holdTetra[0];
	tetraModels[13] = holdTetra[1];
	tetraModels[14] = holdTetra[2];
	PrismifyByTri(baseLines1[3], baseLines1[1], baseLines1[2], baseLines2[3], baseLines2[1], baseLines2[2], holdTetra);
	tetraModels[15] = holdTetra[0];
	tetraModels[16] = holdTetra[1];
	tetraModels[17] = holdTetra[2];
	PrismifyByTri(baseLines1[0], baseLines1[3], baseLines1[2], baseLines2[0], baseLines2[3], baseLines2[2], holdTetra);
	tetraModels[18] = holdTetra[0];
	tetraModels[19] = holdTetra[1];
	tetraModels[20] = holdTetra[2];
	PrismifyByTri(baseLines1[0], baseLines1[1], baseLines1[3], baseLines2[0], baseLines2[1], baseLines2[3], holdTetra);
	tetraModels[21] = holdTetra[0];
	tetraModels[22] = holdTetra[1];
	tetraModels[23] = holdTetra[2];
#pragma endregion
	tetraModelStarts[0] = 0;
	tetraModelStarts[1] = 5;
	tetraModelStarts[2] = 10;
	
	Line* modelLines = new Line[tetraModels.size()*6];
	for (int i = 0; i < tetraModels.size(); i++) 
	{
		for (int l = 0; l < 6; l++)
		{
			modelLines[(i * 6) + l] = tetraModels[i].lines[l];
		}
	}
	tetraModelBuffer = cl::Buffer(crossSection.context, CL_MEM_HOST_WRITE_ONLY | CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(Tetrahedron)*tetraModels.size(), modelLines);
	delete modelLines;

	hexaModels.resize(8);
	hexaModelStarts.resize(1);
#pragma region Hexa Models
	glm::vec4 cubeP[8] = { glm::vec4(-1,-1,-1,0),glm::vec4(1,-1,-1,0),glm::vec4(-1,1,-1,0),glm::vec4(1,1,-1,0),
	glm::vec4(-1,-1,1,0),glm::vec4(1,-1,1,0),glm::vec4(-1,1,1,0),glm::vec4(1,1,1,0)};
	//hexaModels[0] = Hexahedron(cubeP[0],cubeT[0], cubeP[1],cubeT[1], cubeP[3],cubeT[3], cubeP[2],cubeT[2], cubeP[4],cubeT[4], cubeP[5],cubeT[5], cubeP[7],cubeT[7], cubeP[6],cubeT[6]);
	//hexaModels[1] = Hexahedron(cubeP[8],cubeT[0], cubeP[9],cubeT[1], cubeP[11],cubeT[3], cubeP[10],cubeT[2], cubeP[12],cubeT[4], cubeP[13],cubeT[5], cubeP[15],cubeT[7], cubeP[14],cubeT[6]);
	//hexaModels[2] = Hexahedron(cubeP[0],cubeT[0], cubeP[8],cubeT[1], cubeP[10],cubeT[3], cubeP[2],cubeT[2], cubeP[4],cubeT[4], cubeP[12],cubeT[5], cubeP[14],cubeT[7], cubeP[6],cubeT[6]);
	//hexaModels[3] = Hexahedron(cubeP[1],cubeT[0], cubeP[9],cubeT[1], cubeP[11],cubeT[3], cubeP[3],cubeT[2], cubeP[5],cubeT[4], cubeP[13],cubeT[5], cubeP[15],cubeT[7], cubeP[7],cubeT[6]);
	//hexaModels[4] = Hexahedron(cubeP[0],cubeT[0], cubeP[1],cubeT[1], cubeP[9],cubeT[3], cubeP[8],cubeT[2], cubeP[4],cubeT[4], cubeP[5],cubeT[5], cubeP[13],cubeT[7], cubeP[12],cubeT[6]);
	//hexaModels[5] = Hexahedron(cubeP[2],cubeT[0], cubeP[3],cubeT[1], cubeP[11],cubeT[3], cubeP[10],cubeT[2], cubeP[6],cubeT[4], cubeP[7],cubeT[5], cubeP[15],cubeT[7], cubeP[14],cubeT[6]);
	//hexaModels[6] = Hexahedron(cubeP[0],cubeT[0], cubeP[1],cubeT[1], cubeP[3],cubeT[3], cubeP[2],cubeT[2], cubeP[8],cubeT[4], cubeP[9],cubeT[5], cubeP[11],cubeT[7], cubeP[10],cubeT[6]);
	//hexaModels[7] = Hexahedron(cubeP[4],cubeT[0], cubeP[5],cubeT[1], cubeP[7],cubeT[3], cubeP[6],cubeT[2], cubeP[12],cubeT[4], cubeP[13],cubeT[5], cubeP[15],cubeT[7], cubeP[14],cubeT[6]);
	for (int i = 0; i < 8; i++)
	{
		Rotater rotater;
		glm::vec4 posOffset;
		glm::vec3 texOffset;
		switch (i)
		{
		case 0:
			posOffset = glm::vec4(0, 0, 0, -1);
			texOffset = glm::vec3(0, 0, 0);
			rotater = Rotater(0, 0, 0, 0, 0, 0);
			break;
		case 1:
			posOffset = glm::vec4(0, 0, 0, 1);
			texOffset = glm::vec3(0.5f, 0, 0);
			rotater = Rotater(0, 0, 0, 0, 0, 0);
			break;
		case 2:
			posOffset = glm::vec4(-1, 0, 0, 0);
			texOffset = glm::vec3(0, 0.5f, 0);
			rotater = Rotater(0, 0, 0, 90, 0, 0);
			break;
		case 3:
			posOffset = glm::vec4(1, 0, 0, 0);
			texOffset = glm::vec3(0.5f, 0.5f, 0);
			rotater = Rotater(0, 0, 0, 90, 0, 0);
			break;
		case 4:
			posOffset = glm::vec4(0, -1, 0, 0);
			texOffset = glm::vec3(0, 0, 0.5f);
			rotater = Rotater(0, 0, 0, 0, 90, 0);
			break;
		case 5:
			posOffset = glm::vec4(0, 1, 0, 0);
			texOffset = glm::vec3(0.5f, 0, 0.5f);
			rotater = Rotater(0, 0, 0, 0, 90, 0);
			break;
		case 6:
			posOffset = glm::vec4(0, 0, -1, 0);
			texOffset = glm::vec3(0, 0.5f, 0.5f);
			rotater = Rotater(0, 0, 0, 0, 0, 90);
			break;
		case 7:
			posOffset = glm::vec4(0, 0, 1, 0);
			texOffset = glm::vec3(0.5f, 0.5f, 0.5f);
			rotater = Rotater(0, 0, 0, 0, 0, 90);
			break;
		}
		Hexahedron cube = Hexahedron(cubeP[0], texOffset+glm::vec3(0,0,0), cubeP[1], texOffset + glm::vec3(0.5f, 0, 0), cubeP[3], texOffset + glm::vec3(0.5f, 0.5f, 0), cubeP[2], texOffset + glm::vec3(0, 0.5f, 0),
			cubeP[4], texOffset + glm::vec3(0, 0, 0.5f), cubeP[5], texOffset + glm::vec3(0.5f, 0, 0.5f), cubeP[7], texOffset + glm::vec3(0.5f, 0.5f, 0.5f), cubeP[6], texOffset + glm::vec3(0, 0.5f, 0.5f));
		for (int l = 0; l < 12; l++)
		{
			cube.lines[l].TransformAround(rotater.GetTransform(), glm::vec4(0,0,0,0));
			cube.lines[l].Translate(posOffset.x, posOffset.y, posOffset.z, posOffset.w);
		}
		hexaModels[i] = cube;
	}
#pragma endregion
	hexaModelStarts[0] = 0;
	modelLines = new Line[hexaModels.size() * 12];
	for (int i = 0; i < hexaModels.size(); i++)
	{
		for (int l = 0; l < 12; l++)
		{
			modelLines[(i * 12) + l] = hexaModels[i].lines[l];
		}
	}
	hexaModelBuffer = cl::Buffer(crossSection.context, CL_MEM_HOST_WRITE_ONLY | CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(Hexahedron)*hexaModels.size(), modelLines);
	delete modelLines;
}

void RenderManager::ClearDeques(unsigned int textureCountInt, unsigned int transTextureCountInt)
{
	if (vertexPos != nullptr)
		delete[] vertexPos;
	if (vertexCol != nullptr)
		delete[] vertexCol;
	if (vertexTex != nullptr)
		delete[] vertexTex;
	if (transVertexPos != nullptr)
		delete[] transVertexPos;
	if (transVertexCol != nullptr)
		delete[] transVertexCol;
	if (transVertexTex != nullptr)
		delete[] transVertexTex;
	textureCount = textureCountInt;
	transTextureCount = transTextureCountInt;
	vertexPos = new std::deque<glm::vec4>[textureCount];
	vertexCol = new std::deque<glm::vec3>[textureCount];
	vertexTex = new std::deque<glm::vec3>[textureCount];
	transVertexPos = new std::deque<glm::vec4>[transTextureCount];
	transVertexCol = new std::deque<glm::vec3>[transTextureCount];
	transVertexTex = new std::deque<glm::vec3>[transTextureCount];
}

void RenderManager::ModelGenerate(Rotater& camera, glm::vec4 position)
{
	int modelSize = tetraRenderables.size();
	if (modelSize == 0)
		return;
#pragma region Kernel setup and launch
	//glm::mat4x4* modelMatrices = new glm::mat4x4[modelSize];
	//glm::vec4* modelOffsets = new glm::vec4[modelSize];

	int tetraSize = 0;
	for (int i = 0; i < modelSize; i++)
	{
		tetraSize += tetraRenderables[i].polyCount;
	}
	int lineSize = tetraSize * 6;

	//int* modelIDs = new int[tetraSize]; ///Used later as well
	//int* tetraIDs = new int[tetraSize];

	//glm::vec4* points = new glm::vec4[lineSize];
	//glm::vec4* texCoords = new glm::vec4[lineSize];

	int index = 0;
	for (int i = 0; i < modelSize; i++)
	{
		Renderable& renderable = tetraRenderables[i];
		modelMatrices[i] = renderable.transformation;
		modelOffsets[i] = renderable.offset;
		for (int t = 0; t < renderable.polyCount; t++)
		{
			modelIDs[index] = i;
			tetraIDs[index] = tetraModelStarts[renderable.modelID] + t;
			index++;
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
	cl::Buffer offsetBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4), glm::value_ptr(position));

	crossSection.SetVariable(0, 0, modelMatricesBuffer);
	crossSection.SetVariable(0, 1, modelOffsetsBuffer);
	crossSection.SetVariable(0, 2, modelIDBuffer);
	crossSection.SetVariable(0, 3, tetraIDBuffer);
	crossSection.SetVariable(0, 4, pointBuffer);
	crossSection.SetVariable(0, 5, stateBuffer);
	crossSection.SetVariable(0, 6, texCoordBuffer);
	crossSection.SetVariable(0, 7, matrixBuffer);
	crossSection.SetVariable(0, 8, offsetBuffer);
	crossSection.SetVariable(0, 9, tetraModelBuffer);
	crossSection.LaunchKernel(0, 0, tetraSize);

	//delete modelMatrices;
	//delete modelOffsets;
	//delete tetraIDs;
#pragma endregion
	crossSection.ReadKernel(0, pointBuffer, CL_TRUE, 0, sizeof(glm::vec4) * lineSize, points);
	crossSection.ReadKernel(0, texCoordBuffer, CL_TRUE, 0, sizeof(glm::vec4) * lineSize, texCoords);
#pragma region tetrahedron kernel
	if (tetraSize > 0)
	{
		//char* tetraStates = new char[tetraSize];
		cl::Buffer tetraStateBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(char)*tetraSize);
		crossSection.SetVariable(1, 0, pointBuffer);
		crossSection.SetVariable(1, 1, stateBuffer);
		crossSection.SetVariable(1, 2, texCoordBuffer);
		crossSection.SetVariable(1, 3, tetraStateBuffer);
		crossSection.LaunchKernel(1, 0, tetraSize);
		crossSection.ReadKernel(1, pointBuffer, CL_TRUE, 0, sizeof(glm::vec4) * lineSize, points);
		crossSection.ReadKernel(1, texCoordBuffer, CL_TRUE, 0, sizeof(glm::vec4) * lineSize, texCoords);
		crossSection.ReadKernel(1, tetraStateBuffer, CL_TRUE, 0, sizeof(char)*tetraSize, tetraStates);
		for (int i = 0; i < tetraSize; i++)
		{
			if (tetraStates[i] >= 3)
			{
				int tID = tetraRenderables[modelIDs[i]].textureID;
				int checkSize = tetraStates[i] - 2;
				if (!tetraRenderables[modelIDs[i]].transparent)
				{
					for (int t = 0; t < checkSize; t++)
					{
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
				else
				{
					for (int t = 0; t < checkSize; t++)
					{
						transVertexPos[tID].push_back(points[(i * 6) + 0]);
						transVertexCol[tID].push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
						transVertexTex[tID].push_back(texCoords[(i * 6) + 0]);
						transVertexPos[tID].push_back(points[(i * 6) + 1 + t]);
						transVertexCol[tID].push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
						transVertexTex[tID].push_back(texCoords[(i * 6) + 1 + t]);
						transVertexPos[tID].push_back(points[(i * 6) + 2 + t]);
						transVertexCol[tID].push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
						transVertexTex[tID].push_back(texCoords[(i * 6) + 2 + t]);
					}
				}
			}
		}
		//delete tetraStates;
	}
#pragma endregion
	
	//delete modelIDs;
	//delete points;
	//delete texCoords;
}

void RenderManager::Refresh()
{
	int modelSize = tetraRenderables.size();
	if (modelSize > 0)
	{
		if (modelMatrices != nullptr)
			delete modelMatrices;
		modelMatrices = new glm::mat4x4[modelSize];
		if (modelOffsets != nullptr)
			delete modelOffsets;
		modelOffsets = new glm::vec4[modelSize];
		int tetraSize = 0;
		for (int i = 0; i < modelSize; i++)
		{
			tetraSize += tetraRenderables[i].polyCount;
		}
		int lineSize = tetraSize * 6;

		if (modelIDs != nullptr)
			delete modelIDs;
		modelIDs = new int[tetraSize];
		if (tetraIDs != nullptr)
			delete tetraIDs;
		tetraIDs = new int[tetraSize];
		if (points != nullptr)
			delete points;
		points = new glm::vec4[lineSize];
		if (texCoords != nullptr)
			delete texCoords;
		texCoords = new glm::vec4[lineSize];
		if (tetraStates != nullptr)
			delete tetraStates;
		tetraStates = new char[tetraSize];
	}
}

void RenderManager::DynamicGenerate(Rotater& camera, glm::vec4 position)
{
	int tetraSize = dynamicTetras.size();
	if (tetraSize == 0)
		return;
#pragma region Kernel setup and launch
	int lineSize = tetraSize * 6;

	//glm::vec4* points = new glm::vec4[lineSize];
	//glm::vec4* texCoords = new glm::vec4[lineSize];

	cl::Buffer lineBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(Line) * lineSize, dynLineData);
	cl::Buffer pointBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(glm::vec4) * lineSize);
	cl::Buffer stateBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(char) * lineSize);
	cl::Buffer texCoordBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(glm::vec4) * lineSize);
	cl::Buffer matrixBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::mat4), camera.GetTransformValuePtr());
	cl::Buffer offsetBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4), glm::value_ptr(position));

	crossSection.SetVariable(2, 0, lineBuffer);
	crossSection.SetVariable(2, 1, pointBuffer);
	crossSection.SetVariable(2, 2, stateBuffer);
	crossSection.SetVariable(2, 3, texCoordBuffer);
	crossSection.SetVariable(2, 4, matrixBuffer);
	crossSection.SetVariable(2, 5, offsetBuffer);
	crossSection.LaunchKernel(2, 0, lineSize);

#pragma endregion
	crossSection.ReadKernel(2, pointBuffer, CL_TRUE, 0, sizeof(glm::vec4) * lineSize, dynPoints);
	crossSection.ReadKernel(2, texCoordBuffer, CL_TRUE, 0, sizeof(glm::vec4) * lineSize, dynTexCoords);
#pragma region tetrahedron kernel
	if (tetraSize > 0)
	{
		//char* tetraStates = new char[tetraSize];
		cl::Buffer tetraStateBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(char)*tetraSize);
		crossSection.SetVariable(1, 0, pointBuffer);
		crossSection.SetVariable(1, 1, stateBuffer);
		crossSection.SetVariable(1, 2, texCoordBuffer);
		crossSection.SetVariable(1, 3, tetraStateBuffer);
		crossSection.LaunchKernel(1, 0, tetraSize);
		crossSection.ReadKernel(1, pointBuffer, CL_TRUE, 0, sizeof(glm::vec4) * lineSize, dynPoints);
		crossSection.ReadKernel(1, texCoordBuffer, CL_TRUE, 0, sizeof(glm::vec4) * lineSize, dynTexCoords);
		crossSection.ReadKernel(1, tetraStateBuffer, CL_TRUE, 0, sizeof(char)*tetraSize, dynTetraStates);
		for (int i = 0; i < tetraSize; i++)
		{
			if (dynTetraStates[i] >= 3)
			{
				int checkSize = dynTetraStates[i] - 2;
				for (int t = 0; t < checkSize; t++)
				{
					int tID = dynamicTex[i];
					vertexPos[tID].push_back(dynPoints[(i * 6) + 0]);
					vertexCol[tID].push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					vertexTex[tID].push_back(dynTexCoords[(i * 6) + 0]);
					vertexPos[tID].push_back(dynPoints[(i * 6) + 1 + t]);
					vertexCol[tID].push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					vertexTex[tID].push_back(dynTexCoords[(i * 6) + 1 + t]);
					vertexPos[tID].push_back(dynPoints[(i * 6) + 2 + t]);
					vertexCol[tID].push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					vertexTex[tID].push_back(dynTexCoords[(i * 6) + 2 + t]);
				}
			}
		}
		//delete tetraStates;
	}
#pragma endregion

	//delete points;
	//delete texCoords;
}

void RenderManager::DynRefresh()
{
	int tetraSize = dynamicTetras.size();
	if (tetraSize > 0)
	{
		int lineSize = dynamicTetras.size() * 6;
		if (dynLineData != nullptr)
			delete dynLineData;
		dynLineData = new Line[lineSize];
		if (dynPoints != nullptr)
			delete dynPoints;
		dynPoints = new glm::vec4[lineSize];
		if (dynTexCoords != nullptr)
			delete dynTexCoords;
		dynTexCoords = new glm::vec4[lineSize];
		if (dynTetraStates != nullptr)
			delete dynTetraStates;
		dynTetraStates = new char[tetraSize];
		for (int i = 0; i < tetraSize; i++)
		{
			dynLineData[(i * 6) + 0] = dynamicTetras[i].lines[0];
			dynLineData[(i * 6) + 1] = dynamicTetras[i].lines[1];
			dynLineData[(i * 6) + 2] = dynamicTetras[i].lines[2];
			dynLineData[(i * 6) + 3] = dynamicTetras[i].lines[3];
			dynLineData[(i * 6) + 4] = dynamicTetras[i].lines[4];
			dynLineData[(i * 6) + 5] = dynamicTetras[i].lines[5];
		}
	}
}

void RenderManager::HexaModelGenerate(Rotater& camera, glm::vec4 position)
{
	int modelSize = hexaRenderables.size();
	if (modelSize == 0)
		return;
#pragma region Kernel setup and launch
	//glm::mat4x4* modelMatrices = new glm::mat4x4[modelSize];
	//glm::vec4* modelOffsets = new glm::vec4[modelSize];

	int hexaSize = 0;
	for (int i = 0; i < modelSize; i++)
	{
		hexaSize += hexaRenderables[i].polyCount;
	}
	int lineSize = hexaSize * 12;

	//int* modelIDs = new int[hexaSize]; ///Used later as well
	//int* hexaIDs = new int[hexaSize];

	//glm::vec4* points = new glm::vec4[lineSize];
	//glm::vec4* texCoords = new glm::vec4[lineSize];

	int index = 0;
	for (int i = 0; i < modelSize; i++)
	{
		Renderable& renderable = hexaRenderables[i];
		hexaModelMatrices[i] = renderable.transformation;
		hexaModelOffsets[i] = renderable.offset;
		for (int t = 0; t < renderable.polyCount; t++)
		{
			hexaModelIDs[index] = i;
			hexaIDs[index] = hexaModelStarts[renderable.modelID] + t;
			index++;
		}
	}
	
	cl::Buffer modelMatricesBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::mat4x4)*modelSize, hexaModelMatrices);
	cl::Buffer modelOffsetsBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4)*modelSize, hexaModelOffsets);
	cl::Buffer modelIDBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int) * hexaSize, hexaModelIDs);
	cl::Buffer hexaIDBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int) * hexaSize, hexaIDs);
	cl::Buffer pointBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(glm::vec4) * lineSize);
	cl::Buffer stateBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(char) * lineSize);
	cl::Buffer texCoordBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(glm::vec4) * lineSize);
	cl::Buffer matrixBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::mat4), camera.GetTransformValuePtr());
	cl::Buffer offsetBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4), glm::value_ptr(position));

	crossSection.SetVariable(3, 0, modelMatricesBuffer);
	crossSection.SetVariable(3, 1, modelOffsetsBuffer);
	crossSection.SetVariable(3, 2, modelIDBuffer);
	crossSection.SetVariable(3, 3, hexaIDBuffer);
	crossSection.SetVariable(3, 4, pointBuffer);
	crossSection.SetVariable(3, 5, stateBuffer);
	crossSection.SetVariable(3, 6, texCoordBuffer);
	crossSection.SetVariable(3, 7, matrixBuffer);
	crossSection.SetVariable(3, 8, offsetBuffer);
	crossSection.SetVariable(3, 9, hexaModelBuffer);
	crossSection.LaunchKernel(3, 0, hexaSize);

	//delete modelMatrices;
	//delete modelOffsets;
	//delete hexaIDs;
#pragma endregion
	crossSection.ReadKernel(3, pointBuffer, CL_TRUE, 0, sizeof(glm::vec4) * lineSize, hexaPoints);
	crossSection.ReadKernel(3, texCoordBuffer, CL_TRUE, 0, sizeof(glm::vec4) * lineSize, hexaTexCoords);
#pragma region Octochoron kernel
	if (hexaSize > 0)
	{
		//char* hexaStates = new char[hexaSize];
		cl::Buffer hexaStateBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(char)*hexaSize);
		crossSection.SetVariable(4, 0, pointBuffer);
		crossSection.SetVariable(4, 1, stateBuffer);
		crossSection.SetVariable(4, 2, texCoordBuffer);
		crossSection.SetVariable(4, 3, hexaStateBuffer);
		crossSection.LaunchKernel(4, 0, hexaSize);
		crossSection.ReadKernel(4, pointBuffer, CL_TRUE, 0, sizeof(glm::vec4) * lineSize, hexaPoints);
		crossSection.ReadKernel(4, texCoordBuffer, CL_TRUE, 0, sizeof(glm::vec4) * lineSize, hexaTexCoords);
		crossSection.ReadKernel(4, hexaStateBuffer, CL_TRUE, 0, sizeof(char)*hexaSize, hexaStates);
		for (int i = 0; i < hexaSize; i++)
		{
			if (hexaStates[i] >= 3)
			{
				int checkSize = hexaStates[i] - 2;
				int tID = hexaRenderables[hexaModelIDs[i]].textureID;
				if (!hexaRenderables[hexaModelIDs[i]].transparent)
				{
					for (int t = 0; t < checkSize; t++)
					{
						vertexPos[tID].push_back(hexaPoints[(i * 12) + 0]);
						vertexCol[tID].push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
						vertexTex[tID].push_back(hexaTexCoords[(i * 12) + 0]);
						vertexPos[tID].push_back(hexaPoints[(i * 12) + 1 + t]);
						vertexCol[tID].push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
						vertexTex[tID].push_back(hexaTexCoords[(i * 12) + 1 + t]);
						vertexPos[tID].push_back(hexaPoints[(i * 12) + 2 + t]);
						vertexCol[tID].push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
						vertexTex[tID].push_back(hexaTexCoords[(i * 12) + 2 + t]);
					}
				}
				else
				{
					for (int t = 0; t < checkSize; t++)
					{
						transVertexPos[tID].push_back(hexaPoints[(i * 12) + 0]);
						transVertexCol[tID].push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
						transVertexTex[tID].push_back(hexaTexCoords[(i * 12) + 0]);
						transVertexPos[tID].push_back(hexaPoints[(i * 12) + 1 + t]);
						transVertexCol[tID].push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
						transVertexTex[tID].push_back(hexaTexCoords[(i * 12) + 1 + t]);
						transVertexPos[tID].push_back(hexaPoints[(i * 12) + 2 + t]);
						transVertexCol[tID].push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
						transVertexTex[tID].push_back(hexaTexCoords[(i * 12) + 2 + t]);
					}
				}
			}
		}
		//delete hexaStates;
	}
#pragma endregion

	//delete modelIDs;
	//delete points;
	//delete texCoords;
}
void RenderManager::HexaRefresh()
{
	int modelSize = hexaRenderables.size();
	if (modelSize > 0)
	{
		if (hexaModelMatrices != nullptr)
			delete hexaModelMatrices;
		hexaModelMatrices = new glm::mat4x4[modelSize];
		if (hexaModelOffsets != nullptr)
			delete hexaModelOffsets;
		hexaModelOffsets = new glm::vec4[modelSize];
		int hexaSize = 0;
		for (int i = 0; i < modelSize; i++)
		{
			hexaSize += hexaRenderables[i].polyCount;
		}
		int lineSize = hexaSize * 12;

		if (hexaModelIDs != nullptr)
			delete hexaModelIDs;
		hexaModelIDs = new int[hexaSize];
		if (hexaIDs != nullptr)
			delete hexaIDs;
		hexaIDs = new int[hexaSize];
		if (hexaPoints != nullptr)
			delete hexaPoints;
		hexaPoints = new glm::vec4[lineSize];
		if (hexaTexCoords != nullptr)
			delete hexaTexCoords;
		hexaTexCoords = new glm::vec4[lineSize];
		if (hexaStates != nullptr)
			delete hexaStates;
		hexaStates = new char[hexaSize];
	}
}
void RenderManager::CopyToBuffer(unsigned int* VBOaddress, unsigned int* bufferSizes)
{
	for (int i = 0; i < textureCount; i++)
	{
		bufferSizes[i] = 0;
	}
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
	delete vboData;
}
void RenderManager::TransCopyToBuffer(std::vector<unsigned int>& textureIDs, std::vector<unsigned int>& textureRunLengths, unsigned int* VBOAddress)
{
	int triangleCount = 0;
	for (int i = 0; i < transTextureCount; i++)
	{
		triangleCount += transVertexPos[i].size() / 3;
	}
	if (triangleCount == 0)
	{
		textureRunLengths.resize(1);
		textureRunLengths[0] = 0;
		return;
	}
	SmartArray<SortItem<std::pair<int, int>>> triangles(triangleCount);
	int index = 0;
	for (int i = 0; i < transTextureCount; i++)
	{
		for (int v = 0; v < transVertexPos[i].size() / 3; v++)
		{
			if (transVertexPos[i][(v * 3)].z > 0 || transVertexPos[i][1 + (v * 3)].z > 0 || transVertexPos[i][2 + (v * 3)].z > 0 || true)
			{
				glm::vec3 averagePos = (transVertexPos[i][(v * 3)] + transVertexPos[i][1 + (v * 3)] + transVertexPos[i][2 + (v * 3)]) / 3.0f;
				triangles[index].value = glm::pow(averagePos.x,2)+glm::pow(averagePos.y, 2)+glm::pow(averagePos.z, 2);
				triangles[index].data.first = i;
				triangles[index].data.second = v;
				index++;
			}
		}
	}
	triangles.count = index;
	InsertSort(triangles, true);
	float* vboData = new float[triangleCount * 27];
	textureIDs.reserve(triangleCount);
	textureRunLengths.reserve(triangleCount);
	textureIDs.resize(triangleCount);
	textureRunLengths.resize(triangleCount);
	unsigned int currentTexture = triangles[0].data.first;
	textureIDs[0] = currentTexture;
	index = 0;
	unsigned int currentTexRun = 1;
	for (int i = 0; i < triangles.count; i++)
	{
		std::pair<int,int>& triangleData = triangles[i].data;
		if (triangleData.first != currentTexture)
		{
			currentTexture = triangleData.first;
			textureRunLengths[index] = currentTexRun;
			currentTexRun = 0;
			index++;
			textureIDs[index] = currentTexture;
		}
		currentTexRun++;
		vboData[0 + (i * 27)] = transVertexPos[triangleData.first][(triangleData.second * 3)].x;
		vboData[1 + (i * 27)] = transVertexPos[triangleData.first][(triangleData.second * 3)].y;
		vboData[2 + (i * 27)] = transVertexPos[triangleData.first][(triangleData.second * 3)].z;
		vboData[3 + (i * 27)] = transVertexCol[triangleData.first][(triangleData.second * 3)].x;
		vboData[4 + (i * 27)] = transVertexCol[triangleData.first][(triangleData.second * 3)].y;
		vboData[5 + (i * 27)] = transVertexCol[triangleData.first][(triangleData.second * 3)].z;
		vboData[6 + (i * 27)] = transVertexTex[triangleData.first][(triangleData.second * 3)].x;
		vboData[7 + (i * 27)] = transVertexTex[triangleData.first][(triangleData.second * 3)].y;
		vboData[8 + (i * 27)] = transVertexTex[triangleData.first][(triangleData.second * 3)].z;
		vboData[9 + (i * 27)] = transVertexPos[triangleData.first][(triangleData.second * 3)+1].x;
		vboData[10 + (i * 27)] = transVertexPos[triangleData.first][(triangleData.second * 3)+1].y;
		vboData[11 + (i * 27)] = transVertexPos[triangleData.first][(triangleData.second * 3)+1].z;
		vboData[12 + (i * 27)] = transVertexCol[triangleData.first][(triangleData.second * 3)+1].x;
		vboData[13 + (i * 27)] = transVertexCol[triangleData.first][(triangleData.second * 3)+1].y;
		vboData[14 + (i * 27)] = transVertexCol[triangleData.first][(triangleData.second * 3)+1].z;
		vboData[15 + (i * 27)] = transVertexTex[triangleData.first][(triangleData.second * 3)+1].x;
		vboData[16 + (i * 27)] = transVertexTex[triangleData.first][(triangleData.second * 3)+1].y;
		vboData[17 + (i * 27)] = transVertexTex[triangleData.first][(triangleData.second * 3)+1].z;
		vboData[18 + (i * 27)] = transVertexPos[triangleData.first][(triangleData.second * 3)+2].x;
		vboData[19 + (i * 27)] = transVertexPos[triangleData.first][(triangleData.second * 3)+2].y;
		vboData[20 + (i * 27)] = transVertexPos[triangleData.first][(triangleData.second * 3)+2].z;
		vboData[21 + (i * 27)] = transVertexCol[triangleData.first][(triangleData.second * 3)+2].x;
		vboData[22 + (i * 27)] = transVertexCol[triangleData.first][(triangleData.second * 3)+2].y;
		vboData[23 + (i * 27)] = transVertexCol[triangleData.first][(triangleData.second * 3)+2].z;
		vboData[24 + (i * 27)] = transVertexTex[triangleData.first][(triangleData.second * 3)+2].x;
		vboData[25 + (i * 27)] = transVertexTex[triangleData.first][(triangleData.second * 3)+2].y;
		vboData[26 + (i * 27)] = transVertexTex[triangleData.first][(triangleData.second * 3)+2].z;
	}
	textureIDs.resize(index+1);
	textureRunLengths.resize(index+1);
	textureRunLengths[index] = currentTexRun-1;
	glBindBuffer(GL_ARRAY_BUFFER, *VBOAddress);
	glBufferData(GL_ARRAY_BUFFER, 27 * sizeof(float) * triangleCount, vboData, GL_DYNAMIC_DRAW);
	delete vboData;
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