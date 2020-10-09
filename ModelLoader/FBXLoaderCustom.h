#pragma once
#include "VertexStructs.h"
#include <vector>
#include <map>
#include <string>
#include <xmmintrin.h>

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
	CLoaderMesh() {myShaderType = 0; myVerticies = nullptr; myVertexBufferSize = 0; myVertexCount = 0; myModel = nullptr; }
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
	CLoaderModel() : myNumBones(), myScene(nullptr) { myIsLoaded = false; myAnimationDuration = 0.0f; }
	~CLoaderModel(){}
	void SetData(const char* aModelPath){ myModelPath = aModelPath; }
	
	CLoaderMesh* CreateMesh() 
	{
		CLoaderMesh *model = new CLoaderMesh();
		myMeshes.emplace_back(model);
		model->myModel = this;
		return model;
	}

	std::vector<CLoaderMesh*> myMeshes;
	std::string myModelPath;
	float myAnimationDuration;
	const struct aiScene* myScene;
	Matrix44f myGlobalInverseTransform;
	bool myIsLoaded;
	std::vector<std::string> myTextures;
	// Animation data
	std::vector<BoneInfo> myBoneInfo;
	std::map<std::string, unsigned int> myBoneNameToIndex;
	unsigned int myNumBones;


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

