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

void InsertSort(std::vector<SortItem>& items)
{
	int itemSize = items.size();

	for (int i = 1; i < itemSize; i++)
	{
		SortItem item = items[i];
		for (int b = 1; b <= i; b++)
		{
			SortItem* checkItem = &(items[i - b]);
			if (item.value < checkItem->value)
			{
				SortItem swapper = *checkItem;
				*checkItem = items[i - (b-1)];
				items[i - (b-1)] = swapper;
			}
			else
			{
				break;
			}
		}
	}
}

