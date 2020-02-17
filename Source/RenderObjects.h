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

class Texture
{
public:
	unsigned int id;
	bool usesAlpha;
	Texture();
	Texture(int idInt, bool usesAlphaBool) : id(idInt), usesAlpha(usesAlphaBool) {}

	void BindTexture(int);
};

class Camera
{
public:
	glm::mat4x4 copyMatrix;
public:
	glm::vec4 position;
	float rotation[6];

	Camera();
	Camera(glm::vec4, float, float, float, float, float, float);

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

	Renderable();
	Renderable(glm::mat4x4, glm::vec4, int, int, int);
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
	std::deque<glm::vec4>* vertexPos;
	std::deque<glm::vec3>* vertexCol;
	std::deque<glm::vec3>* vertexTex;

	RenderManager();

	void ClearDeques(unsigned int);
	void ModelGenerate(Camera&);
	void DynamicGenerate(Camera&);
	void HexaModelGenerate(Camera&);
	void CopyToBuffer(unsigned int*, unsigned int*);
};