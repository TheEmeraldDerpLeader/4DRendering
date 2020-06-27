#include "Rotater.h"

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
	glm::mat4x4 holdMatrix = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	glm::mat4x4 tempMat;
	float angle;
	//column major

	//Z->W rotation
	angle = -glm::radians(rotation[5]);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle));
	holdMatrix *= tempMat;

	//Y->W rotation
	angle = -glm::radians(rotation[4]);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, glm::cos(angle), 0, -glm::sin(angle), 0, 0, 1, 0, 0, glm::sin(angle), 0, glm::cos(angle));
	holdMatrix *= tempMat;

	//X->W rotation
	angle = -glm::radians(rotation[3]);
	tempMat = glm::mat4x4(glm::cos(angle), 0, 0, -glm::sin(angle), 0, 1, 0, 0, 0, 0, 1, 0, glm::sin(angle), 0, 0, glm::cos(angle));
	holdMatrix *= tempMat;

	//Z->X rotation
	angle = -glm::radians(rotation[2]);
	tempMat = glm::mat4x4(glm::cos(angle), 0, glm::sin(angle), 0, 0, 1, 0, 0, -glm::sin(angle), 0, glm::cos(angle), 0, 0, 0, 0, 1);
	holdMatrix *= tempMat;

	//Y->Z rotation
	angle = -glm::radians(rotation[1]);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle), 0, 0, 0, 0, 1);
	holdMatrix *= tempMat;

	//X->Y rotation
	angle = -glm::radians(rotation[0]);
	tempMat = glm::mat4x4(glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	holdMatrix *= tempMat;

	return holdMatrix;
}

glm::mat4x4 Rotater::GetNormalTransform()
{
	glm::mat4x4 holdMatrix = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	glm::mat4x4 tempMat;
	float angle;
	//column major

	//Z->W rotation
	angle = -glm::radians(rotation[5]);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle));
	holdMatrix *= tempMat;

	//Y->W rotation
	angle = -glm::radians(rotation[4]);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, glm::cos(angle), 0, -glm::sin(angle), 0, 0, 1, 0, 0, glm::sin(angle), 0, glm::cos(angle));
	holdMatrix *= tempMat;

	//X->W rotation
	angle = -glm::radians(rotation[3]);
	tempMat = glm::mat4x4(glm::cos(angle), 0, 0, -glm::sin(angle), 0, 1, 0, 0, 0, 0, 1, 0, glm::sin(angle), 0, 0, glm::cos(angle));
	holdMatrix *= tempMat;

	//Z->X rotation
	angle = -glm::radians(rotation[2]);
	tempMat = glm::mat4x4(glm::cos(angle), 0, glm::sin(angle), 0, 0, 1, 0, 0, -glm::sin(angle), 0, glm::cos(angle), 0, 0, 0, 0, 1);
	holdMatrix *= tempMat;

	//Y->Z rotation
	angle = -glm::radians(rotation[1]);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle), 0, 0, 0, 0, 1);
	holdMatrix *= tempMat;

	//X->Y rotation
	angle = -glm::radians(rotation[0]);
	tempMat = glm::mat4x4(glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	holdMatrix *= tempMat;

	holdMatrix = glm::transpose(glm::inverse(holdMatrix));
	return holdMatrix;
}

glm::mat4x4 Rotater::GetInvertedTransform()
{
	glm::mat4x4 holdMatrix = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	glm::mat4x4 tempMat;
	float angle;
	//Inverted and column major

	//X->Y rotation
	angle = glm::radians(rotation[0]);
	tempMat = glm::mat4x4(glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	holdMatrix *= tempMat;

	//Y->Z rotation
	angle = glm::radians(rotation[1]);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle), 0, 0, 0, 0, 1);
	holdMatrix *= tempMat;

	//Z->X rotation
	angle = glm::radians(rotation[2]);
	tempMat = glm::mat4x4(glm::cos(angle), 0, glm::sin(angle), 0, 0, 1, 0, 0, -glm::sin(angle), 0, glm::cos(angle), 0, 0, 0, 0, 1);
	holdMatrix *= tempMat;

	//X->W rotation
	angle = glm::radians(rotation[3]);
	tempMat = glm::mat4x4(glm::cos(angle), 0, 0, -glm::sin(angle), 0, 1, 0, 0, 0, 0, 1, 0, glm::sin(angle), 0, 0, glm::cos(angle));
	holdMatrix *= tempMat;

	//Y->W rotation
	angle = glm::radians(rotation[4]);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, glm::cos(angle), 0, -glm::sin(angle), 0, 0, 1, 0, 0, glm::sin(angle), 0, glm::cos(angle));
	holdMatrix *= tempMat;

	//Z->W rotation
	angle = glm::radians(rotation[5]);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle));
	holdMatrix *= tempMat;

	return holdMatrix;
}

glm::mat4x4 Rotater::GetInvertedNormalTransform()
{
	glm::mat4x4 holdMatrix = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	glm::mat4x4 tempMat;
	float angle;
	//Inverted and column major

	//X->Y rotation
	angle = glm::radians(rotation[0]);
	tempMat = glm::mat4x4(glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	holdMatrix *= tempMat;

	//Y->Z rotation
	angle = glm::radians(rotation[1]);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle), 0, 0, 0, 0, 1);
	holdMatrix *= tempMat;

	//Z->X rotation
	angle = glm::radians(rotation[2]);
	tempMat = glm::mat4x4(glm::cos(angle), 0, glm::sin(angle), 0, 0, 1, 0, 0, -glm::sin(angle), 0, glm::cos(angle), 0, 0, 0, 0, 1);
	holdMatrix *= tempMat;

	//X->W rotation
	angle = glm::radians(rotation[3]);
	tempMat = glm::mat4x4(glm::cos(angle), 0, 0, -glm::sin(angle), 0, 1, 0, 0, 0, 0, 1, 0, glm::sin(angle), 0, 0, glm::cos(angle));
	holdMatrix *= tempMat;

	//Y->W rotation
	angle = glm::radians(rotation[4]);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, glm::cos(angle), 0, -glm::sin(angle), 0, 0, 1, 0, 0, glm::sin(angle), 0, glm::cos(angle));
	holdMatrix *= tempMat;

	//Z->W rotation
	angle = glm::radians(rotation[5]);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle));
	holdMatrix *= tempMat;

	holdMatrix = glm::transpose(glm::inverse(holdMatrix));
	return holdMatrix;
}
void Rotater::InvertedCopyToMatrixBuffer()
{
	glm::mat4x4 transformMatrix = GetInvertedTransform();
	glm::mat4x4 normalMatrix = GetInvertedNormalTransform();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matrixBuffer[(i * 4) + j] = transformMatrix[i][j];
			matrixBuffer[(i * 4) + j + 16] = normalMatrix[i][j];
		}
	}
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