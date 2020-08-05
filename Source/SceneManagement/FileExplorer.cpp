#include "FileExplorer.h"

FileExplorer::FileExplorer(RenderManager* renderManagerPtr, LightingManager* lightingManagerPtr, SceneManager* sceneManagerPtr, GUIManager* guiManagerPtr)
{
	renderManager = renderManagerPtr;
	lightingManager = lightingManagerPtr;
	sceneManager = sceneManagerPtr;
	guiManager = guiManagerPtr;
}

void FileExplorer::SetupUI()
{
	for (int i = 0; i < explorerTextCount; i++)
	{
		explorerTexts[i] = (Text*)guiManager->AddBack(0, 3);
		explorerTexts[i]->enabled = false;
		explorerTexts[i]->text = sf::Text("Empty", guiManager->fonts[0], 24);
		explorerTexts[i]->text.setFillColor(sf::Color(0, 0, 0));
	}
	explorerTexts[0]->text.setString("Back");
	explorerTexts[0]->onClick = Back;
	explorerTexts[1]->text.setString("Up");
	explorerTexts[1]->onClick = Up;
	explorerTexts[2]->text.setString("Down");
	explorerTexts[2]->onClick = Down;
	explorerTexts[3]->text.setString("Only Dir");
	explorerTexts[3]->onClick = OnlyDir;
	explorerTexts[4]->text.setString("Only .fds");
	explorerTexts[4]->onClick = OnlyFDS;
	explorerTexts[5]->text.setString("Directory Name");
	explorerTexts[6]->text.setString("File 0");
	explorerTexts[6]->onClick = File0;
	explorerTexts[7]->text.setString("File 1");
	explorerTexts[7]->onClick = File1;
	explorerTexts[8]->text.setString("File 2");
	explorerTexts[8]->onClick = File2;
	explorerTexts[9]->text.setString("File 3");
	explorerTexts[9]->onClick = File3;
	explorerTexts[10]->text.setString("File 4");
	explorerTexts[10]->onClick = File4;
	explorerTexts[11]->text.setString("File 5");
	explorerTexts[11]->onClick = File5;
	for (int i = 0; i < 5; i++)
	{
		explorerTexts[i]->text.setFillColor(sf::Color(0, 0, 255));
		explorerTexts[i]->text.setPosition(70, 135 + (i * 40));
		explorerTexts[i]->text.setCharacterSize(22);
		explorerTexts[i]->MiddleAlignment();
	}
	explorerTexts[5]->text.setPosition(90, 30);
	for (int i = 0; i < 6; i++)
	{
		explorerTexts[i + 6]->text.setPosition(140, 105 + (i * 40));
	}

	for (int i = 0; i < explorerRectCount; i++)
	{
		explorerRects[i] = (RectangleShape*)guiManager->AddBack(1, 2);
		explorerRects[i]->enabled = false;
		explorerRects[i]->rectangle = sf::RectangleShape();
	}
	explorerRects[0]->depth = 1;
	explorerRects[0]->SetRectangle(10, 10, 780, 490, 180, 180, 180, 255, 120, 120, 120, 255, 6);
	explorerRects[1]->SetRectangle(18, 18, 50, 50, 255, 0, 0, 255, 0, 0, 0, 255, 5);
	explorerRects[2]->SetRectangle(84, 18, 688, 50, 150, 220, 220, 255, 0, 160, 220, 255, 5);

	confirmOpen = false;
	for (int i = 0; i < confirmTextCount; i++)
	{
		confirmTexts[i] = (Text*)guiManager->AddBack(0, 6);
		confirmTexts[i]->enabled = false;
		confirmTexts[i]->text = sf::Text("Empty", guiManager->fonts[0], 24);
		confirmTexts[i]->text.setFillColor(sf::Color(0, 0, 0));
	}
	confirmTexts[0]->text.setString("Do you want to save or load this file?");
	confirmTexts[1]->text.setString("File Name");
	confirmTexts[1]->text.setCharacterSize(28);
	confirmTexts[2]->text.setString("Load");
	confirmTexts[3]->text.setString("Save");
	confirmTexts[0]->MiddleAlignment();
	confirmTexts[0]->text.setPosition(400, 170);
	confirmTexts[1]->text.setPosition(400, 255);
	confirmTexts[2]->MiddleAlignment();
	confirmTexts[2]->text.setPosition(210, 352);
	confirmTexts[3]->MiddleAlignment();
	confirmTexts[3]->text.setPosition(590, 352);
	for (int i = 0; i < confirmRectCount; i++)
	{
		confirmRects[i] = (RectangleShape*)guiManager->AddBack(1, 5);
		confirmRects[i]->enabled = false;
		confirmRects[i]->rectangle = sf::RectangleShape();
	}
	confirmRects[0]->depth = 4;
	confirmRects[0]->SetRectangle(90, 160, 620, 260, 150, 200, 190, 255, 100, 110, 140, 255, 6);
	confirmRects[0]->onClick = CloseConfirm;
	confirmRects[1]->SetRectangle(150, 340, 120, 40, 150, 170, 200, 255, 60, 80, 120, 255, 5);
	confirmRects[1]->onClick = Load;
	confirmRects[2]->SetRectangle(530, 340, 120, 40, 150, 170, 200, 255, 60, 80, 120, 255, 5);
	confirmRects[2]->onClick = Save;
}

