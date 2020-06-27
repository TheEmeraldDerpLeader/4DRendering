#include "Lighting.h"

DirLight::DirLight()
{
	direction = glm::normalize(glm::vec4(1, -1, 0, 0));
	SetColor(glm::vec3(0.8f, 0.8f, 0.8f));
}

DirLight::DirLight(glm::vec4 directionVec, glm::vec3 diffuseVec, glm::vec3 specularVec)
{
	direction = directionVec;
	diffuse = diffuseVec;
	specular = specularVec;
}

void DirLight::SetColor(glm::vec3 color)
{
	diffuse = color;
	specular = color;
}

PointLight::PointLight()
{
	position = glm::vec4(0, 0, 0, 0);
	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032f;
	SetColor(glm::vec3(0.3f, 0.3f, 0.3f));
}

PointLight::PointLight(glm::vec4 positionVec, float constantFloat, float linearFloat, float quadraticFloat, glm::vec3 diffuseVec, glm::vec3 specularVec)
{
	position = positionVec;
	constant = constantFloat;
	linear = linearFloat;
	quadratic = quadraticFloat;
	diffuse = glm::vec4(diffuseVec,0);
	specular = glm::vec4(specularVec,0);
}

void PointLight::SetColor(glm::vec3 color)
{
	diffuse = glm::vec4(color,0);
	specular = glm::vec4(color,0);
}

SpotLight::SpotLight()
{
	position = glm::vec4(0, 0, 0, 0);
	direction = glm::vec4(0, 0, -1, 0);
	SetCutOff(45, 70);
	SetColor(glm::vec3(0.3f, 0.3f, 0.3f));
}

SpotLight::SpotLight(glm::vec4 positionVec, glm::vec4 directionVec, float cutOffFloat, float outCutOffFloat, glm::vec3 diffuseVec, glm::vec3 specularVec)
{
	position = positionVec;
	direction = glm::normalize(directionVec);
	SetCutOff(cutOffFloat, outCutOffFloat);
	diffuse = glm::vec4(diffuseVec.x, diffuseVec.y, diffuseVec.z, 0);
	specular = glm::vec4(specularVec.x, specularVec.y, specularVec.z, 0);
}

void SpotLight::SetColor(glm::vec3 color)
{
	diffuse = glm::vec4(color.x, color.y, color.z, 0);
	specular = glm::vec4(color.x, color.y, color.z, 0);
}

void SpotLight::SetCutOff(float inCutOffFloat, float outCutOffFloat)
{
	cutOff = glm::cos(glm::radians(inCutOffFloat));
	outCutOff = glm::cos(glm::radians(outCutOffFloat));
}

LightingManager::LightingManager()
{
	shader = nullptr;
	ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	sceneDirLight = DirLight();
	pointLights.clear();
	spotLights.clear();
	pointLightSize = 0;
	spotLightSize = 0;
	pointLights.reserve(100);
	spotLights.reserve(30);
}

LightingManager::LightingManager(Shader* programShader)
{
	shader = programShader;
	ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	sceneDirLight = DirLight();
	pointLights.clear();
	spotLights.clear();
	pointLightSize = 0;
	spotLightSize = 0;
	pointLights.reserve(100);
	spotLights.reserve(30);
}

void LightingManager::SetAmbientLoc(std::string uniformName)
{
	ambientLoc = glGetUniformLocation(shader->ID, uniformName.c_str());
}

void LightingManager::SetDirLightLoc(std::string uniformName)
{
	dirLightLoc = glGetUniformLocation(shader->ID, uniformName.c_str());
}

void LightingManager::SetPointIndexLoc(std::string uniformName, int blockIndex)
{
	pointCountIndex = glGetUniformBlockIndex(shader->ID,uniformName.c_str());
	glUniformBlockBinding(shader->ID, pointCountIndex, blockIndex);
}
void LightingManager::SetSpotIndexLoc(std::string uniformName, int blockIndex)
{
	spotCountIndex = glGetUniformBlockIndex(shader->ID, uniformName.c_str());
	glUniformBlockBinding(shader->ID, spotCountIndex, blockIndex);
}

void LightingManager::GenUBOs(int pointIndex, int spotIndex)
{
	glGenBuffers(1, &pointLightUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, pointLightUBO);
	glBufferData(GL_UNIFORM_BUFFER, 6404, NULL, GL_STATIC_DRAW);
	glGenBuffers(1, &spotLightUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, spotLightUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2404, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, pointIndex, pointLightUBO);
	glBindBufferBase(GL_UNIFORM_BUFFER, spotIndex, spotLightUBO);

	glBindBuffer(GL_UNIFORM_BUFFER, pointLightUBO);
	pointLightSize = 0;
	glBufferSubData(GL_UNIFORM_BUFFER, 6400, 4, &pointLightSize);
	glBindBuffer(GL_UNIFORM_BUFFER, spotLightUBO);
	spotLightSize = 0;
	glBufferSubData(GL_UNIFORM_BUFFER, 2400, 4, &spotLightSize);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightingManager::TranferToBuffers(glm::mat4x4 rotation, glm::vec4 position)
{
	PointLight pointLightsHold[100];
	SpotLight spotLightsHold[30];
	if (pointLights.size() > 100)
		abort();
	if (spotLights.size() > 30)
		abort();
	pointLightSize = pointLights.size();
	spotLightSize = spotLights.size();
	for (int i = 0; i < pointLightSize; i++)
	{
		PointLight& p = pointLightsHold[i];
		p = pointLights[i];
		p.position -= position;
		p.position = rotation * p.position;
	}
	for (int i = 0; i < spotLightSize; i++)
	{
		SpotLight& s = spotLightsHold[i];
		s = spotLights[i];
		s.position -= position;
		s.position = rotation * s.position;
		s.direction = rotation * s.direction;
	}
	glBindBuffer(GL_UNIFORM_BUFFER, pointLightUBO);
	if (pointLightSize > 0)
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 64 * pointLightSize, pointLightsHold);
	glBufferSubData(GL_UNIFORM_BUFFER, 6400, 4, &pointLightSize);

	glBindBuffer(GL_UNIFORM_BUFFER, spotLightUBO);
	if (spotLightSize > 0)
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 80 * spotLightSize, spotLightsHold);
	glBufferSubData(GL_UNIFORM_BUFFER, 2400, 4, &spotLightSize);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightingManager::AmbientToShader()
{
	glUniform3fv(ambientLoc, 1, glm::value_ptr(ambient));
}

void LightingManager::DirLightToShader(glm::mat4x4 rotation)
{
	glm::vec4 direction = rotation * sceneDirLight.direction;
	glUniform4fv(dirLightLoc, 1, glm::value_ptr(direction));
	glUniform3fv(dirLightLoc+1, 1, glm::value_ptr(sceneDirLight.diffuse));
	glUniform3fv(dirLightLoc+2, 1, glm::value_ptr(sceneDirLight.specular));
}
