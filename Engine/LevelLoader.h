#pragma once
#include <string>
#include <vector>

class CUnityLoader;
class CScene;
class ObjectData;

class CLevelLoader
{
public:
	CLevelLoader();
	~CLevelLoader() = default;

	bool Init(/*const unsigned int alevelIndex = 0*/);

	void CreateLevel(const std::string& aPath);

	void LoadNewLevel(/*const std::string& aPath*/);

private:
	CUnityLoader* myUnityLoader;
	//CScene* myScene;
	std::vector<ObjectData> objectData;
	std::vector<CScene*> myScenes;
};

