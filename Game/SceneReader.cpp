#include "stdafx.h"
#include "SceneReader.h"

CSceneReader::CSceneReader() : myCurrentBinPath("NA")
{
}

CSceneReader::~CSceneReader()
{
	myStreamPtr = nullptr;
	for (auto& data : myLoadScreenData)
	{
		delete data;
		data = nullptr;
	}
	myLoadScreenData.clear();

	for (auto& data : myInGameData)
	{
		delete data;
		data = nullptr;
	}
	myInGameData.clear();
}

bool CSceneReader::OpenBin(const std::string& aBinFilePath)
{
	myCurrentBinPath = aBinFilePath;
	myStream.open(aBinFilePath, std::ios::binary);
	if (!myStream.is_open())
	{
		return false;
	}
    return true;
}

SInGameData& CSceneReader::ReadInGameData()
{
	for (unsigned int i = 0; i < myInGameData.size(); ++i)
	{
		if (myInGameData[i]->myBinPath == myCurrentBinPath)
		{
			myStream.close();
			myStreamPtr = nullptr;
			myCurrentBinPath = "NA";
			std::cout << "Reloading" << std::endl;
			return *myInGameData[i];
		}
	}

	std::string binaryData((std::istreambuf_iterator<char>(myStream)), std::istreambuf_iterator<char>());
	myStreamPtr = &binaryData[0];

	myInGameData.emplace_back(new SInGameData());
	myInGameData.back()->myBinPath = myCurrentBinPath;

	SCameraData cameraData = {};
	myStreamPtr += Read(cameraData);
	myInGameData.back()->myCamera = cameraData;

	SDirectionalLightData directionalLightData = {};
	myStreamPtr += Read(directionalLightData);
	myInGameData.back()->myDirectionalLight = directionalLightData;

	int pointLightDataCount = 0;
	myStreamPtr += Read(pointLightDataCount);
	assert(pointLightDataCount < 1000 && "Something went wrong when reading PointLightData");
	myInGameData.back()->myPointLightData.reserve(pointLightDataCount);
	for (int i = 0; i < pointLightDataCount; ++i)
	{
		SPointLightData pointLightData = {};
		myStreamPtr += Read(pointLightData);
		myInGameData.back()->myPointLightData.emplace_back(pointLightData);
	}

	int playerIsInScene = 0;
	myStreamPtr += Read(playerIsInScene);
	if (playerIsInScene == 1)
	{
		SPlayerData playerData = {};
		myStreamPtr += Read(playerData);
		myInGameData.back()->myPlayerData = playerData;
	}

	int eventDataCount = 0;
	myStreamPtr += Read(eventDataCount);
	assert(eventDataCount < 1000 && "Something went wrong when reading EventData");
	myInGameData.back()->myEventData.reserve(eventDataCount);
	for (int i = 0; i < eventDataCount; ++i) {
		SEventData evenData = {};
		myStreamPtr += Read(evenData);

		std::string eventString = "";
		eventString = ReadStringAuto();
		myInGameData.back()->myEventStringMap[evenData.myInstanceID] = std::string(eventString); //Neccesary to Copy here
		myInGameData.back()->myEventData.emplace_back(evenData);
	}

	int enemyDataCount = 0;
	myStreamPtr += Read(enemyDataCount);
	assert(enemyDataCount < 1000 && "Something went wrong when reading EnemyData");
	myInGameData.back()->myEnemyData.reserve(enemyDataCount);
	for (int i = 0; i < enemyDataCount; ++i) {
		SEnemyData enemyData = {};
		myStreamPtr += Read(enemyData);
		myInGameData.back()->myEnemyData.emplace_back(enemyData);
	}

	int destructibleDataCount = 0;
	myStreamPtr += Read(destructibleDataCount);
	assert(destructibleDataCount < 1000 && "Something went wrong when reading DestructibleData");
	myInGameData.back()->myDestructibleData.reserve(destructibleDataCount);
	for (int i = 0; i < destructibleDataCount; ++i)
	{
		SDestructibleData destructibleData = {};
		myStreamPtr += Read(destructibleData);
		myInGameData.back()->myDestructibleData.emplace_back(destructibleData);
	}

	int myGameObjectDataCount = 0;
	myStreamPtr += Read(myGameObjectDataCount);

	assert(myGameObjectDataCount < 100000 && "Something went wrong when reading GameObjectData");

	myInGameData.back()->myGameObjects.reserve(myGameObjectDataCount);
	for (int i = 0; i < myGameObjectDataCount; ++i)
	{
		SGameObjectData gameObjectData = { 0 };
		myStreamPtr += Read(gameObjectData);
		myInGameData.back()->myGameObjects.emplace_back(gameObjectData);
	}

	int myEnvironmentFXCount = 0;
	myStreamPtr += Read(myEnvironmentFXCount);
	for (int i = 0; i < myEnvironmentFXCount; ++i)
	{
		SEnvironmentFXData data = {};
		myStreamPtr += Read(data);
		
		std::string jsonName = "";
		jsonName = ReadStringAuto();
		
		myInGameData.back()->myEnvironmentFXs.emplace_back(data);
		myInGameData.back()->myEnvironmentFXStringMap[data.myInstanceID] = std::string(jsonName);
	}

	int myParticleFXCount = 0;
	myStreamPtr += Read(myParticleFXCount);
	for (int i = 0; i < myParticleFXCount; ++i)
	{
		SParticleFXData data = {};
		myStreamPtr += Read(data);

		myInGameData.back()->myParticleFXs.emplace_back(data);

		int myJsonNameCount = 0;
		myStreamPtr += Read(myJsonNameCount);

		for (int j = 0; j < myJsonNameCount; ++j)
		{	
			std::string jsonName = "";
			jsonName = ReadStringAuto();
			myInGameData.back()->myParticleFXStringMap[data.myInstanceID].emplace_back(std::string(jsonName));
		}
	}


	myStream.close();
	myStreamPtr = nullptr;
	myCurrentBinPath = "NA";

	return *myInGameData.back();
}

