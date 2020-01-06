#pragma once
#include "Helpers.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Modulo.h"

#include <vector>
#include <iostream>

struct Line
{
	glm::vec4 point;
	glm::vec4 direction;
	glm::vec4 textureCoords;
	glm::vec4 textureDirection;

	Line();
	Line(glm::vec4, glm::vec3);
	Line(glm::vec4, glm::vec4, glm::vec3, glm::vec3);
	Line(float, float, float, float, float, float, float);
	Line(float, float, float, float, float, float, float, float, float, float, float, float, float, float);
	Line(Line, Line);

	void Translate(float, float, float, float);
	void Rotate(float, float, float, float, float, float);
	void Transform(glm::mat4x4);
	void TransformAround(glm::mat4x4, glm::vec4);
	void Invert();

	Line operator+(glm::vec4);
	Line operator-(glm::vec4);
	bool operator==(Line&);
};

struct Tetrahedron
{
	Line lines[6];

	Tetrahedron();
	Tetrahedron(Line, Line, Line, Line, Line, Line);
	Tetrahedron(glm::vec4, glm::vec3, glm::vec4, glm::vec3, glm::vec4, glm::vec3, glm::vec4, glm::vec3);
	Tetrahedron(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);

	void Translate(float, float, float, float);

	bool operator==(Tetrahedron&);
};

void PrismifyByVec(Line, Line, Line, glm::vec4, glm::vec3, std::vector<Tetrahedron>&);
void PrismifyByTri();

/*
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
	Pentachoron(glm::vec4, glm::vec3, glm::vec4, glm::vec3, glm::vec4, glm::vec3, glm::vec4, glm::vec3, glm::vec4, glm::vec3);
	Pentachoron(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);

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
*/