void FileExplorer::EnableUI()
{
	for (int i = 0; i < explorerTextCount; i++)
		explorerTexts[i]->enabled = true;
	for (int i = 0; i < explorerRectCount; i++)
		explorerRects[i]->enabled = true;
	confirmOpen = false;
	for (int i = 0; i < 14; i++)
		explorerTimers[i] = 1;
	hudTexts[8]->text.setString("Paused");
	directoryIndex = 0;
	directoryString = "C:\\Users\\Chris\\Desktop\\Game Design folder o' folders\\C++ Programs\\4DRenderAttempt\\4DRenderAttempt\\Assets\\Scenes";
	explorerSetting = 0;
	sceneManager->ReadDirectory(directoryString, explorerSetting);
}

void FileExplorer::UpdateFileExplorerUI(sf::RenderWindow& window)
{
	sf::Vector2f mousePosition = (sf::Vector2f)(sf::Mouse::getPosition() - window.getPosition()) + sf::Vector2f(-3, -21);
	mouseRectangle->rectangle.setPosition(mousePosition);
	if (!confirmOpen && explorerRects[1]->rectangle.getGlobalBounds().contains(mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		for (int i = 0; i < explorerTextCount; i++)
			explorerTexts[i]->enabled = false;
		for (int i = 0; i < explorerRectCount; i++)
			explorerRects[i]->enabled = false;
		mouseRectangle->enabled = false;
		firstMouse = true;
		window.setMouseCursorVisible(false);
		window.setMouseCursorGrabbed(true);
		cursorLock = true;
		state = 0;
	}
	for (int i = 0; i < 5; i++) //Explorer left side things
	{
		sf::Text& text = explorerTexts[i]->text;
		if (!confirmOpen && text.getGlobalBounds().contains(mousePosition))
		{
			text.setFillColor(sf::Color(0, 0, 255 * 0.67f));
			if (explorerTexts[i]->onClick != nullptr && explorerTimers[i] == 0 && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				explorerTexts[i]->onClick(this);
		}
		else
			text.setFillColor(sf::Color(0, 0, 255));
	}
	if (directoryString.size() > 40) //Directory thing at top
		explorerTexts[5]->text.setString(directoryString.substr(directoryString.size() - 40, 40));
	else
		explorerTexts[5]->text.setString(directoryString);
	for (int i = 0; i < glm::min(6, (int)sceneManager->directoryFiles.size() - directoryIndex); i++) //Selectable Files
	{
		sf::Text& text = explorerTexts[i + 6]->text;
		text.setString(sceneManager->directoryFiles[i + directoryIndex]);
		if (!confirmOpen && text.getGlobalBounds().contains(mousePosition))
		{
			text.setFillColor(sf::Color(80, 80, 80));
			if (explorerTexts[i + 6]->onClick != nullptr && explorerTimers[i + 5] == 0 && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				explorerTexts[i + 6]->onClick(this);
		}
		else
			text.setFillColor(sf::Color(0, 0, 0));
	}
	for (int i = 5; i >= glm::max(0, (int)sceneManager->directoryFiles.size() - directoryIndex);i--)
	{
		sf::Text& text = explorerTexts[i + 6]->text;
		text.setString("");
	}

	if (confirmOpen)
	{
		if (!confirmRects[0]->rectangle.getGlobalBounds().contains(mousePosition) && confirmRects[0]->onClick != nullptr && explorerTimers[11] == 0 && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			confirmRects[0]->onClick(this);
		}
		for (int i = 1; i < 3; i++)
		{
			sf::RectangleShape& rect = confirmRects[i]->rectangle;
			if (rect.getGlobalBounds().contains(mousePosition))
			{
				rect.setFillColor(sf::Color(90, 120, 170, 255));
				if (confirmRects[i]->onClick != nullptr && explorerTimers[11 + i] == 0 && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
					confirmRects[i]->onClick(this);
			}
			else
			{
				rect.setFillColor(sf::Color(150, 170, 200, 255));
			}
		}
	}
}

void FileExplorer::RenderFileExplorer(sf::RenderWindow& window, Shader& shader, unsigned int* bufferSizes, std::vector<unsigned int>& transTextureIDs, std::vector<unsigned int>& transTextureRunLengths, GUIManager& guiManager)
{
	window.clear(sf::Color((0.2f * 255), (0.3f * 255), (0.3f * 255), 255));
	glClear(GL_DEPTH_BUFFER_BIT);

	shader.use();
	glActiveTexture(GL_TEXTURE0);

	renderManager->ResetIndexes();
	camera.InvertedCopyToMatrixBuffer();
	renderManager->ModelGenerate(camera, position);
	renderManager->DynamicGenerate(camera, position);
	renderManager->HexaModelGenerate(camera, position);
	renderManager->CopyToBuffer(VBOs, bufferSizes);

	lightingManager->AmbientToShader();
	lightingManager->DirLightToShader(camera.GetInvertedTransform());
	lightingManager->TranferToBuffers(camera.GetInvertedTransform(), position);
	errorCode = glGetError();
	for (int i = 0; i < textureCount; i++)
	{
		glBindVertexArray(VAOs[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
		textures[i].BindTexture(0);
		glDrawArrays(GL_TRIANGLES, 0, bufferSizes[i]);
	}
	renderManager->TransCopyToBuffer(transTextureIDs, transTextureRunLengths, &transVBO);
	if (transTextureRunLengths[0] != 0)
	{
		glBindVertexArray(transVAO);
		glBindBuffer(GL_ARRAY_BUFFER, transVBO);
		int index = 0;
		for (int i = 0; i < transTextureIDs.size(); i++)
		{
			transTextures[transTextureIDs[i]].BindTexture(0);
			glDrawArrays(GL_TRIANGLES, index * 3, transTextureRunLengths[i] * 3);
			index += transTextureRunLengths[i];
		}
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
	UpdateFileExplorerUI(window);
	guiManager.Draw(&window);
	window.popGLStates();

	window.display();
}

void FileExplorer::ProcessFileExplorerInput(sf::RenderWindow& window)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		terminated = true;
		window.close();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
	{
		for (int i = 0; i < explorerTextCount; i++)
			explorerTexts[i]->enabled = false;
		for (int i = 0; i < explorerRectCount; i++)
			explorerRects[i]->enabled = false;
		for (int i = 0; i < confirmTextCount; i++)
			confirmTexts[i]->enabled = false;
		for (int i = 0; i < confirmRectCount; i++)
			confirmRects[i]->enabled = false;
		confirmOpen = false;
		mouseRectangle->enabled = false;
		firstMouse = true;
		window.setMouseCursorVisible(false);
		window.setMouseCursorGrabbed(true);
		cursorLock = true;
		state = 0;
	}
	if (cursorLockWait != 0)
	{
		cursorLockWait -= deltaTime;
		if (cursorLockWait < 0)
			cursorLockWait = 0;
	}
	for (int i = 0; i < 14; i++)
	{
		if (explorerTimers[i] != 0)
		{
			explorerTimers[i] -= deltaTime;
			if (explorerTimers[i] < 0)
				explorerTimers[i] = 0;
		}
	}
}

void Up(void* fileExplorer)
{
	FileExplorer& thisFE = *(FileExplorer*)fileExplorer;
	if (thisFE.directoryIndex > 0)
	{
		thisFE.directoryIndex--;
	}
	thisFE.explorerTimers[1] = 0.1f;
}
void Down(void* fileExplorer)
{
	FileExplorer& thisFE = *(FileExplorer*)fileExplorer;
	if (thisFE.directoryIndex < (int)thisFE.sceneManager->directoryFiles.size() - 6)
	{
		thisFE.directoryIndex++;
	}
	thisFE.explorerTimers[2] = 0.1f;
}
void Back(void* fileExplorer)
{
	FileExplorer& thisFE = *(FileExplorer*)fileExplorer;
	thisFE.explorerTimers[0] = 0.3f;
	for (int i = thisFE.directoryString.size() - 1; i >= 0; i--)
	{
		if (thisFE.directoryString[i] == '\\')
		{
			thisFE.directoryString = thisFE.directoryString.substr(0, i);
			thisFE.directoryIndex = 0;
			thisFE.sceneManager->ReadDirectory(thisFE.directoryString, thisFE.explorerSetting);
			return;
		}
	}
	thisFE.sceneManager->ReadDirectory(thisFE.directoryString, thisFE.explorerSetting);
}
void File0(void* fileExplorer)
{
	FileExplorer& thisFE = *(FileExplorer*)fileExplorer;
	thisFE.explorerTimers[5] = 1;
	std::ifstream stream((thisFE.directoryString + '\\' + thisFE.explorerTexts[6]->text.getString()).getData(), std::ios::in | std::ios::binary);
	if (stream.good())
	{
		char check[8];
		bool invalidCheck = false;
		stream.read(check, 8);
		for (int i = 0; i < 8; i++)
		{
			if (check[i] != "4DRENDER"[i])
				invalidCheck = true;
		}
		if (invalidCheck)
			return;
		thisFE.selectedFileString = thisFE.explorerTexts[6]->text.getString();
		stream.close();
		for (int i = 0; i < thisFE.confirmTextCount; i++)
			thisFE.confirmTexts[i]->enabled = true;
		for (int i = 0; i < thisFE.confirmRectCount; i++)
			thisFE.confirmRects[i]->enabled = true;
		thisFE.confirmTexts[1]->text.setString(thisFE.selectedFileString);
		thisFE.confirmTexts[1]->MiddleAlignment();
		thisFE.confirmOpen = true;
		for (int i = 11; i < 14; i++)
		{
			thisFE.explorerTimers[i] = 1;
		}
	}
	else
	{
		stream.close();
		thisFE.directoryString.append('\\' + thisFE.explorerTexts[6]->text.getString());
		thisFE.directoryIndex = 0;
		thisFE.sceneManager->ReadDirectory(thisFE.directoryString, thisFE.explorerSetting);
	}
}
void File1(void* fileExplorer)
{
	FileExplorer& thisFE = *(FileExplorer*)fileExplorer;
	thisFE.explorerTimers[6] = 1;
	std::ifstream stream((thisFE.directoryString + '\\' + thisFE.explorerTexts[7]->text.getString()).getData(), std::ios::in | std::ios::binary);
	if (stream.good())
	{
		char check[8];
		bool invalidCheck = false;
		stream.read(check, 8);
		for (int i = 0; i < 8; i++)
		{
			if (check[i] != "4DRENDER"[i])
				invalidCheck = true;
		}
		if (invalidCheck)
			return;
		thisFE.selectedFileString = thisFE.explorerTexts[7]->text.getString();
		stream.close();
		for (int i = 0; i < thisFE.confirmTextCount; i++)
			thisFE.confirmTexts[i]->enabled = true;
		for (int i = 0; i < thisFE.confirmRectCount; i++)
			thisFE.confirmRects[i]->enabled = true;
		thisFE.confirmTexts[1]->text.setString(thisFE.selectedFileString);
		thisFE.confirmTexts[1]->MiddleAlignment();
		thisFE.confirmOpen = true;
		for (int i = 11; i < 14; i++)
		{
			thisFE.explorerTimers[i] = 1;
		}
	}
	else
	{
		stream.close();
		thisFE.directoryString.append('\\' + thisFE.explorerTexts[7]->text.getString());
		thisFE.directoryIndex = 0;
		thisFE.sceneManager->ReadDirectory(thisFE.directoryString, thisFE.explorerSetting);
	}
}
void File2(void* fileExplorer)
{
	FileExplorer& thisFE = *(FileExplorer*)fileExplorer;
	thisFE.explorerTimers[7] = 1;
	std::ifstream stream((thisFE.directoryString + '\\' + thisFE.explorerTexts[8]->text.getString()).getData(), std::ios::in | std::ios::binary);
	if (stream.good())
	{
		char check[8];
		bool invalidCheck = false;
		stream.read(check, 8);
		for (int i = 0; i < 8; i++)
		{
			if (check[i] != "4DRENDER"[i])
				invalidCheck = true;
		}
		if (invalidCheck)
			return;
		thisFE.selectedFileString = thisFE.explorerTexts[8]->text.getString();
		stream.close();
		for (int i = 0; i < thisFE.confirmTextCount; i++)
			thisFE.confirmTexts[i]->enabled = true;
		for (int i = 0; i < thisFE.confirmRectCount; i++)
			thisFE.confirmRects[i]->enabled = true;
		thisFE.confirmTexts[1]->text.setString(thisFE.selectedFileString);
		thisFE.confirmTexts[1]->MiddleAlignment();
		thisFE.confirmOpen = true;
		for (int i = 11; i < 14; i++)
		{
			thisFE.explorerTimers[i] = 1;
		}
	}
	else
	{
		stream.close();
		thisFE.directoryString.append('\\' + thisFE.explorerTexts[8]->text.getString());
		thisFE.directoryIndex = 0;
		thisFE.sceneManager->ReadDirectory(thisFE.directoryString, thisFE.explorerSetting);
	}
}
void File3(void* fileExplorer)
{
	FileExplorer& thisFE = *(FileExplorer*)fileExplorer;
	thisFE.explorerTimers[8] = 1;
	std::ifstream stream((thisFE.directoryString + '\\' + thisFE.explorerTexts[9]->text.getString()).getData(), std::ios::in | std::ios::binary);
	if (stream.good())
	{
		char check[8];
		bool invalidCheck = false;
		stream.read(check, 8);
		for (int i = 0; i < 8; i++)
		{
			if (check[i] != "4DRENDER"[i])
				invalidCheck = true;
		}
		if (invalidCheck)
			return;
		thisFE.selectedFileString = thisFE.explorerTexts[9]->text.getString();
		stream.close();
		for (int i = 0; i < thisFE.confirmTextCount; i++)
			thisFE.confirmTexts[i]->enabled = true;
		for (int i = 0; i < thisFE.confirmRectCount; i++)
			thisFE.confirmRects[i]->enabled = true;
		thisFE.confirmTexts[1]->text.setString(thisFE.selectedFileString);
		thisFE.confirmTexts[1]->MiddleAlignment();
		thisFE.confirmOpen = true;
		for (int i = 11; i < 14; i++)
		{
			thisFE.explorerTimers[i] = 1;
		}
	}
	else
	{
		stream.close();
		thisFE.directoryString.append('\\' + thisFE.explorerTexts[9]->text.getString());
		thisFE.directoryIndex = 0;
		thisFE.sceneManager->ReadDirectory(thisFE.directoryString, thisFE.explorerSetting);
	}
}
void File4(void* fileExplorer)
{
	FileExplorer& thisFE = *(FileExplorer*)fileExplorer;
	thisFE.explorerTimers[9] = 1;
	std::ifstream stream((thisFE.directoryString + '\\' + thisFE.explorerTexts[10]->text.getString()).getData(), std::ios::in | std::ios::binary);
	if (stream.good())
	{
		char check[8];
		bool invalidCheck = false;
		stream.read(check, 8);
		for (int i = 0; i < 8; i++)
		{
			if (check[i] != "4DRENDER"[i])
				invalidCheck = true;
		}
		if (invalidCheck)
			return;
		thisFE.selectedFileString = thisFE.explorerTexts[10]->text.getString();
		stream.close();
		for (int i = 0; i < thisFE.confirmTextCount; i++)
			thisFE.confirmTexts[i]->enabled = true;
		for (int i = 0; i < thisFE.confirmRectCount; i++)
			thisFE.confirmRects[i]->enabled = true;
		thisFE.confirmTexts[1]->text.setString(thisFE.selectedFileString);
		thisFE.confirmTexts[1]->MiddleAlignment();
		thisFE.confirmOpen = true;
		for (int i = 11; i < 14; i++)
		{
			thisFE.explorerTimers[i] = 1;
		}
	}
	else
	{
		stream.close();
		thisFE.directoryString.append('\\' + thisFE.explorerTexts[10]->text.getString());
		thisFE.directoryIndex = 0;
		thisFE.sceneManager->ReadDirectory(thisFE.directoryString, thisFE.explorerSetting);
	}
}
void File5(void* fileExplorer)
{
	FileExplorer& thisFE = *(FileExplorer*)fileExplorer;
	thisFE.explorerTimers[10] = 1;
	std::ifstream stream((thisFE.directoryString + '\\' + thisFE.explorerTexts[11]->text.getString()).getData(), std::ios::in | std::ios::binary);
	if (stream.good())
	{
		char check[8];
		bool invalidCheck = false;
		stream.read(check, 8);
		for (int i = 0; i < 8; i++)
		{
			if (check[i] != "4DRENDER"[i])
				invalidCheck = true;
		}
		if (invalidCheck)
			return;
		thisFE.selectedFileString = thisFE.explorerTexts[11]->text.getString();
		stream.close();
		for (int i = 0; i < thisFE.confirmTextCount; i++)
			thisFE.confirmTexts[i]->enabled = true;
		for (int i = 0; i < thisFE.confirmRectCount; i++)
			thisFE.confirmRects[i]->enabled = true;
		thisFE.confirmTexts[1]->text.setString(thisFE.selectedFileString);
		thisFE.confirmTexts[1]->MiddleAlignment();
		thisFE.confirmOpen = true;
		for (int i = 11; i < 14; i++)
		{
			thisFE.explorerTimers[i] = 1;
		}
	}
	else
	{
		stream.close();
		thisFE.directoryString.append('\\' + thisFE.explorerTexts[11]->text.getString());
		thisFE.directoryIndex = 0;
		thisFE.sceneManager->ReadDirectory(thisFE.directoryString, thisFE.explorerSetting);
	}
}
void OnlyDir(void* fileExplorer)
{
	FileExplorer& thisFE = *(FileExplorer*)fileExplorer;
	thisFE.explorerTimers[3] = 0.5f;
	if (thisFE.explorerSetting != 1)
		thisFE.explorerSetting = 1;
	else
		thisFE.explorerSetting = 0;
	thisFE.sceneManager->ReadDirectory(thisFE.directoryString, thisFE.explorerSetting);
}
void OnlyFDS(void* fileExplorer)
{
	FileExplorer& thisFE = *(FileExplorer*)fileExplorer;
	thisFE.explorerTimers[4] = 0.5f;
	if (thisFE.explorerSetting != 2)
		thisFE.explorerSetting = 2;
	else
		thisFE.explorerSetting = 0;
	thisFE.sceneManager->ReadDirectory(thisFE.directoryString, thisFE.explorerSetting);
}
void CloseConfirm(void* fileExplorer)
{
	FileExplorer& thisFE = *(FileExplorer*)fileExplorer;
	for (int i = 0; i < thisFE.confirmTextCount; i++)
		thisFE.confirmTexts[i]->enabled = false;
	for (int i = 0; i < thisFE.confirmRectCount; i++)
		thisFE.confirmRects[i]->enabled = false;
	thisFE.confirmOpen = false;
	for (int i = 0; i < 11; i++)
		thisFE.explorerTimers[i] = 0.5f;
}
void Load(void* fileExplorer)
{
	FileExplorer& thisFE = *(FileExplorer*)fileExplorer;
	thisFE.explorerTimers[12] = 1;
	std::cout << "Loading\n";
	thisFE.sceneManager->LoadScene(*thisFE.renderManager, *thisFE.lightingManager, camera, position, std::string(thisFE.directoryString + '\\' + thisFE.selectedFileString));
}
void Save(void* fileExplorer)
{
	FileExplorer& thisFE = *(FileExplorer*)fileExplorer;
	thisFE.explorerTimers[13] = 1;
	std::cout << "Saving\n";
	thisFE.sceneManager->SaveScene(*thisFE.renderManager, *thisFE.lightingManager, camera, position, std::string(thisFE.directoryString + '\\' + thisFE.selectedFileString));

}
