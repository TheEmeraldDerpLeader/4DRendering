#include <SFML/Main.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <GL/glew.h>

#include "ShaderHandler.h"
#include "RenderObjects.h"
#include "Helpers.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Modulo.h"
#include <iostream>
#include <vector>
#include <deque>

void ProcessInput(sf::Window*);

char vertexShader[] = "Assets\\Shaders\\VertexShader.glsl";
char fragmentShader[] = "Assets\\Shaders\\FragmentShader.glsl";

const int screenx = 800;
const int screeny = 600;

Camera camera;
RenderManager renderManager;

glm::vec2 lastMouse(screenx / 2, screeny / 2);
bool firstMouse = true;
bool cursorLock = true;

unsigned int VBO, VAO;

float moveSpeed = 1.0f;
float rotationSpeed = 45.0f;
float deltaTime;

//TODO: Add textures to VAO and texture coords to cross sectioning

int main(){
	glm::mat4x4 perspective = glm::perspective(45.0f, (float)screenx / (float)screeny, 0.1f, 100.0f);

	camera.position = glm::vec4(0, 0, 3, 0.1f);

	//On start, kernel: 4ms, cpu: 3ms. With beeg wall, kernel: 10ms, cpu: 21ms
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			for (int w = 0; w < 2; w++)
			{
				renderManager.pentaRenderables.push_back(Renderable(glm::mat4x4(1), glm::vec4(-0+ x,-0 + y,-3,-0 + w), 0));
			}
		}
	}

	sf::ContextSettings settings;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	settings.depthBits = 24;
	sf::RenderWindow window(sf::VideoMode(screenx,screeny), "4D Rendering", sf::Style::Default, settings);
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	//window.setVerticalSyncEnabled(true);
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE); decreases triangles to shader for cull checks. Currently inefficent unless vertex shader is complex
	glFrontFace(GL_CW);

	Shader shader(vertexShader, fragmentShader);
	shader.use();
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	unsigned int stride = sizeof(float)*9;
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0); //Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3*sizeof(float))); //Color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float))); //Texture coords
	glEnableVertexAttribArray(2);

	unsigned int texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_3D, texture);
	unsigned char data[2][2][2][4];
#pragma region Texture Data
	data[0][0][0][0] = (unsigned char)255;
	data[0][0][0][1] = (unsigned char)255;
	data[0][0][0][2] = (unsigned char)255;
	data[0][0][0][3] = (unsigned char)255;
	data[0][0][1][0] = (unsigned char)255;
	data[0][0][1][1] = (unsigned char)0;
	data[0][0][1][2] = (unsigned char)0;
	data[0][0][1][3] = (unsigned char)255;
	data[0][1][0][0] = (unsigned char)0;
	data[0][1][0][1] = (unsigned char)255;
	data[0][1][0][2] = (unsigned char)0;
	data[0][1][0][3] = (unsigned char)255;
	data[0][1][1][0] = (unsigned char)255;
	data[0][1][1][1] = (unsigned char)255;
	data[0][1][1][2] = (unsigned char)255;
	data[0][1][1][3] = (unsigned char)255;
	data[1][0][0][0] = (unsigned char)0;
	data[1][0][0][1] = (unsigned char)0;
	data[1][0][0][2] = (unsigned char)255;
	data[1][0][0][3] = (unsigned char)255;
	data[1][0][1][0] = (unsigned char)255;
	data[1][0][1][1] = (unsigned char)255;
	data[1][0][1][2] = (unsigned char)255;
	data[1][0][1][3] = (unsigned char)255;
	data[1][1][0][0] = (unsigned char)255;
	data[1][1][0][1] = (unsigned char)255;
	data[1][1][0][2] = (unsigned char)255;
	data[1][1][0][3] = (unsigned char)255;
	data[1][1][1][0] = (unsigned char)255;
	data[1][1][1][1] = (unsigned char)255;
	data[1][1][1][2] = (unsigned char)255;
	data[1][1][1][3] = (unsigned char)255;
#pragma endregion
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, 2, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned int perspectiveLoc = glGetUniformLocation(shader.ID, "perspectiveMat");
	glUniformMatrix4fv(perspectiveLoc, 1, GL_FALSE, glm::value_ptr(perspective));

	shader.setInt("texture0", 0);

	window.setMouseCursorVisible(false);
	window.setMouseCursorGrabbed(true);

