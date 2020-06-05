#pragma once
#include <GL/glew.h>

#include "KernelHelper.h"
#include "Helpers.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Modulo.h"
#include "RenderingGeometry.h"

#include <vector>
#include <deque>
#include <utility>

class Texture
{
public:
	unsigned int id;
	bool usesAlpha;
	Texture();
	Texture(int idInt, bool usesAlphaBool) : id(idInt), usesAlpha(usesAlphaBool) {}

	void BindTexture(int);
};

class Rotater
{
public:
	glm::mat4x4 copyMatrix;
public:
	float rotation[6];

	Rotater();
	Rotater(float, float, float, float, float, float);

	glm::mat4x4 GetTransform();
	float* GetTransformValuePtr();
	void RotateXY(float);
	void RotateYZ(float);
	void RotateZX(float);
	void RotateXW(float);
	void RotateYW(float);
	void RotateZW(float);
};

class Renderable
{
public:
	glm::mat4x4 transformation;
	glm::vec4 offset;
	int modelID;
	int textureID;
	int polyCount;
	bool transparent; 

	Renderable();
	Renderable(glm::mat4x4, glm::vec4, int, int, int, bool);
};

class RenderManager
{
	GPUProgram crossSection;
public:
	std::vector<Tetrahedron> tetraModels;
	std::vector<unsigned int> tetraModelStarts;
	std::vector<Hexahedron> hexaModels;
	std::vector<unsigned int> hexaModelStarts;
	cl::Buffer tetraModelBuffer;
	cl::Buffer hexaModelBuffer;
	std::deque<Renderable> tetraRenderables; //modelGen
	std::deque<Tetrahedron> dynamicTetras; //dynamicGen
	std::deque<unsigned int> dynamicTex;
	std::deque<Renderable> hexaRenderables; //hexaModelGen
	unsigned int textureCount;
	unsigned int transTextureCount;
	std::deque<glm::vec4>* vertexPos;
	std::deque<glm::vec4>* transVertexPos;
	std::deque<glm::vec3>* vertexCol;
	std::deque<glm::vec3>* transVertexCol;
	std::deque<glm::vec3>* vertexTex;
	std::deque<glm::vec3>* transVertexTex;

	glm::mat4x4* modelMatrices;
	glm::vec4* modelOffsets;
	int* modelIDs;
	int* tetraIDs;
	glm::vec4* points;
	glm::vec4* texCoords;
	char* tetraStates;

	Line* dynLineData;
	glm::vec4* dynPoints;
	glm::vec4* dynTexCoords;
	char* dynTetraStates;

	glm::mat4x4* hexaModelMatrices;
	glm::vec4* hexaModelOffsets;
	int* hexaModelIDs;
	int* hexaIDs;
	glm::vec4* hexaPoints;
	glm::vec4* hexaTexCoords;
	char* hexaStates;

	RenderManager();

	void ClearDeques(unsigned int, unsigned int);
	void ModelGenerate(Rotater&, glm::vec4);
	void Refresh();
	void DynamicGenerate(Rotater&, glm::vec4);
	void DynRefresh();
	void HexaModelGenerate(Rotater&, glm::vec4);
	void HexaRefresh();
	void CopyToBuffer(unsigned int*, unsigned int*);
	void TransCopyToBuffer(std::vector<unsigned int>&, std::vector<unsigned int>&, unsigned int*);
};