#pragma once
#include <SFML/Main.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <GL/glew.h>

#include <Rendering/RenderObjects.h>
#include <Rendering/Lighting.h>
#include <SceneManagement/SceneManaging.h>
#include <GUI/GUIManaging.h>

#include <HelpfulScripts/Helpers.h>
#include <HelpfulScripts/Rotater.h>
#include <HelpfulScripts/Modulo.h>
#include <HelpfulScripts/ShaderHandler.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <iostream>
#include <vector>
#include <deque>

const unsigned int textureCount = 4;
const unsigned int transTextureCount = 1;
inline unsigned int VBOs[textureCount];
inline unsigned int VAOs[textureCount];
inline unsigned int transVBO;
inline unsigned int transVAO;
inline unsigned int errorCode;

const int screenx = 800;
const int screeny = 600;

inline Rotater camera;
inline glm::vec4 position;

inline glm::vec2 lastMouse(screenx / 2, screeny / 2);
inline bool firstMouse = true;
inline bool cursorLock = true;
inline bool terminated = false;
inline int state = 0;

inline float moveSpeed = 1.0f;
inline float rotationSpeed = 45.0f;
inline float deltaTime;
inline float cursorLockWait = 0.0f;

inline Texture textures[textureCount];
inline Texture transTextures[transTextureCount];

inline Text* hudTexts[9];
inline RectangleShape* mouseRectangle;
