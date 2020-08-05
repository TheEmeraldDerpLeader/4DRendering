#pragma once

#include <Rendering/RenderObjects.h>
#include <Rendering/Lighting.h>
#include <HelpfulScripts/Rotater.h>

#include <Windows.h>
#include <iostream>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

class SceneManager
{
public:
	std::vector<std::string> directoryFiles;
	SceneManager();

	void SaveScene(RenderManager&, LightingManager&, Rotater&, glm::vec4&, std::string);
	void LoadScene(RenderManager&, LightingManager&, Rotater&, glm::vec4&, std::string);

	void ReadDirectory(std::string, unsigned int);
};
