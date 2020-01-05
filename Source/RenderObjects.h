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
	int tetraCount;

	Renderable();
	Renderable(glm::mat4x4, glm::vec4, int, int, int);
};

class RenderManager
{
	GPUProgram crossSection;
public:
	std::vector<Tetrahedron> models;
	std::vector<unsigned int> modelStarts;
	cl::Buffer modelBuffer;
	std::deque<Renderable> renderables;

	RenderManager();

	void SetBuffer(Camera&, unsigned int*, int, unsigned int*);

};