#pragma once
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <deque>
#include <HelpfulScripts/Helpers.h>
#include <SceneManagement/SceneObjects.h>


class GUIManager
{
public:
	std::deque<Unique<Text>> texts;
	std::deque<Unique<RectangleShape>> rectangles;
	std::deque<Unique<Sprite>> sprites;

	std::vector<sf::Font> fonts;

	int screenX;
	int screenY;

	GUIManager();
	GUIManager(int, int);

	void Draw(sf::RenderWindow*);
	void* AddBack(int, int);
	void Remove(int, int);
};