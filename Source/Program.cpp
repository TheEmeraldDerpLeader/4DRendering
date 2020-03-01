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

void CreateTexture(int, unsigned char*, unsigned int, unsigned int, unsigned int, Texture&);
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
bool terminated = false;

const unsigned int textureCount = 4;
unsigned int VBO[textureCount];
unsigned int VAO[textureCount];
unsigned int errorCode;

float moveSpeed = 1.0f;
float rotationSpeed = 45.0f;
float deltaTime;
float cursorLockWait = 0.0f;

Texture textures[textureCount];

//TODO: Transparency D:

//Later: Point lighting system

int main()
{
	glm::mat4x4 perspective = glm::perspective(45.0f, (float)screenx / (float)screeny, 0.1f, 100.0f);

	camera.position = glm::vec4(0, 0, 6, 0.1f);
	camera.RotateXW(0.5f);
	glm::mat4x4 derpTest = RotateMat(0, 0, 0, 0, 0, 0);
	// modelID, textureID, cellCount
	renderManager.tetraRenderables.push_back(Renderable(glm::mat4x4(1), glm::vec4(0, 0, 0, 0), 0, 0, 5));
	renderManager.tetraRenderables.push_back(Renderable(glm::mat4x4(1), glm::vec4(2, 0, 0, 0), 1, 1, 5));
	renderManager.tetraRenderables.push_back(Renderable(glm::mat4x4(3), glm::vec4(-5, 0, 0, -0.5f), 2, 2, 14));
	//renderManager.tetraRenderables.push_back(Renderable(glm::mat4x4(1), glm::vec4(0, 0, 0, -0.5f), 2, 2, 14));
	renderManager.hexaRenderables.push_back(Renderable(derpTest, glm::vec4(0,-2,0,0), 0, 1, 8));

	sf::ContextSettings settings;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	settings.depthBits = 24;
	sf::RenderWindow window(sf::VideoMode(screenx,screeny), "4D Rendering", sf::Style::Default, settings);
	//window.setVerticalSyncEnabled(true);
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE); decreases triangles to shader for cull checks. Currently inefficent unless vertex shader is complex
	glFrontFace(GL_CW);

	Shader shader(vertexShader, fragmentShader);
	shader.use();
	glGenBuffers(textureCount, VBO);
	glGenVertexArrays(textureCount, VAO);

	unsigned int stride = sizeof(float)*9;

	for (int i = 0; i < textureCount; i++)
	{
		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0); //Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float))); //Color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float))); //Texture coords
		glEnableVertexAttribArray(2);
	}

	CreateTexture(6, nullptr, 8, 8, 8, textures[0]);
	CreateTexture(1, nullptr, 128, 128, 128, textures[1]);
	CreateTexture(2, nullptr, 128, 128, 128, textures[2]);
	CreateTexture(4, nullptr, 128, 128, 128, textures[3]);

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

	errorCode = glGetError();
	if (errorCode != 0)
	{
		std::cout << errorCode << '\n';
		abort();
	}
	unsigned int bufferSizes[textureCount];
	sf::Clock frameTime;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
			{
				terminated = true;
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
		if (!terminated)
		{
			window.clear(sf::Color((0.2f * 255), (0.3f * 255), (0.3f * 255), 255));
			glClear(GL_DEPTH_BUFFER_BIT);

			shader.use();
			glActiveTexture(GL_TEXTURE0);

			renderManager.ClearDeques(textureCount);
			renderManager.ModelGenerate(camera);
			renderManager.DynamicGenerate(camera);
			renderManager.HexaModelGenerate(camera);
			renderManager.CopyToBuffer(VBO, bufferSizes);
			errorCode = glGetError();
			for (int i = 0; i < textureCount; i++)
			{
				glBindVertexArray(VAO[i]);
				glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
				textures[i].BindTexture(0);
				glDrawArrays(GL_TRIANGLES, 0, bufferSizes[i]);
				glGetError();
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glUseProgram(0);
			errorCode = glGetError();
			if (errorCode != 0)
			{
				std::cout << errorCode << '\n';
				abort();
			}

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
		}
		window.setActive(false);
	}

	glDeleteVertexArrays(textureCount, VAO);
	glDeleteBuffers(textureCount, VBO);
	return 0;
}

