#pragma once
#include <ProgramGlobals.h>

class FileExplorer
{
public:
	static const int explorerRectCount = 3;
	static const int explorerTextCount = 12;
	RectangleShape* explorerRects[explorerRectCount];
	Text* explorerTexts[explorerTextCount];
	float explorerTimers[14];
	static const int confirmRectCount = 3;
	static const int confirmTextCount = 4;
	RectangleShape* confirmRects[confirmRectCount];
	Text* confirmTexts[confirmTextCount];
	bool confirmOpen;
	int directoryIndex = 0;
	std::string directoryString = "C:\\Users\\Chris\\Desktop\\Game Design folder o' folders\\C++ Programs\\4DRenderAttempt\\4DRenderAttempt\\Assets\\Scenes";
	std::string selectedFileString;
	int explorerSetting;

	RenderManager* renderManager;
	LightingManager* lightingManager;
	SceneManager* sceneManager;
	GUIManager* guiManager;

	FileExplorer(RenderManager*, LightingManager*, SceneManager*, GUIManager*);

	void SetupUI();
	void EnableUI();

	void UpdateFileExplorerUI(sf::RenderWindow&);
	void RenderFileExplorer(sf::RenderWindow&, Shader&, unsigned int*, std::vector<unsigned int>&, std::vector<unsigned int>&, GUIManager&);
	void ProcessFileExplorerInput(sf::RenderWindow&);
};
void Up(void*);
void Down(void*);
void Back(void*);
void File0(void*);
void File1(void*);
void File2(void*);
void File3(void*);
void File4(void*);
void File5(void*);
void OnlyDir(void*);
void OnlyFDS(void*);
void CloseConfirm(void*);
void Load(void*);
void Save(void*);
