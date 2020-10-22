#pragma once
#include "VertexStructs.h"
#include <vector>
#include <map>
#include <string>

using namespace FBXLoader;
struct BoneInfo
{
	Matrix44f BoneOffset;
	Matrix44f FinalTransformation;

	BoneInfo()
	{

	}
};

// One model can contain multiple meshes
class CLoaderMesh
{
public:
	CLoaderMesh() { myShaderType = 0; myVerticies = nullptr; myVertexBufferSize = 0; myVertexCount = 0; myModel = nullptr; myIndexes.reserve(1); }
	~CLoaderMesh(){}
	std::vector<unsigned int> myIndexes;
	std::vector<CLoaderMesh*> myChildren;
	unsigned int myShaderType;
	unsigned int myVertexBufferSize;
	int myVertexCount;
	class CLoaderModel* myModel;
	char* myVerticies;
};

class CLoaderModel
{
public:
	CLoaderModel() { myIsLoaded = false; myAnimationDuration = 0.0f; myNumBones = 0; myScene = nullptr; }
	~CLoaderModel(){}
	


public:
	std::vector<CLoaderMesh*>	myMeshes;
	std::string					myModelPath;
	float						myAnimationDuration;
	const struct aiScene*		myScene;
	Matrix44f					myGlobalInverseTransform;
	bool						myIsLoaded;
	std::vector<std::string>	myTextures;
	// Animation data
	std::vector<BoneInfo>				myBoneInfo;
	std::map<std::string, unsigned int> myBoneNameToIndex;
	unsigned int						myNumBones;

public:
	void SetData(const char* aModelPath){ myModelPath = aModelPath; }
	// Will only resize if myMeshes is empty
	void SetNumberOfMeshes(const unsigned int& aNumber) 
	{ 
		if (myMeshes.empty()) 
		{ 
			myMeshes.resize(aNumber);
			//myMeshes.reserve(aNumber);
			CLoaderMesh* modelBlock = new CLoaderMesh[aNumber];
			//memmove(&myMeshes, &modelBlock, aNumber);
			for (unsigned int i = 0; i < aNumber; ++i)
			{
				myMeshes[i] = /*&modelBlock[i];*/ std::move(&modelBlock[i]);
			}
			modelBlock = nullptr;
		} 
	}
	CLoaderMesh* CreateMesh()
	{ 
		CLoaderMesh *model = new CLoaderMesh(); 
		myMeshes.emplace_back(model); 
		model->myModel = this; return model; 
	}
	CLoaderMesh* GetMesh(const unsigned int& anIndex)
	{
		CLoaderMesh* mesh = myMeshes[anIndex]; 
		mesh->myModel = this; 
		return mesh; 
	}
};


class CFBXLoaderCustom
{
public:
	CFBXLoaderCustom();
	~CFBXLoaderCustom();
	CLoaderModel *LoadModel(const char* aModel);

private:
	void* LoadModelInternal(CLoaderModel* someInput);
	int DetermineAndLoadVerticies(struct aiMesh* aMesh, CLoaderMesh* aLoaderMesh);
	void LoadMaterials(const struct aiScene *sc, CLoaderModel* aModel);
	void LoadTexture(int aType, std::vector<std::string>& someTextures, struct aiMaterial* aMaterial);

};

