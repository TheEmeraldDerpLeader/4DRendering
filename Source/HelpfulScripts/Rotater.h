#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Rotater
{
public:
	float rotation[6];
	float matrixBuffer[32];

	Rotater();
	Rotater(float, float, float, float, float, float);

	glm::mat4x4 GetTransform();
	glm::mat4x4 GetNormalTransform();
	glm::mat4x4 GetInvertedTransform();
	glm::mat4x4 GetInvertedNormalTransform();
	void CopyToMatrixBuffer(); //not implemented
	void InvertedCopyToMatrixBuffer();
	void RotateXY(float);
	void RotateYZ(float);
	void RotateZX(float);
	void RotateXW(float);
	void RotateYW(float);
	void RotateZW(float);
};