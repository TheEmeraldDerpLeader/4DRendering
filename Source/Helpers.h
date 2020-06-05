#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Modulo.h"

#include <vector>

const double PI = 3.14159265359;

glm::mat4x4 RotateMat(float, float, float, float, float, float);
glm::vec4 CrossProd4(glm::vec4, glm::vec4, glm::vec4, glm::vec4);

template <class T>
struct SortItem
{
	T data;
	float value;

	SortItem() : data(T()), value(0){}
	SortItem(T dataTemp, float valueFloat) : data(dataTemp), value(valueFloat){}
};
class RandomGenerator
{
public:
	static int RandomInt(int, int);
	static float RandomFloat(float, float);
};

template <class T>
class SmartArray
{
public:
	T* data;
	unsigned int count;

	SmartArray()
	{
		data = nullptr;
		count = 0;
	}
	SmartArray(unsigned int size)
	{
		data = new T[size];
		count = size;
	}
	~SmartArray()
	{
		Release();
	}

	void Initialize(unsigned int size)
	{
		if (data != nullptr)
			Release();
		data = new T[size];
		count = size;
	}
	void Release()
	{
		if (data != nullptr)
		{
			delete[] data;
			data = nullptr;
		}
	}

	T& operator[](int index){
		if (index < 0 || index >= count)
			abort();
		return data[index];
	}
};

template<class T>
//void InsertSort(SmartArray<SortItem<T>>&);
void InsertSort(SmartArray<SortItem<T>>& items, bool reverse)
{
	int itemSize = items.count;

	SortItem<T>* data = items.data;
	if (!reverse)
	{
		for (int i = 1; i < itemSize; i++)
		{
			SortItem<T> item = data[i];
			for (int b = 1; b <= i; b++)
			{
				SortItem<T>* checkItem = &(data[i - b]);
				if (item.value < checkItem->value)
				{
					SortItem<T> swapper = *checkItem;
					*checkItem = data[i - (b - 1)];
					data[i - (b - 1)] = swapper;
				}
				else
				{
					break;
				}
			}
		}
	}
	else
	{
		for (int i = 1; i < itemSize; i++)
		{
			SortItem<T> item = data[i];
			for (int b = 1; b <= i; b++)
			{
				SortItem<T>* checkItem = &(data[i - b]);
				if (item.value > checkItem->value)
				{
					SortItem<T> swapper = *checkItem;
					*checkItem = data[i - (b - 1)];
					data[i - (b - 1)] = swapper;
				}
				else
				{
					break;
				}
			}
		}
	}
}