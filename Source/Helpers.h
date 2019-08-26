#pragma once
#include <chrono>
#include <vector>

struct SortItem
{
	int ID;
	float value;

	SortItem(int IDInt, float valueFloat) : ID(IDInt), value(valueFloat){}
};
void InsertSort(std::vector<SortItem>&);

class RandomGenerator
{
public:
	static int RandomInt(int, int);
	static float RandomFloat(float, float);
};