#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderObjects.h"

#include <HelpfulScripts/ShaderHandler.h>
#include <HelpfulScripts/Modulo.h>

#include <iostream>
#include <vector>

struct DirLight
{
public:
	glm::vec4 direction;

	glm::vec3 diffuse;
	glm::vec3 specular;

	DirLight();
	DirLight(glm::vec4, glm::vec3, glm::vec3);

	void SetColor(glm::vec3);
};

struct PointLight
{
public:
	glm::vec4 position;

	float constant;
	float linear;
	float quadratic;
	float padding;

	glm::vec4 diffuse; //vec3 with padding
	glm::vec4 specular; //vec3 with padding

	PointLight();
	PointLight(glm::vec4, float, float, float, glm::vec3, glm::vec3);

	void SetColor(glm::vec3);
};

struct SpotLight
{
public:
	glm::vec4  position;
	glm::vec4  direction;
	float cutOff;
	float outCutOff;
	float padding1;
	float padding2;
	glm::vec4 diffuse; //vec3 with padding
	glm::vec4 specular; //vec3 with padding

	SpotLight();
	SpotLight(glm::vec4, glm::vec4, float, float, glm::vec3, glm::vec3);

	void SetColor(glm::vec3);
	void SetCutOff(float, float);
};

class LightingManager
{
public:
	Shader* shader;
	glm::vec3 ambient;
	unsigned int ambientLoc;
	DirLight sceneDirLight;
	unsigned int dirLightLoc;
	unsigned int pointLightUBO;
	unsigned int spotLightUBO;
	unsigned int pointCountIndex;
	unsigned int spotCountIndex;
	int pointLightSize;
	int spotLightSize;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;

	LightingManager();
	LightingManager(Shader*);

	void SetAmbientLoc(std::string);
	void SetDirLightLoc(std::string);
	void SetPointIndexLoc(std::string, int);
	void SetSpotIndexLoc(std::string, int);
	void GenUBOs(int, int);
	void TranferToBuffers(glm::mat4x4, glm::vec4);
	void AmbientToShader();
	void DirLightToShader(glm::mat4x4);
};
