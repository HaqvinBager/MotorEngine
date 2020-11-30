#pragma once
#include <fstream>
#include "SceneData.h"

enum class EBin
{
	InGameScene,
	LoadScene,
};

class CSceneReader
{
public:
	CSceneReader();
	~CSceneReader();

	bool OpenBin(const std::string& aBinFilePath);

	SInGameData& ReadInGameData();
	SLoadScreenData& ReadLoadScreenData();

private:

	template<typename T>
	size_t Read(T& data)
	{
		memcpy(&data, myStreamPtr, sizeof(T));
		return sizeof(T);
	}

	char* myStreamPtr;
	std::ifstream myStream;

	std::vector<SLoadScreenData*> myLoadScreenData;
	std::vector<SInGameData*> myInGameData;

};