SLoadScreenData& CSceneReader::ReadLoadScreenData()
{
	for (unsigned int i = 0; i < myLoadScreenData.size(); ++i)
	{
		if (myLoadScreenData[i]->myBinPath == myCurrentBinPath)
		{
			myStream.close();
			myStreamPtr = nullptr;
			myCurrentBinPath = "NA";
			std::cout << "Reloading" << std::endl;
			return *myLoadScreenData[i];
		}
	}

	std::string binaryData((std::istreambuf_iterator<char>(myStream)), std::istreambuf_iterator<char>());
	myStreamPtr = &binaryData[0];

	myLoadScreenData.emplace_back(new SLoadScreenData());
	myLoadScreenData.back()->myBinPath = myCurrentBinPath;

	SCameraData cameraData = {};
	myStreamPtr += Read(cameraData);
	myLoadScreenData.back()->myCamera = cameraData;

	SDirectionalLightData directionalLightData = {};
	myStreamPtr += Read(directionalLightData);
	myLoadScreenData.back()->myDirectionalLight = directionalLightData;

	unsigned int pointLightcount = 0;
	myStreamPtr += Read(pointLightcount);

	unsigned int isPlayerInScene = 0;
	myStreamPtr += Read(isPlayerInScene);

	unsigned int eventDataCount = 0;
	myStreamPtr += Read(eventDataCount);

	unsigned int enemyDataCount = 0;
	myStreamPtr += Read(enemyDataCount);

	unsigned int gameObjectCount = 0;
	myStreamPtr += Read(gameObjectCount);
	
	SGameObjectData gameObjectData = {};
	myStreamPtr += Read(gameObjectData);
	myLoadScreenData.back()->myGameObject = gameObjectData;

	myStream.close();
	myStreamPtr = nullptr;
	myCurrentBinPath = "NA";

	return *myLoadScreenData.back();
}
