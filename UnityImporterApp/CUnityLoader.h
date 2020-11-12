#pragma once
#include <string>
#include <vector>
#include <map>

class EnviromentDataRaw {
public:
	float myDirectionX;
	float myDirectionY;
	float myDirectionZ;
	float myColorR;
	float myColorG;
	float myColorB;
	float myIntensity;
};

class CameraDataRaw {
public:
	float myRotX;
	float myRotY;
	float myRotZ;
	float myPosX;
	float myPosY;
	float myPosZ;
	float myFieldOfView;
};

class AssetDataRaw {
public:
	int instanceID;
	float myRotX;
	float myRotY;
	float myRotZ;
	float myPosX;
	float myPosY;
	float myPosZ;
	float myScaleX;
	float myScaleY;
	float myScaleZ;
	int myModelIndex;
};

class ObjectDataRaw
{
public:
	//std::string myName; //funkar inte i binary!!
	int instanceID;
	float myRotX;
	float myRotY;
	float myRotZ;
	float myPosX;
	float myPosY;
	float myPosZ;
	float myScaleX;
	float myScaleY;
	float myScaleZ;
	int myModelIndex;

	//float myColliderRadius;
	//float myColliderHeight;

	//float myIntensity;
	//float myR;
	//float myG;
	//float myB;
	//float myA;
	//
	//float myColliderSizeX;
	//float myColliderSizeY;
	//float myColliderSizeZ;
};

class ObjectDataBin : public ObjectDataRaw
{
public:
	int myModelIndex;
};
class ObjectData : public ObjectDataRaw
{
public:
	std::string myRelativePath;
};

class LevelData {
public:
	CameraDataRaw myCameraData;
	EnviromentDataRaw myEnviromentData;
	std::vector<AssetDataRaw> myModelData;
	std::map<int, std::string> myModelPaths;
};

enum EReadMode
{
	EReadMode_ASCII,
	EReadMode_Binary,
	EReadMode_MAX
};

class CUnityLoader
{
public:
	CUnityLoader();
	std::vector<ObjectData> LoadGameObjects(const std::string& aGameObjectFile, EReadMode aReadMode = EReadMode_Binary);
public:
	LevelData* LoadLevelBinary(const std::string& aGameObjectFile);
private:
	std::map<int, std::string> LoadModels(std::string aModelPathFile);
	std::vector<ObjectData> LoadGameObjectsBinary(const std::string& aGameObjectFile);
	std::vector<ObjectData> LoadGameObjectsASCII(const std::string& aGameObjectFile);

	bool myModelPathsFetched = false;
};

