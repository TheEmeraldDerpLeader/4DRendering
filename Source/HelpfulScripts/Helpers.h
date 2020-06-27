#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Modulo.h"

#include <vector>

const double PI = 3.14159265359;

glm::mat4x4 RotateMat(float, float, float, float, float, float);
glm::vec4 CrossProd4(glm::vec4, glm::vec4, glm::vec4, glm::vec4);
glm::vec4 CrossProd4(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);

template <class T>
struct SortItem
{
	T data;
	float value;

	SortItem() : data(T()), value(0){}
	SortItem(T dataTemp, float valueFloat) : data(dataTemp), value(valueFloat){}
	void operator=(SortItem& copyItem) { data = copyItem.data; value = copyItem.value; }
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
	void Copy(SmartArray<T>& arrayToCopy)
	{
		Initialize(arrayToCopy.count);
		for (int i = 0; i < count; i++)
		{
			data[i] = arrayToCopy.data[i];
		}
	}

	T& operator[](int index)
	{
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

template<class T>
void HeapSort(SmartArray<SortItem<T>>& items, bool reverse)
{
	int count = items.count;
	for (int i = count / 2 - 1; i >= 0; i--)
		Heapify(items, count, i, reverse);

	for (int i = count - 1; i > 0; i--)
	{
		SortItem<T> hold = items[0];
		items[0] = items[i];
		items[i] = hold;

		Heapify(items, i, 0, reverse);
	}
}

template<class T>
void Heapify(SmartArray<SortItem<T>>& items, int count, int index, bool reverse)
{
	int largest = index;
	int l = 2 * index + 1;
	int r = 2 * index + 2;

	if (!reverse)
	{
		if (l < count && items[l].value > items[largest].value)
			largest = l;
		if (r < count && items[r].value > items[largest].value)
			largest = r;
	}
	else
	{
		if (l < count && items[l].value < items[largest].value)
			largest = l;
		if (r < count && items[r].value < items[largest].value)
			largest = r;
	}

	if (largest != index)
	{
		SortItem<T> hold = items[index];
		items[index] = items[largest];
		items[largest] = hold;

		Heapify(items, count, largest, reverse);
	}
}