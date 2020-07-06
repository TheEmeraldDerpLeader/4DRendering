#include "SceneManaging.h"

SceneManager::SceneManager()
{

}

//8 byte identifier, camera position, camera rotation (euler angle array), tetraModel count, (tetra count, tetras, normals), hexaModel count, (hexa count, hexas, normals),\
tetraRenderable count, tetraRederables, hexaRenderable count, hexaRenderables, dirLight, pointLight count, pointLights, spotLight count, spotLights

void SceneManager::SaveScene(RenderManager& renderManager, LightingManager& lightingManager, Rotater& camera, glm::vec4& position, std::string fileName)
{
	std::ofstream stream(fileName, std::ios::out | std::ios::binary);
	if (stream.good()) //"Scene1.fds"
	{
		char* byteTransfer;

		stream.put('4');
		stream.put('D');
		stream.put('R');
		stream.put('E');
		stream.put('N');
		stream.put('D');
		stream.put('E');
		stream.put('R');
		if (stream.fail())
		{
			stream.close();
			std::cout << "Stream failed to write to file: " << fileName << '\n';
			abort();
		}
		byteTransfer = (char*)&(position.x);
		stream.write(byteTransfer, 16);
		
		byteTransfer = (char*)camera.rotation;
		stream.write(byteTransfer, 24);
		
		int tetraModelCount = renderManager.tetraModelStarts.size();
		int hexaModelCount = renderManager.hexaModelStarts.size();
		int tetraRenderableCount = renderManager.tetraRenderables.size();
		int hexaRenderableCount = renderManager.hexaRenderables.size();

		byteTransfer = (char*)&tetraModelCount;
		stream.write(byteTransfer,4);
		for (int i = 0; i < tetraModelCount; i++)
		{
			int modelIndex = renderManager.tetraModelStarts[i];
			int modelSize;
			if (i != tetraModelCount - 1)
				modelSize = renderManager.tetraModelStarts[i + 1] - renderManager.tetraModelStarts[i];
			else
				modelSize = renderManager.tetraModels.size() - renderManager.tetraModelStarts[i];
			byteTransfer = (char*)&modelSize;
			stream.write(byteTransfer, 4);
			byteTransfer = (char*)&renderManager.tetraModels[modelIndex];
			stream.write(byteTransfer, modelSize * sizeof(Tetrahedron));
			byteTransfer = (char*)&renderManager.tetraNormals[modelIndex];
			stream.write(byteTransfer, modelSize * sizeof(glm::vec4));
		}
		byteTransfer = (char*)&hexaModelCount;
		stream.write(byteTransfer, 4);
		for (int i = 0; i < hexaModelCount; i++)
		{
			int modelIndex = renderManager.hexaModelStarts[i];
			int modelSize;
			if (i != hexaModelCount - 1)
				modelSize = renderManager.hexaModelStarts[i + 1] - renderManager.hexaModelStarts[i];
			else
				modelSize = renderManager.hexaModels.size() - renderManager.hexaModelStarts[i];
			byteTransfer = (char*)&modelSize;
			stream.write(byteTransfer, 4);
			byteTransfer = (char*)&renderManager.hexaModels[modelIndex];
			stream.write(byteTransfer, modelSize * sizeof(Hexahedron));
			byteTransfer = (char*)&renderManager.hexaNormals[modelIndex];
			stream.write(byteTransfer, modelSize * sizeof(glm::vec4));
		}
		Renderable* renderables = new Renderable[glm::max(tetraRenderableCount, hexaRenderableCount)];
		std::deque<Renderable>::iterator renderableIter;

		byteTransfer = (char*)&tetraRenderableCount;
		stream.write(byteTransfer, 4);
		renderableIter = renderManager.tetraRenderables.begin();
		for (int i = 0; i < tetraRenderableCount; i++)
		{
			renderables[i] = *renderableIter;
			renderableIter++;
		}
		byteTransfer = (char*)renderables;
		stream.write(byteTransfer, tetraRenderableCount * sizeof(Renderable));

		byteTransfer = (char*)&hexaRenderableCount;
		stream.write(byteTransfer, 4);
		renderableIter = renderManager.hexaRenderables.begin();
		for (int i = 0; i < hexaRenderableCount; i++)
		{
			renderables[i] = *renderableIter;
			renderableIter++;
		}
		byteTransfer = (char*)renderables;
		stream.write(byteTransfer, hexaRenderableCount * sizeof(Renderable));
		delete renderables;

		byteTransfer = (char*)&lightingManager.ambient;
		stream.write(byteTransfer, sizeof(glm::vec3));

		byteTransfer = (char*)&lightingManager.sceneDirLight;
		stream.write(byteTransfer, sizeof(DirLight));

		int pointLightCount = lightingManager.pointLights.size();
		byteTransfer = (char*)&pointLightCount;
		stream.write(byteTransfer, 4);
		byteTransfer = (char*)&lightingManager.pointLights[0];
		stream.write(byteTransfer, pointLightCount * sizeof(PointLight));

		int spotLightCount = lightingManager.spotLights.size();
		byteTransfer = (char*)&spotLightCount;
		stream.write(byteTransfer, 4);
		byteTransfer = (char*)&lightingManager.spotLights[0];
		stream.write(byteTransfer, pointLightCount * sizeof(SpotLight));

		stream.flush();
	}
	else
	{
		stream.close();
		std::cout << "Cannot open file: " << fileName << '\n';
		abort();
	}
	if (stream.bad() || !stream.good() || stream.fail())
	{
		std::cout << "stream encountered an error! " << stream.bad() << stream.good() << stream.fail() << stream.eof() << " File: " << fileName << '\n';
		abort();
	}
	stream.close();
}

