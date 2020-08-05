#include "GUIManaging.h"

GUIManager::GUIManager()
{
	fonts.resize(1);
	if (!fonts[0].loadFromFile("Assets\\Fonts\\Vera\\VeraBd.ttf"))
	{
		std::cout << "Could not load font\n";
		abort();
	}
	screenX = 800;
	screenY = 600;
}

GUIManager::GUIManager(int screenXInt, int screenYInt)
{
	fonts.resize(1);
	if (!fonts[0].loadFromFile("Assets\\Fonts\\Vera\\VeraBd.ttf"))
	{
		std::cout << "Could not load font\n";
		abort();
	}
	screenX = screenXInt;
	screenY = screenYInt;
}

void GUIManager::Draw(sf::RenderWindow* window)
{
	int textSize = texts.size();
	int rectangleSize = rectangles.size();
	int spriteSize = sprites.size();
	SmartArray<SortItem<int>> depthArray(textSize+rectangleSize+spriteSize);
	for (int i = 0; i < textSize; i++)
	{
		depthArray[i].data = i;
		depthArray[i].value = texts[i].data->depth;
	}
	for (int i = 0; i < rectangleSize; i++)
	{
		depthArray[i+textSize].data = i+textSize;
		depthArray[i+textSize].value = rectangles[i].data->depth;
	}
	for (int i = 0; i < spriteSize; i++)
	{
		depthArray[i+textSize+rectangleSize].data = i+textSize+rectangleSize;
		depthArray[i+textSize+rectangleSize].value = sprites[i].data->depth;
	}
	HeapSort<int>(depthArray, false);
	for (int i = 0; i < textSize+rectangleSize+spriteSize; i++)
	{
		SortItem<int>& item = depthArray[i];
		if (item.data >= rectangleSize+textSize)
		{
			if (sprites[item.data-textSize-rectangleSize].data->enabled)
				window->draw(sprites[item.data - textSize - rectangleSize].data->sprite);
		} 
		else if (item.data >= textSize)
		{
			if (rectangles[item.data - textSize].data->enabled)
				window->draw(rectangles[item.data - textSize].data->rectangle);
		} 
		else
		{
			if (texts[item.data].data->enabled)
				window->draw(texts[item.data].data->text);
		}
	}
}

void* GUIManager::AddBack(int type, int depth)
{
	switch (type)
	{
	case 0:
	{
		texts.push_back(Unique<Text>());
		Unique<Text>& textRef = texts[texts.size() - 1];
		textRef.Reassign(new Text());
		textRef.data->pointerIndex = texts.size() - 1;
		textRef.data->depth = depth;
		return textRef.data;
	}
	break;
	case 1:
	{
		rectangles.push_back(Unique<RectangleShape>());
		Unique<RectangleShape>& rectangleRef = rectangles[rectangles.size() - 1];
		rectangleRef.Reassign(new RectangleShape());
		rectangleRef.data->pointerIndex = rectangles.size() - 1;
		rectangleRef.data->depth = depth;
		return rectangleRef.data;
	}
	break;
	case 2:
	{
		sprites.push_back(Unique<Sprite>());
		Unique<Sprite>& spriteRef = sprites[sprites.size() - 1];
		spriteRef.Reassign(new Sprite());
		spriteRef.data->pointerIndex = sprites.size() - 1;
		spriteRef.data->depth = depth;
		return spriteRef.data;
	}
	break;
	}
}

void GUIManager::Remove(int type, int index)
{
	switch (type)
	{
	case 0:
		texts[index].~Unique();
		for (int i = index; i < texts.size() - 1; i++)
		{
			texts[i] = texts[i + 1];
			texts[i].data->pointerIndex--;
		}
		texts[texts.size()].data = nullptr;
		texts.pop_back();
		break;
	case 1:
		rectangles[index].~Unique();
		for (int i = index; i < rectangles.size() - 1; i++)
		{
			rectangles[i] = rectangles[i + 1];
			rectangles[i].data->pointerIndex--;
		}
		rectangles[rectangles.size()].data = nullptr;
		rectangles.pop_back();
		break;
	case 2:
		sprites[index].~Unique();
		for (int i = index; i < sprites.size() - 1; i++)
		{
			sprites[i] = sprites[i + 1];
			sprites[i].data->pointerIndex--;
		}
		sprites[sprites.size()].data = nullptr;
		sprites.pop_back();
		break;
	}
}

