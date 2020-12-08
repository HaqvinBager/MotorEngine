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

	std::string ReadStringAuto()
	{
		int length = 0;
		myStreamPtr += Read(length);

		std::string text = "";
		myStreamPtr += ReadString(text, length);

		return text;
	}

	size_t ReadString(std::string& data, size_t aLength)
	{
		data.reserve(aLength + 1);
		memcpy(&data.data()[0], myStreamPtr, sizeof(char) * aLength);
		data.data()[aLength] = '\0';
		return sizeof(char) * aLength;
	}

	std::string myCurrentBinPath;
	char* myStreamPtr;
	std::ifstream myStream;

	std::vector<SLoadScreenData*> myLoadScreenData;
	std::vector<SInGameData*> myInGameData;

};