void SceneManager::LoadScene(RenderManager& renderManager, LightingManager& lightingManager, Rotater& camera, glm::vec4& position, std::string fileName)
{
	renderManager.tetraModels.clear();
	renderManager.tetraModelStarts.clear();
	renderManager.tetraNormals.clear();
	renderManager.hexaModels.clear();
	renderManager.hexaModelStarts.clear();
	renderManager.hexaNormals.clear();
	renderManager.tetraRenderables.clear();
	renderManager.hexaRenderables.clear();

	std::ifstream stream(fileName, std::ios::in | std::ios::binary);
	if (stream.good()) //"Scene1.fds"
	{
		char inputBuffer[1024];

		bool invalidCheck = false;
		stream.read(inputBuffer, 8);
		for (int i = 0; i < 8; i++)
		{
			if (inputBuffer[i] != "4DRENDER"[i])
				invalidCheck = true;
		}
		if (invalidCheck)
		{
			stream.close();
			std::cout << "(identifier) Invalid file: " << fileName << '\n';
			abort();
		}
		stream.read(inputBuffer, 16);
		position.x = *(float*)inputBuffer;
		position.y = *(float*)(inputBuffer + 4);
		position.z = *(float*)(inputBuffer + 8);
		position.w = *(float*)(inputBuffer + 12);
		for (int i = 0; i < 6; i++)
		{
			stream.read(inputBuffer, 4);
			if (!stream.good() || stream.fail())
			{
				std::cout << stream.good() << '\n';
				std::cout << stream.fail() << '\n';
				abort();
			}
			camera.rotation[i] = *(float*)inputBuffer;
		}
		stream.read(inputBuffer, 4);
		int tetraModelCount = *(int*)inputBuffer;
		renderManager.tetraModelStarts.resize(tetraModelCount);
		renderManager.tetraModelStarts[0] = 0;
		for (int i = 0; i < tetraModelCount; i++)
		{
			stream.read(inputBuffer, 4);
			int tetraCount = *(int*)inputBuffer;
			if (i != tetraModelCount - 1)
				renderManager.tetraModelStarts[i + 1] = tetraCount + renderManager.tetraModelStarts[i];
			for (int j = 0; j < tetraCount; j++)
			{
				stream.read(inputBuffer, sizeof(Tetrahedron));
				renderManager.tetraModels.push_back(*(Tetrahedron*)inputBuffer);
			}
			for (int j = 0; j < tetraCount; j++)
			{
				stream.read(inputBuffer, sizeof(glm::vec4));
				renderManager.tetraNormals.push_back(*(glm::vec4*)inputBuffer);
			}
		}
		if (stream.bad() || !stream.good() || stream.fail())
		{
			std::cout << "stream encountered an error! " << stream.bad() << stream.good() << stream.fail() << stream.eof() << " File: " << fileName << '\n';
			abort();
		}
		stream.read(inputBuffer, 4);
		int hexaModelCount = *(int*)inputBuffer;
		renderManager.hexaModelStarts.resize(hexaModelCount);
		renderManager.hexaModelStarts[0] = 0;
		for (int i = 0; i < hexaModelCount; i++)
		{
			stream.read(inputBuffer, 4);
			int hexaCount = *(int*)inputBuffer;
			if (i != hexaModelCount - 1)
				renderManager.hexaModelStarts[i + 1] = hexaCount + renderManager.hexaModelStarts[i];
			for (int j = 0; j < hexaCount; j++)
			{
				stream.read(inputBuffer, sizeof(Hexahedron));
				renderManager.hexaModels.push_back(*(Hexahedron*)inputBuffer);
			}
			for (int j = 0; j < hexaCount; j++)
			{
				stream.read(inputBuffer, sizeof(glm::vec4));
				renderManager.hexaNormals.push_back(*(glm::vec4*)inputBuffer);
			}
		}
		renderManager.SetupModelBuffers();
		stream.read(inputBuffer, 4);
		int tetraRenderableCount = *(int*)inputBuffer;
		for (int i = 0; i < tetraRenderableCount; i++)
		{
			stream.read(inputBuffer, sizeof(Renderable));
			renderManager.tetraRenderables.push_back(*(Renderable*)inputBuffer);
		}
		stream.read(inputBuffer, 4);
		int hexaRenderableCount = *(int*)inputBuffer;
		for (int i = 0; i < hexaRenderableCount; i++)
		{
			stream.read(inputBuffer, sizeof(Renderable));
			renderManager.hexaRenderables.push_back(*(Renderable*)inputBuffer);
		}

		lightingManager.pointLights.clear();
		lightingManager.spotLights.clear();

		stream.read(inputBuffer, sizeof(glm::vec3));
		lightingManager.ambient = *(glm::vec3*)inputBuffer;

		stream.read(inputBuffer, sizeof(DirLight));
		lightingManager.sceneDirLight = *(DirLight*)inputBuffer;

		stream.read(inputBuffer, 4);
		int pointLightCount = *(int*)inputBuffer;
		if (pointLightCount > 100)
		{
			std::cout << "Too many PointLights\n";
			abort();
		}
		lightingManager.pointLights.resize(pointLightCount);
		for (int i = 0; i < pointLightCount; i++)
		{
			stream.read(inputBuffer, sizeof(PointLight));
			lightingManager.pointLights[i] = *(PointLight*)inputBuffer;
		}

		stream.read(inputBuffer, 4);
		int spotLightCount = *(int*)inputBuffer;
		if (spotLightCount > 30)
		{
			std::cout << "Too many SpotLights\n";
			abort();
		}
		lightingManager.spotLights.resize(spotLightCount);
		for (int i = 0; i < spotLightCount; i++)
		{
			stream.read(inputBuffer, sizeof(SpotLight));
			lightingManager.spotLights[i] = *(SpotLight*)inputBuffer;
		}
	}
	else
	{
		stream.close();
		std::cout << "Cannot open file: " << fileName << '\n';
		abort();
	}
	if (stream.bad() || !stream.good() || stream.fail())
	{
		std::cout << "stream encountered an error! " << stream.bad() << stream.good() << stream.fail() << stream.eof() << " File: " << fileName << '\n';
		abort();
	}
	stream.close();
}

void SceneManager::ReadDirectory(std::string name, std::vector<std::string>& filesFound)
{
	std::string pattern(name);
	pattern.append("\\*");
	WIN32_FIND_DATA data;
	HANDLE hFind;
	if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			filesFound.push_back(data.cFileName);
		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}
}