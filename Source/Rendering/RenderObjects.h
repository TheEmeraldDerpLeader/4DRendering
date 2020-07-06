#pragma once
#include <GL/glew.h>

#include "KernelHelper.h"
#include "RenderingGeometry.h"
		 
#include <HelpfulScripts/Helpers.h>
#include <HelpfulScripts/Rotater.h>
#include <HelpfulScripts/Modulo.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <deque>
#include <utility>

struct Texture
{
public:
	unsigned int id;
	bool usesAlpha;
	Texture();
	Texture(int idInt, bool usesAlphaBool) : id(idInt), usesAlpha(usesAlphaBool) {}

	void BindTexture(int);
};

struct Renderable
{
public:
	glm::mat4x4 transformation;
	glm::mat4x4 normalTransformation;
	glm::vec4 offset;
	int modelID;
	int textureID;
	int polyCount;
	bool transparent; 

	Renderable();
	Renderable(glm::mat4x4, glm::vec4, int, int, int, bool);

	void SetTransformation(glm::mat4x4);
};

class RenderManager
{
	GPUProgram crossSection;
public:
	std::vector<Tetrahedron> tetraModels;
	std::vector<unsigned int> tetraModelStarts;
	std::vector<glm::vec4> tetraNormals;
	std::vector<Hexahedron> hexaModels;
	std::vector<unsigned int> hexaModelStarts;
	std::vector<glm::vec4> hexaNormals;
	cl::Buffer tetraModelBuffer;
	cl::Buffer modelNormalBuffer;
	cl::Buffer hexaModelBuffer;
	cl::Buffer hexaModelNormalBuffer;
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
	std::deque<glm::vec4>* normalData;
	std::deque<glm::vec4>* transNormalData;
	int* vertexIndex;
	int* transVertexIndex;

	glm::mat4x4* modelMatrices;
	glm::vec4* modelOffsets;
	int* modelIDs;
	int* tetraIDs;
	glm::vec4* points;
	glm::vec4* texCoords;
	glm::vec4* outNormals;
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
	glm::vec4* hexaOutNormals;
	char* hexaStates;

	RenderManager();

	void SetupModelBuffers();

	void ResetIndexes();
	void InitializeDeques(unsigned int, unsigned int);
	void ModelGenerate(Rotater&, glm::vec4);
	void Refresh();
	void DynamicGenerate(Rotater&, glm::vec4);
	void DynRefresh();
	void HexaModelGenerate(Rotater&, glm::vec4);
	void HexaRefresh();
	void CopyToBuffer(unsigned int*, unsigned int*);
	void TransCopyToBuffer(std::vector<unsigned int>&, std::vector<unsigned int>&, unsigned int*);
};