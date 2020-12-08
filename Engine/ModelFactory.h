#pragma once
#include <string>
#include <map>

class CEngine;
class CModel;

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

	CModel* GetInstancedModel(std::string aFilePath, int aNumberOfInstanced);

private:
	struct SInstancedModel
	{	
		const std::string myModelType;
		const int myCount;
		size_t myModelTypeHashCode;

		SInstancedModel(const std::string aModelType, const int aCount)
			: myModelType(aModelType)
			, myCount(aCount)
		{
			myModelTypeHashCode = std::hash<std::string>()(aModelType);
		}

		bool operator< (const SInstancedModel& lhs) const
		{
			return std::tie(myCount, myModelTypeHashCode) < std::tie(lhs.myCount, lhs.myModelTypeHashCode);
		}
	};

private:
	CModel* CreateInstancedModels(std::string aFilePath, int aNumberOfInstanced);
	CModelFactory();
	~CModelFactory();
	//wchar_t* TexturePathWide(std::string aTexturePath) const;
	ID3D11ShaderResourceView* GetShaderResourceView(ID3D11Device* aDevice, std::string aTexturePath);

	std::map<std::string, CModel*> myModelMapPBR;
	std::map<SInstancedModel, CModel*> myInstancedModelMapPBR;
	std::map<std::string, CModel*> myModelMap;
	CEngine* myEngine;
	CModel* myOutlineModelSubset;
	static CModelFactory* ourInstance;
};