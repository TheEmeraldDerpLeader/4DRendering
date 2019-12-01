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

	Renderable();
	Renderable(glm::mat4x4, glm::vec4, int);
};

class RenderManager
{
	GPUProgram crossSection;
	std::deque<glm::vec4> vertexPos;
	std::deque<glm::vec3> vertexCol;
	std::deque<glm::vec3> vertexTex;
public:
	bool renderMode;
	Tetrahedron pentachoronModel[5];
	cl::Buffer modelBuffer;
	std::vector<Renderable> pentaRenderables;

	RenderManager();

	unsigned int SetBuffer(Camera&, unsigned int);

};