#pragma once
#include <SFML/Graphics.hpp>

struct RectangleShape
{
public:
	sf::RectangleShape rectangle;
	bool enabled;
	int pointerIndex;
	int depth;
	void(*onClick)(void*);

	RectangleShape();
	RectangleShape(sf::RectangleShape&);
	RectangleShape(sf::RectangleShape&, bool);

	void SetRectangle(float,float, float,float, float,float,float,float, float,float,float,float, float);
};
struct Text
{
public:
	sf::Text text;
	bool enabled;
	int pointerIndex;
	int depth;
	void(*onClick)(void*);

	Text();
	Text(sf::Text&);
	Text(sf::Text&, bool);

	void LeftAlignment();
	void MiddleAlignment();
	void RightAlignment();
};

struct Sprite
{
public:
	sf::Sprite sprite;
	bool enabled;
	int pointerIndex;
	int depth;
	void(*onClick)(void*);

	Sprite();
	Sprite(sf::Sprite&);
	Sprite(sf::Sprite&, bool);
};