#pragma region Text Stuff
	sf::Font lemonMilk;
	if (!lemonMilk.loadFromFile("Assets\\Fonts\\LemonMilk.otf"))
	{
		std::cout << "Could not load LemonMilk font\n";
		abort();
	}
	sf::Text positionXText = sf::Text("X", lemonMilk, 32);
	sf::Text positionYText = sf::Text("Y", lemonMilk, 32);
	sf::Text positionZText = sf::Text("Z", lemonMilk, 32);
	sf::Text positionWText = sf::Text("W", lemonMilk, 32);
	sf::Text rotationYZText = sf::Text("YZ", lemonMilk, 32);
	sf::Text rotationZXText = sf::Text("ZX", lemonMilk, 32);
	sf::Text rotationXWText = sf::Text("XW", lemonMilk, 32);
	sf::Text rotationZWText = sf::Text("ZW", lemonMilk, 32);

	positionXText.setFillColor(sf::Color(255, 0, 0, 255));
	positionYText.setFillColor(sf::Color(0, 255, 0, 255));
	positionZText.setFillColor(sf::Color(0, 0, 255, 255));
	positionWText.setFillColor(sf::Color(255, 0, 255, 255));
	rotationYZText.setFillColor(sf::Color(255, 120, 120, 255));
	rotationZXText.setFillColor(sf::Color(120, 255, 120, 255));
	rotationXWText.setFillColor(sf::Color(120, 120, 255, 255));
	rotationZWText.setFillColor(sf::Color(255, 120, 255, 255));

	positionXText.setPosition(5, 5);
	positionYText.setPosition(5, 55);
	positionZText.setPosition(5, 105);
	positionWText.setPosition(5, 155);
	rotationYZText.setPosition(5, screeny - 187);
	rotationZXText.setPosition(5, screeny - 137);
	rotationXWText.setPosition(5, screeny - 87);
	rotationZWText.setPosition(5, screeny - 37);
#pragma endregion

	sf::Clock frameTime;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::Resized)
			{
				// adjust the viewport when the window is resized
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}

		deltaTime = frameTime.getElapsedTime().asSeconds();
		std::cout << deltaTime << '\n';
		frameTime.restart();
		window.setActive(true);
		ProcessInput(&window);
		window.clear(sf::Color((0.2f * 255),(0.3f * 255),(0.3f * 255), 255));
		glClear(GL_DEPTH_BUFFER_BIT);

		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, texture);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		
		int vertexNumber = renderManager.SetBuffer(camera, VBO);
		glDrawArrays(GL_TRIANGLES, 0, vertexNumber);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		window.pushGLStates();
		positionXText.setString("X: " + std::to_string(camera.position.x));
		positionYText.setString("Y: " + std::to_string(camera.position.y));
		positionZText.setString("Z: " + std::to_string(camera.position.z));
		positionWText.setString("W: " + std::to_string(camera.position.w));
		rotationYZText.setString("YZ: " + std::to_string(camera.rotation[1]));
		rotationZXText.setString("ZX: " + std::to_string(camera.rotation[2]));
		rotationXWText.setString("XW: " + std::to_string(camera.rotation[3]));
		rotationZWText.setString("ZW: " + std::to_string(camera.rotation[5]));
		window.draw(positionXText);
		window.draw(positionYText);
		window.draw(positionZText);
		window.draw(positionWText);
		window.draw(rotationYZText);
		window.draw(rotationZXText);
		window.draw(rotationXWText);
		window.draw(rotationZWText);
		window.popGLStates();

		window.display();
		window.setActive(false);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	return 0;
}

void ProcessInput(sf::Window* window)
{
	glm::mat4x4 rotation = glm::inverse(camera.GetTransform());
	//Window input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		window->close();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		if (cursorLock)
		{
			cursorLock = false;
			window->setMouseCursorVisible(true);
			window->setMouseCursorGrabbed(false);
		}
		else
		{
			cursorLock = true;
			firstMouse = true;
			window->setMouseCursorVisible(false);
			window->setMouseCursorGrabbed(true);
		}
	}
	//Movement Input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		moveSpeed = 2.0f;
	}
	else
	{
		moveSpeed = 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		camera.position += rotation * glm::vec4(0, 0, -deltaTime * moveSpeed, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		camera.position += rotation * glm::vec4(0, 0, deltaTime * moveSpeed, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		camera.position += rotation * glm::vec4(deltaTime * moveSpeed, 0, 0, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		camera.position += rotation * glm::vec4(-deltaTime * moveSpeed, 0, 0, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		camera.position += rotation * glm::vec4(0, deltaTime * moveSpeed, 0, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		camera.position += rotation * glm::vec4(0, -deltaTime * moveSpeed, 0, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
	{
		camera.position += rotation * glm::vec4(0, 0, 0, deltaTime * moveSpeed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		camera.position += rotation * glm::vec4(0, 0, 0, -deltaTime * moveSpeed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
	{
		std::cout << glGetError() << '\n';
		abort();
		if (renderManager.renderMode == false)
			renderManager.renderMode = true;
		else
			renderManager.renderMode = false;
	
	}
	//Mouse input
	if (firstMouse) // this bool variable is initially set to true
	{
		sf::Mouse::setPosition(sf::Vector2i((window->getPosition().x + (screenx / 2)), (window->getPosition().y + (screeny / 2))));
		firstMouse = false;
	}
	float xOffset = sf::Mouse::getPosition().x - (window->getPosition().x + (screenx / 2));
	float yOffset = sf::Mouse::getPosition().y - (window->getPosition().y + (screeny / 2));
	if (!cursorLock)
		return;

	float sensitivity = 0.25f;
	xOffset *= sensitivity;
	yOffset *= -sensitivity;
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		camera.RotateZX(-xOffset * deltaTime * rotationSpeed);
		camera.RotateYZ(yOffset * deltaTime * rotationSpeed);
	}
	else
	{
		camera.RotateXW(xOffset * deltaTime * rotationSpeed);
		camera.RotateZW(yOffset * deltaTime * rotationSpeed);
	}
	sf::Mouse::setPosition(sf::Vector2i((window->getPosition().x + (screenx / 2)), (window->getPosition().y + (screeny / 2))));
}