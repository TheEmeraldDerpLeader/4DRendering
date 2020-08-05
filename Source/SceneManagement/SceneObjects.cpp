#include "SceneObjects.h"
RectangleShape::RectangleShape()
{
	rectangle = sf::RectangleShape();
	enabled = true;
	onClick = nullptr;
}
RectangleShape::RectangleShape(sf::RectangleShape& rectangleRectangleShape)
{
	rectangle = rectangleRectangleShape;
	enabled = true;
	onClick = nullptr;
}
RectangleShape::RectangleShape(sf::RectangleShape& rectangleRectangleShape, bool enabledBool)
{
	rectangle = rectangleRectangleShape;
	enabled = enabledBool;
	onClick = nullptr;
}
void RectangleShape::SetRectangle(float xPos,float yPos, float xSize,float ySize, float fillR,float fillG,float fillB,float fillA, float outR,float outG,float outB,float outA, float outThickness)
{
	rectangle.setPosition(xPos, yPos);
	rectangle.setSize(sf::Vector2f(xSize, ySize));
	rectangle.setFillColor(sf::Color(fillR, fillG, fillB, fillA));
	rectangle.setOutlineColor(sf::Color(outR, outG, outB, outA));
	rectangle.setOutlineThickness(outThickness);
}
Text::Text()
{
	text = sf::Text();
	enabled = true;
	onClick = nullptr;
}
Text::Text(sf::Text& textText)
{
	text = textText;
	enabled = true;
	onClick = nullptr;
}
Text::Text(sf::Text& textText, bool enabledBool)
{
	text = textText;
	enabled = enabledBool;
	onClick = nullptr;
}
void Text::LeftAlignment()
{
	sf::FloatRect rect = text.getLocalBounds();
	text.setOrigin(rect.left, rect.top);
}
void Text::MiddleAlignment()
{
	sf::FloatRect rect = text.getLocalBounds();
	text.setOrigin(rect.left + (rect.width/2.0f),rect.top);
}
void Text::RightAlignment()
{
	sf::FloatRect rect = text.getLocalBounds();
	text.setOrigin(rect.left+rect.width, rect.top);

}
Sprite::Sprite()
{
	sprite = sf::Sprite();
	enabled = true;
	onClick = nullptr;
}
Sprite::Sprite(sf::Sprite& spriteSprite)
{
	sprite = spriteSprite;
	enabled = true;
	onClick = nullptr;
}
Sprite::Sprite(sf::Sprite& spriteSprite, bool enabledBool)
{
	sprite = spriteSprite;
	enabled = enabledBool;
	onClick = nullptr;
}