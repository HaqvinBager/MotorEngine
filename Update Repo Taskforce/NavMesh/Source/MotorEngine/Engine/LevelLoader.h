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

	bool Init();

	void CreateLevel(const std::string& aPath);

	void LoadNewLevel(const std::string& aPath);

private:
	CUnityLoader* myUnityLoader;
	CScene* myScene;
	std::vector<ObjectData> objectData;
};

