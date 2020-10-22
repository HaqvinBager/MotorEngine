#pragma once
#include <map>
#include <string>

class CEngine;
class CModel;

class CModelFactory
{
	friend class CEngine;
public:
	bool Init(CEngine& engine);

	CModel* LoadModel(std::string aFilePath);
	CModel* GetModel(std::string aFilePath);

	CModel* GetOutlineModelSubset();

	CModel* GetCube();

	static CModelFactory* GetInstance();

private:
	CModelFactory();
	~CModelFactory();
private:
	static CModelFactory* ourInstance;
	std::map<std::string, CModel*> myModelMap;
	CModel* myOutlineModelSubset;
	CEngine* myEngine;
};