void CreateTexture(int presetNumber, unsigned char* data, unsigned int xDim, unsigned int yDim, unsigned int zDim, Texture& texture)
{
	unsigned int texP;
	glGenTextures(1, &texP);
	texture.id = texP;
	texture.usesAlpha = false;
	glBindTexture(GL_TEXTURE_3D, texP);
	bool initData = false;
	if (data == nullptr)
	{
		initData = true;
		data = new unsigned char[xDim * yDim * zDim * 4];
	}
	switch (presetNumber)
	{
	case 0:
#pragma region RedGradient128
		for (int z = 0; z < 128; z++)
		{
			for (int y = 0; y < 128; y++)
			{
				for (int x = 0; x < 128; x++)
				{
					data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 0] = (unsigned char)(255);
					data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 1] = (unsigned char)(255 - (x * 2));
					data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 2] = (unsigned char)(255 - (x * 2));
					data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 3] = (unsigned char)(255);
				}
			}
		}
#pragma endregion
		break;
	case 1:
#pragma region Celestial128
		for (int z = 0; z < 128; z++)
		{
			for (int y = 0; y < 128; y++)
			{
				for (int x = 0; x < 128; x++)
				{
					data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 0] = (unsigned char)(255 - (x * 2));
					data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 1] = (unsigned char)(255 - (y * 2));
					data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 2] = (unsigned char)(255 - (z * 2));
					data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 3] = (unsigned char)(255);
				}
			}
		}
#pragma endregion
		break;
	case 2:
#pragma region RGBColors128
		for (int z = 0; z < 128; z++)
		{
			for (int y = 0; y < 128; y++)
			{
				for (int x = 0; x < 128; x++)
				{
					data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 0] = (unsigned char)(255 - ((127 - x) * 2));
					data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 1] = (unsigned char)(255 - ((127 - y) * 2));
					data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 2] = (unsigned char)(255 - ((127 - z) * 2));
					data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 3] = (unsigned char)(255);
				}
			}
		}
#pragma endregion
		break;
	case 3:
#pragma region UniformStripes128
		for (int z = 0; z < 128; z++)
		{
			for (int y = 0; y < 128; y++)
			{
				for (int x = 0; x < 128; x++)
				{
					if (Modulo(x, 16) > 8 || Modulo(y, 16) > 8 || Modulo(z, 16) > 8)
					{
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 0] = (unsigned char)(255);
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 1] = (unsigned char)(0);
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 2] = (unsigned char)(0);
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 3] = (unsigned char)(255);
					}
					else
					{
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 0] = (unsigned char)(255);
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 1] = (unsigned char)(255);
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 2] = (unsigned char)(255);
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 3] = (unsigned char)(255);
					}
				}
			}
		}
#pragma endregion
		break;
	case -1:
#pragma region Legacy2
		data[0] = (unsigned char)255;
		data[1] = (unsigned char)255;
		data[2] = (unsigned char)255;
		data[3] = (unsigned char)255;
		data[4] = (unsigned char)255;
		data[5] = (unsigned char)0;
		data[6] = (unsigned char)0;
		data[7] = (unsigned char)255;
		data[8] = (unsigned char)0;
		data[9] = (unsigned char)255;
		data[10] = (unsigned char)0;
		data[11] = (unsigned char)255;
		data[12] = (unsigned char)255;
		data[13] = (unsigned char)255;
		data[14] = (unsigned char)255;
		data[15] = (unsigned char)255;
		data[16] = (unsigned char)0;
		data[17] = (unsigned char)0;
		data[18] = (unsigned char)255;
		data[19] = (unsigned char)255;
		data[20] = (unsigned char)255;
		data[21] = (unsigned char)255;
		data[22] = (unsigned char)255;
		data[23] = (unsigned char)255;
		data[24] = (unsigned char)255;
		data[25] = (unsigned char)255;
		data[26] = (unsigned char)255;
		data[27] = (unsigned char)255;
		data[28] = (unsigned char)255;
		data[29] = (unsigned char)255;
		data[30] = (unsigned char)255;
		data[31] = (unsigned char)255;
#pragma endregion
		break;
	case 4:
