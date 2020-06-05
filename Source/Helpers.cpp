#include "Helpers.h"
#include <chrono>

glm::mat4x4 RotateMat(float xy, float yz, float zx, float xw, float yw, float zw)
{
	glm::mat4x4 copyMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	glm::mat4x4 tempMat;
	float angle;
	//Inverted and column major

	//X->Y rotation
	angle = glm::radians(xy);
	tempMat = glm::mat4x4(glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	copyMatrix *= tempMat;

	//Y->Z rotation
	angle = glm::radians(yz);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle), 0, 0, 0, 0, 1);
	copyMatrix *= tempMat;

	//Z->X rotation
	angle = glm::radians(zx);
	tempMat = glm::mat4x4(glm::cos(angle), 0, glm::sin(angle), 0, 0, 1, 0, 0, -glm::sin(angle), 0, glm::cos(angle), 0, 0, 0, 0, 1);
	copyMatrix *= tempMat;

	//X->W rotation
	angle = glm::radians(xw);
	tempMat = glm::mat4x4(glm::cos(angle), 0, 0, -glm::sin(angle), 0, 1, 0, 0, 0, 0, 1, 0, glm::sin(angle), 0, 0, glm::cos(angle));
	copyMatrix *= tempMat;

	//Y->W rotation
	angle = glm::radians(yw);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, glm::cos(angle), 0, -glm::sin(angle), 0, 0, 1, 0, 0, glm::sin(angle), 0, glm::cos(angle));
	copyMatrix *= tempMat;

	//Z->W rotation
	angle = glm::radians(zw);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle));
	copyMatrix *= tempMat;

	return copyMatrix;
}

glm::vec4 CrossProd4(glm::vec4 defVec1, glm::vec4 defVec2, glm::vec4 defVec3, glm::vec4 refPoint)
{
	glm::vec4 crossVec;
	glm::mat3x3 holdMat;
	holdMat[0][0] = defVec1.y;
	holdMat[1][0] = defVec1.z;
	holdMat[2][0] = defVec1.w;
	holdMat[0][1] = defVec2.y;
	holdMat[1][1] = defVec2.z;
	holdMat[2][1] = defVec2.w;
	holdMat[0][2] = defVec3.y;
	holdMat[1][2] = defVec3.z;
	holdMat[2][2] = defVec3.w;
	crossVec.x = glm::determinant(holdMat);
	holdMat[0][0] = defVec1.x;
	holdMat[1][0] = defVec1.z;
	holdMat[2][0] = defVec1.w;
	holdMat[0][1] = defVec2.x;
	holdMat[1][1] = defVec2.z;
	holdMat[2][1] = defVec2.w;
	holdMat[0][2] = defVec3.x;
	holdMat[1][2] = defVec3.z;
	holdMat[2][2] = defVec3.w;
	crossVec.y = glm::determinant(holdMat);
	holdMat[0][0] = defVec1.x;
	holdMat[1][0] = defVec1.y;
	holdMat[2][0] = defVec1.w;
	holdMat[0][1] = defVec2.x;
	holdMat[1][1] = defVec2.y;
	holdMat[2][1] = defVec2.w;
	holdMat[0][2] = defVec3.x;
	holdMat[1][2] = defVec3.y;
	holdMat[2][2] = defVec3.w;
	crossVec.z = glm::determinant(holdMat);
	holdMat[0][0] = defVec1.x;
	holdMat[1][0] = defVec1.y;
	holdMat[2][0] = defVec1.z;
	holdMat[0][1] = defVec2.x;
	holdMat[1][1] = defVec2.y;
	holdMat[2][1] = defVec2.z;
	holdMat[0][2] = defVec3.x;
	holdMat[1][2] = defVec3.y;
	holdMat[2][2] = defVec3.z;
	crossVec.w = glm::determinant(holdMat);
	if (glm::dot(crossVec, refPoint) < 0)
	{
		crossVec *= -1;
	}
	return crossVec;
}


int RandomGenerator::RandomInt(int firstInt, int secondInt)
{
	if (firstInt == secondInt)
	{
		return firstInt;
	}
	else if (firstInt < secondInt)
	{
		secondInt -= firstInt;
		return (int)(((float)rand() / (float)RAND_MAX) * (float)secondInt) + firstInt;
	}
	else
	{
		firstInt -= secondInt;
		return (int)(((float)rand() / (float)RAND_MAX) * (float)firstInt) + secondInt;
	}
}

float RandomGenerator::RandomFloat(float firstFloat, float secondFloat)
{
	if (firstFloat == secondFloat)
	{
		return firstFloat;
	}
	else if (firstFloat < secondFloat)
	{
		secondFloat -= firstFloat;
		return (((float)rand() / (float)RAND_MAX) * secondFloat) + firstFloat;
	}
	else
	{
		firstFloat -= secondFloat;
		return (((float)rand() / (float)RAND_MAX) * firstFloat) + secondFloat;
	}
}
/*
template <class T>
void InsertSort(SmartArray<SortItem<T>>& items)
{
	int itemSize = items.count;

	SortItem<T>* data = items.data;
	for (int i = 1; i < itemSize; i++)
	{
		SortItem<T> item = data[i];
		for (int b = 1; b <= i; b++)
		{
			SortItem<T>* checkItem = &(data[i - b]);
			if (item.value < checkItem->value)
			{
				SortItem<T> swapper = *checkItem;
				*checkItem = data[i - (b-1)];
				data[i - (b-1)] = swapper;
			}
			else
			{
				break;
			}
		}
	}
}

template<class T>
SmartArray<T>::SmartArray()
{
	data = nullptr;
	count = 0;
}

template<class T>
SmartArray<T>::SmartArray(unsigned int size)
{
	data = new T[size];
	count = size;
}

template<class T>
SmartArray<T>::~SmartArray()
{
	Release();
}

template<class T>
void SmartArray<T>::Initialize(unsigned int size)
{
	if (data != nullptr)
		Release();
	data = new T[size];
	count = size;
}

template<class T>
void SmartArray<T>::Release()
{
	if (data != nullptr)
	{
		delete[] data;
		data = nullptr;
	}
}

template<class T>
T& SmartArray<T>::operator[](int index)
{
	if (index < 0 || index >= count)
		abort();
	return data[index];
}
*/