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

void InsertSort(SmartArray<SortItem>& items)
{
	int itemSize = items.count;

	SortItem* data = items.data;
	for (int i = 1; i < itemSize; i++)
	{
		SortItem item = data[i];
		for (int b = 1; b <= i; b++)
		{
			SortItem* checkItem = &(data[i - b]);
			if (item.value < checkItem->value)
			{
				SortItem swapper = *checkItem;
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

/*
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
		delete data;
}
*/