#pragma region UnuniformStripes128
		for (int z = 0; z < 128; z++)
		{
			for (int y = 0; y < 128; y++)
			{
				for (int x = 0; x < 128; x++)
				{
					if (Modulo(x, 16) > 8)
					{
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 0] = (unsigned char)(255);
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 1] = (unsigned char)(0);
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 2] = (unsigned char)(0);
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 3] = (unsigned char)(255);
					}
					else if (Modulo(y, 16) > 8)
					{
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 0] = (unsigned char)(0);
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 1] = (unsigned char)(255);
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 2] = (unsigned char)(0);
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 3] = (unsigned char)(255);
					}
					else if (Modulo(z, 16) > 8)
					{
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 0] = (unsigned char)(0);
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 1] = (unsigned char)(0);
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 2] = (unsigned char)(255);
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 3] = (unsigned char)(255);
					}
					else
					{
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 0] = (unsigned char)(255);
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 1] = (unsigned char)(255);
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 2] = (unsigned char)(255);
						data[(((z*(128 * 128)) + (y * 128) + x) * 4) + 3] = (unsigned char)(255);
					}
				}
			}
		}
#pragma endregion
		break;
	case 5:
#pragma region Checkers128
		for (int z = 0; z < 16; z++)
		{
			for (int y = 0; y < 16; y++)
			{
				for (int x = 0; x < 16; x++)
				{
					if (Modulo(Modulo(x, 2) + Modulo(y, 2) + Modulo(z, 2), 2) == 1)
					{
						for (int px = 0; px < 8; px++)
						{
							for (int py = 0; py < 8; py++)
							{
								for (int pz = 0; pz < 8; pz++)
								{
									int gx = (x * 8) + px;
									int gy = (y * 8) + py;
									int gz = (z * 8) + pz;
									data[(((gz*(128 * 128)) + (gy * 128) + gx) * 4) + 0] = (unsigned char)(255);
									data[(((gz*(128 * 128)) + (gy * 128) + gx) * 4) + 1] = (unsigned char)(0);
									data[(((gz*(128 * 128)) + (gy * 128) + gx) * 4) + 2] = (unsigned char)(0);
									data[(((gz*(128 * 128)) + (gy * 128) + gx) * 4) + 3] = (unsigned char)(255);
								}
							}
						}
					}
					else
					{
						for (int px = 0; px < 8; px++)
						{
							for (int py = 0; py < 8; py++)
							{
								for (int pz = 0; pz < 8; pz++)
								{
									int gx = (x * 8) + px;
									int gy = (y * 8) + py;
									int gz = (z * 8) + pz;
									data[(((gz*(128 * 128)) + (gy * 128) + gx) * 4) + 0] = (unsigned char)(255);
									data[(((gz*(128 * 128)) + (gy * 128) + gx) * 4) + 1] = (unsigned char)(255);
									data[(((gz*(128 * 128)) + (gy * 128) + gx) * 4) + 2] = (unsigned char)(255);
									data[(((gz*(128 * 128)) + (gy * 128) + gx) * 4) + 3] = (unsigned char)(255);
								}
							}
						}
					}
				}
			}
		}
#pragma endregion
		break;
	case 6:
#pragma region ColorLines8
		for (int x = 0; x < 8; x++)
		{
			for (int y = 0; y < 8; y++)
			{
				for (int z = 0; z < 8; z++)
				{
					data[(((z*(64)) + (y * 8) + x) * 4) + 0] = (unsigned char)0;
					data[(((z*(64)) + (y * 8) + x) * 4) + 1] = (unsigned char)(0);
					data[(((z*(64)) + (y * 8) + x) * 4) + 2] = (unsigned char)(0);
					data[(((z*(64)) + (y * 8) + x) * 4) + 3] = (unsigned char)(255);
					switch (x)
					{
					case 0:
						break;
					case 1:
						data[(((z*(64)) + (y * 8) + x) * 4) + 0] = (unsigned char)(255);
						break;
					case 2:
						data[(((z*(64)) + (y * 8) + x) * 4) + 1] = (unsigned char)(255);
						break;
					case 3:
						data[(((z*(64)) + (y * 8) + x) * 4) + 0] = (unsigned char)(255);
						data[(((z*(64)) + (y * 8) + x) * 4) + 1] = (unsigned char)(255);
						break;
					case 4:
						data[(((z*(64)) + (y * 8) + x) * 4) + 2] = (unsigned char)(255);
						break;
					case 5:
						data[(((z*(64)) + (y * 8) + x) * 4) + 0] = (unsigned char)(255);
						data[(((z*(64)) + (y * 8) + x) * 4) + 2] = (unsigned char)(255);
						break;
					case 6:
						data[(((z*(64)) + (y * 8) + x) * 4) + 1] = (unsigned char)(255);
						data[(((z*(64)) + (y * 8) + x) * 4) + 2] = (unsigned char)(255);
						break;
					case 7:
						data[(((z*(64)) + (y * 8) + x) * 4) + 0] = (unsigned char)(255);
						data[(((z*(64)) + (y * 8) + x) * 4) + 1] = (unsigned char)(255);
						data[(((z*(64)) + (y * 8) + x) * 4) + 2] = (unsigned char)(255);
						break;
					}
				}
			}
		}
