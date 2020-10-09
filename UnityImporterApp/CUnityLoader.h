#pragma once
#include <string>
#include <vector>
#include <map>

class ObjectDataRaw
{
public:
	std::string myName;

	float myRotX;
	float myRotY;
	float myRotZ;
	float myPosX;
	float myPosY;
	float myPosZ;

	float myScaleX;
	float myScaleY;
	float myScaleZ;

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
	void LoadModels(std::string aModelPathFile);
	std::vector<ObjectData> LoadGameObjects(const std::string& aGameObjectFile, EReadMode aReadMode = EReadMode_Binary);
private:
	std::vector<ObjectData> LoadGameObjectsBinary(const std::string& aGameObjectFile);
	std::vector<ObjectData> LoadGameObjectsASCII(const std::string& aGameObjectFile);

	std::map<int, std::string> myModelPaths;
	bool myModelPathsFetched = false;
};

