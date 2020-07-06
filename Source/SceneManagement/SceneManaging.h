#pragma once

#include <Rendering/RenderObjects.h>
#include <Rendering/Lighting.h>
#include <HelpfulScripts/Rotater.h>

#include <Windows.h>
#include <iostream>

class SceneManager
{
public:
	SceneManager();

	void SaveScene(RenderManager&, LightingManager&, Rotater&, glm::vec4&, std::string);
	void LoadScene(RenderManager&, LightingManager&, Rotater&, glm::vec4&, std::string);

	void ReadDirectory(std::string, std::vector<std::string>&);
};
