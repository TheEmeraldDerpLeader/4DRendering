#pragma once
#include <chrono>
#include <vector>

struct SortItem
{
	int ID;
	float value;

	SortItem() : ID(0), value(0){}
	SortItem(int IDInt, float valueFloat) : ID(IDInt), value(valueFloat){}
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
			delete data;
			data = nullptr;
		}
	}
};

void InsertSort(SmartArray<SortItem>&);
