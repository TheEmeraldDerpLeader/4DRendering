#pragma once
#include "Helpers.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Modulo.h"

#include <vector>

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
