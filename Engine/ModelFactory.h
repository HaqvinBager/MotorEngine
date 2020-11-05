#pragma once
#include <string>
#include <map>


class CModel;
class CEngine;
class CModelInstance;

struct ID3D11Device;

class CModelFactory {
	friend class CEngine;
public:
	static CModelFactory* GetInstance();
	bool Init(CEngine& engine);
	CModel* GetModel(std::string aFilePath);
	CModel* GetModelPBR(std::string aFilePath);

	CModel* LoadModelPBR(std::string aFilePath);
	CModel* LoadModel(std::string aFilePath);
	CModel* GetCube();

	CModel* GetOutlineModelSubset();

private:
	CModelFactory();
	~CModelFactory();
	//wchar_t* TexturePathWide(std::string aTexturePath) const;
	ID3D11ShaderResourceView* GetShaderResourceView(ID3D11Device* aDevice, std::string aTexturePath);

	std::map<std::string, CModel*> myModelMapPBR;
	std::map<std::string, CModel*> myModelMap;
	CEngine* myEngine;
	CModel* myOutlineModelSubset;
	static CModelFactory* ourInstance;
};