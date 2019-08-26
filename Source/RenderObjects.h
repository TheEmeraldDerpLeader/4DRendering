#pragma once
#include <GL/glew.h>

#include "KernelHelper.h"
#include "Helpers.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Modulo.h"

#include <vector>
#include <deque>

glm::mat4x4 RotateMat(float, float, float, float, float, float);

struct Line
{
	glm::vec4 point;
	glm::vec4 direction;

	Line();
	Line(glm::vec4, glm::vec4);
	Line(float, float, float, float, float, float, float, float);

	void Translate(float, float, float, float);
	void Rotate(float, float, float, float, float, float);
	void Transform(glm::mat4x4);
	void TransformAround(glm::mat4x4, glm::vec4);
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

struct Triangle
{
	Line lines[3];

	Triangle();
	Triangle(glm::vec4, glm::vec4, glm::vec4);
	Triangle(float, float, float, float, float, float, float, float, float, float, float, float);
};

struct Tetrahedron
{
	Line lines[6];

	Tetrahedron();
	Tetrahedron(glm::vec4, glm::vec4, glm::vec4, glm::vec4);
	Tetrahedron(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);

	void Translate(float, float, float, float);
};

struct Cube
{
	Line lines[12];
	glm::vec4 center;

	Cube();
	Cube(glm::vec4, float);
	Cube(float, float, float, float, float);

	void Translate(float, float, float, float);
	void Rotate(float, float, float, float, float, float);
	void Transform(glm::mat4x4);
	void TransformAround(glm::mat4x4, glm::vec4);
};

struct Pentachoron
{
	Tetrahedron tetrahedra[5];

	Pentachoron();
	Pentachoron(glm::vec4, glm::vec4, glm::vec4, glm::vec4, glm::vec4);
	Pentachoron(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);

	void Translate(float, float, float, float);
};

struct Tesseract
{
	Cube cubes[8];
	glm::vec4 center;

	Tesseract();
	Tesseract(glm::vec4, float);
	Tesseract(float, float, float, float, float);

	void Translate(float, float, float, float);
	void Rotate(float, float, float, float, float, float);
	void Transform(glm::mat4x4);
};

class RenderManager
{
	GPUProgram crossSection;
	std::deque<glm::vec4> vertexPos;
	std::deque<glm::vec3> vertexCol;
public:
	int faceColor;

	std::vector<Tetrahedron> tetrahedra;
	std::vector<Cube> cubes;

	RenderManager();
	RenderManager(Tetrahedron*, int);

	unsigned int SetBuffer(Camera&, unsigned int);

};