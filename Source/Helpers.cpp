#include "Helpers.h"
#include <chrono>

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