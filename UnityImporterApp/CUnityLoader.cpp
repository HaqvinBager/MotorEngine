#include "CUnityLoader.h"
#include <functional>
#include <fstream>
#include <streambuf>
#include <algorithm>

#include "rapidjson/document.h"
#include <array>
using namespace rapidjson;

std::function<std::vector<ObjectData>(const std::string&)> myLoaderFunctions[EReadMode_MAX];
CUnityLoader::CUnityLoader()
{
	myLoaderFunctions[EReadMode_ASCII] = std::bind(&CUnityLoader::LoadGameObjectsASCII, this, std::placeholders::_1);
	myLoaderFunctions[EReadMode_Binary] = std::bind(&CUnityLoader::LoadGameObjectsBinary, this, std::placeholders::_1);


}
void CUnityLoader::LoadModels(std::string aModelPathFile)
{
	std::ifstream modelFile(aModelPathFile);
	std::string s;
	int index = -1;
	while (std::getline(modelFile, s))
	{
		index++;
		std::replace(s.begin(), s.end(), '/', '\\');
		myModelPaths[index] = s;
	}

	myModelPathsFetched = true;

}

//Returns a copy
std::vector<ObjectData> CUnityLoader::LoadGameObjects(const std::string& aGameObjectFile, EReadMode aReadMode)
{
	assert(aReadMode < EReadMode_MAX || aReadMode > -1);
	assert(myModelPathsFetched);
	//return LoadGameObjectsASCII(aGameObjectFile);
	return myLoaderFunctions[static_cast<size_t>(aReadMode)](aGameObjectFile);
}

// Much faster than ASCII load, but unreadable files, better with ASCII för debugging
std::vector<ObjectData> CUnityLoader::LoadGameObjectsBinary(const std::string& aGameObjectFile)
{
	std::ifstream t(aGameObjectFile, std::ios::binary);
	assert(t.is_open());

	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	unsigned int count = 0;
	memcpy(&count, &str[0], sizeof(int));

	assert(count != 0);
	char* ptr = &str[0];
	ptr += sizeof(int);

	ObjectDataBin* data = new ObjectDataBin[count];
	memcpy(data, ptr, sizeof(ObjectDataBin) * count);

	std::vector<ObjectData> returnedData;
	for (size_t i=0; i< count; i++)
	{
		returnedData.push_back(ObjectData());
		ObjectData& object = returnedData.back();

		memcpy(&object, &data[i], sizeof(ObjectDataRaw)); // Memcpy all the essential data
		object.myRelativePath = myModelPaths[data[i].myModelIndex];
	}
	delete[] data;
	t.close();
	return returnedData;
}

// Use only for debugging, terribly slow, stringcomparison hell
std::vector<ObjectData> CUnityLoader::LoadGameObjectsASCII(const std::string& aGameObjectFile)
{
	std::ifstream t(aGameObjectFile);
	assert(t.is_open());
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	Document document;
	document.Parse(str.c_str());
	
	Value::ConstMemberIterator root = document.MemberBegin();
	Value::ConstMemberIterator oneStepDown = document.FindMember("dataList");

	rapidjson::Value& results = document["dataList"];

	std::vector<ObjectData> loadedObjects;
	for (rapidjson::SizeType i = 0; i < results.Size(); i++)
	{
		ObjectData object;
		object.myRotX = results[i]["myRotX"].GetDouble();
		object.myRotY = results[i]["myRotY"].GetDouble();
		object.myRotZ = results[i]["myRotZ"].GetDouble();

		object.myPosX = results[i]["myPosX"].GetDouble();
		object.myPosY = results[i]["myPosY"].GetDouble();
		object.myPosZ = results[i]["myPosZ"].GetDouble();

		object.myScaleX = results[i]["myScaleX"].GetDouble();
		object.myScaleY = results[i]["myScaleY"].GetDouble();
		object.myScaleZ = results[i]["myScaleZ"].GetDouble();

		//object.myIntensity = results[i]["myIntensity"].GetDouble();
		//object.myR = results[i]["myR"].GetDouble();
		//object.myG = results[i]["myG"].GetDouble();
		//object.myB = results[i]["myB"].GetDouble();
		//object.myA = results[i]["myA"].GetDouble();

		object.myRelativePath = std::string(results[i]["myPath"].GetString());


		std::replace(object.myRelativePath.begin(), object.myRelativePath.end(), '/', '\\');

		loadedObjects.emplace_back(object);
	}


	return loadedObjects;
}