#pragma endregion
		break;
	default:
		std::cout << "Invalid preset number\n";
		abort();
		break;
	}
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, xDim, yDim, zDim, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if (initData)
		delete data;
}

void ProcessInput(sf::Window* window)
{
	glm::mat4x4 rotation = glm::inverse(camera.GetTransform());
	//Window input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		terminated = true;
		window->close();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && cursorLockWait == 0)
	{
		if (cursorLock)
		{
			cursorLock = false;
			cursorLockWait = 0.5f;
			window->setMouseCursorVisible(true);
			window->setMouseCursorGrabbed(false);
		}
		else
		{
			cursorLock = true;
			firstMouse = true;
			cursorLockWait = 0.5f;
			window->setMouseCursorVisible(false);
			window->setMouseCursorGrabbed(true);
		}
	}
	if (cursorLock)
	{
		//Movement Input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			moveSpeed = 2.0f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		{
			moveSpeed = 0.5f;
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			camera.RotateYZ(60.0f*deltaTime);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			camera.RotateYZ(-60.0f*deltaTime);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			camera.RotateZX(-60.0f*deltaTime);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			camera.RotateZX(60.0f*deltaTime);
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

		//Mouse input
		if (firstMouse) // this bool variable is initially set to true
		{
			sf::Mouse::setPosition(sf::Vector2i((window->getPosition().x + (screenx / 2)), (window->getPosition().y + (screeny / 2))));
			firstMouse = false;
		}
		float xOffset = sf::Mouse::getPosition().x - (window->getPosition().x + (screenx / 2));
		float yOffset = sf::Mouse::getPosition().y - (window->getPosition().y + (screeny / 2));

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
	if (cursorLockWait != 0)
	{
		cursorLockWait -= deltaTime;
		if (cursorLockWait < 0)
			cursorLockWait = 0;
	}
}
//Sphere approx
/*
	{
		int accCount = 8;
		for (int u = 0; u < accCount; u++)
		{
			for (int v = 0; v < accCount; v++)
			{
				for (int w = 0; w < accCount; w++)
				{
					glm::vec4 points[8];
					glm::vec3 texes[8];
					for (int x = 0; x < 2; x++)
					{
						for (int y = 0; y < 2; y++)
						{
							for (int z = 0; z < 2; z++)
							{
								points[(z * 4) + (y * 2) + x] = glm::vec4(
									cos(1 * PI*(double)(u + x) / (double)accCount)*cos(1 * PI*(double)(v + y) / (double)accCount)*cos(1 * PI*(double)(w + z) / (double)accCount),
									sin(1 * PI*(double)(u + x) / (double)accCount) + 4,
									cos(1 * PI*(double)(u + x) / (double)accCount)*sin(1 * PI*(double)(v + y) / (double)accCount),
									cos(1 * PI*(double)(u + x) / (double)accCount)*cos(1 * PI*(double)(v + y) / (double)accCount)*sin(1 * PI*(double)(w + z) / (double)accCount));
								texes[(z * 4) + (y * 2) + x] = glm::vec3(
									(sin(1 * PI*(double)(u + x) / (double)accCount)/2.0f)+0.5f,
									(cos(1 * PI*(double)(u + x) / (double)accCount)*sin(1 * PI*(double)(v + y) / (double)accCount)/2.0f)+0.5f,
									(cos(1 * PI*(double)(u + x) / (double)accCount)*cos(1 * PI*(double)(v + y) / (double)accCount)*sin(1 * PI*(double)(w + z) / (double)accCount)/2.0f)+0.5f);
							}
						}
					}
					renderManager.dynamicTetras.push_back(Tetrahedron(points[0], texes[0], points[1], texes[1], points[2], texes[2], points[4], texes[4]));
					renderManager.dynamicTex.push_back(3);
					renderManager.dynamicTetras.push_back(Tetrahedron(points[7], texes[7], points[1], texes[1], points[2], texes[2], points[4], texes[4]));
					renderManager.dynamicTex.push_back(3);
					renderManager.dynamicTetras.push_back(Tetrahedron(points[7], texes[7], points[1], texes[1], points[5], texes[5], points[4], texes[4]));
					renderManager.dynamicTex.push_back(3);
					renderManager.dynamicTetras.push_back(Tetrahedron(points[7], texes[7], points[1], texes[1], points[2], texes[2], points[3], texes[3]));
					renderManager.dynamicTex.push_back(3);
					renderManager.dynamicTetras.push_back(Tetrahedron(points[7], texes[7], points[6], texes[6], points[2], texes[2], points[4], texes[4]));
					renderManager.dynamicTex.push_back(3);
				}
			}
		}
		for (int u = 0; u < accCount; u++)
		{
			for (int v = 0; v < accCount; v++)
			{
				for (int w = 0; w < accCount; w++)
				{
					glm::vec4 points[8];
					glm::vec3 texes[8];
					for (int x = 0; x < 2; x++)
					{
						for (int y = 0; y < 2; y++)
						{
							for (int z = 0; z < 2; z++)
							{
								points[(z * 4) + (y * 2) + x] = glm::vec4(
									cos(1 * PI*(double)(u + x) / (double)accCount)*cos(1 * PI*(double)(v + y) / (double)accCount)*cos(1 * PI*(double)(w + z) / (double)accCount),
									-sin(1 * PI*(double)(u + x) / (double)accCount) + 4,
									cos(1 * PI*(double)(u + x) / (double)accCount)*sin(1 * PI*(double)(v + y) / (double)accCount),
									cos(1 * PI*(double)(u + x) / (double)accCount)*cos(1 * PI*(double)(v + y) / (double)accCount)*sin(1 * PI*(double)(w + z) / (double)accCount));
								texes[(z * 4) + (y * 2) + x] = glm::vec3(
									(sin(1 * PI*(double)(u + x) / (double)accCount) / 2.0f) + 0.5f,
									(cos(1 * PI*(double)(u + x) / (double)accCount)*sin(1 * PI*(double)(v + y) / (double)accCount) / 2.0f) + 0.5f,
									(cos(1 * PI*(double)(u + x) / (double)accCount)*cos(1 * PI*(double)(v + y) / (double)accCount)*sin(1 * PI*(double)(w + z) / (double)accCount) / 2.0f) + 0.5f);
							}
						}
					}
					renderManager.dynamicTetras.push_back(Tetrahedron(points[0], texes[0], points[1], texes[1], points[2], texes[2], points[4], texes[4]));
					renderManager.dynamicTex.push_back(3);
					renderManager.dynamicTetras.push_back(Tetrahedron(points[7], texes[7], points[1], texes[1], points[2], texes[2], points[4], texes[4]));
					renderManager.dynamicTex.push_back(3);
					renderManager.dynamicTetras.push_back(Tetrahedron(points[7], texes[7], points[1], texes[1], points[5], texes[5], points[4], texes[4]));
					renderManager.dynamicTex.push_back(3);
					renderManager.dynamicTetras.push_back(Tetrahedron(points[7], texes[7], points[1], texes[1], points[2], texes[2], points[3], texes[3]));
					renderManager.dynamicTex.push_back(3);
					renderManager.dynamicTetras.push_back(Tetrahedron(points[7], texes[7], points[6], texes[6], points[2], texes[2], points[4], texes[4]));
					renderManager.dynamicTex.push_back(3);

				}
			}
		}
	}
*/