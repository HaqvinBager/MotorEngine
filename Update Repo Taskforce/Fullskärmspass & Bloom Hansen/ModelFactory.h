#pragma once
#include <map>
#include <string>
#include "ModelInstance.h"
#include "Model.h"

class CModelFactory {
public:
	static CModelFactory* GetInstance();

	CModelFactory();
	~CModelFactory();
	void Init(ID3D11Device* aDevice);
	CModel* LoadModel(std::string aFilePath);
	CModel* GetCube();
	CModel* GetModel(std::string aFilePath);

private:
	ID3D11Device* myDevice;
	std::map<std::string, CModel*> myModelMap;
};

