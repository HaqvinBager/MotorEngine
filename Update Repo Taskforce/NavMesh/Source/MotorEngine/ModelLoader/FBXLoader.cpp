#include "FBXLoader.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "..\GraphicsEngine.h"

#include <map>
#include <string>
#include <windows.h>
#include <fstream>
#include "..\GraphicsEngine.h"

#include <memory.h>

//#error "Remove these libraryincludes and include them in your projectproperties instead"
//#ifdef _DEBUG
//#pragma comment (lib, "assimp-vc140-mtd.lib")
//#else
//#pragma comment (lib, "assimp-vc140-mt.lib")
//#endif

#define TEXTURE_SET_0 0

CFBXLoader::CFBXLoader()
{
}


CFBXLoader::~CFBXLoader() 
{
}

Matrix44f ConvertToEngineMatrix33(const aiMatrix3x3& AssimpMatrix)
{

	Matrix44f mat;
	mat.myMatrix[0][0] = AssimpMatrix.a1;	mat.myMatrix[0][1] = AssimpMatrix.a2;	mat.myMatrix[0][2] = AssimpMatrix.a3;	mat.myMatrix[0][3] = 0.0f;
	mat.myMatrix[1][0] = AssimpMatrix.b1;	mat.myMatrix[1][1] = AssimpMatrix.b2;	mat.myMatrix[1][2] = AssimpMatrix.b3;	mat.myMatrix[1][3] = 0.0f;
	mat.myMatrix[2][0] = AssimpMatrix.c1;	mat.myMatrix[2][1] = AssimpMatrix.c2;	mat.myMatrix[2][2] = AssimpMatrix.c3;	mat.myMatrix[2][3] = 0.0f;
	mat.myMatrix[3][0] = 0.0f;				mat.myMatrix[3][1] = 0.0f;				mat.myMatrix[3][2] = 0.0f;				mat.myMatrix[3][3] = 1.0f;
	return mat;
}

// constructor from Assimp matrix
Matrix44f ConvertToEngineMatrix44(const aiMatrix4x4& AssimpMatrix)
{
	Matrix44f mat;
	mat.myMatrix[0][0] = AssimpMatrix.a1; mat.myMatrix[0][1] = AssimpMatrix.a2; mat.myMatrix[0][2] = AssimpMatrix.a3; mat.myMatrix[0][3] = AssimpMatrix.a4;
	mat.myMatrix[1][0] = AssimpMatrix.b1; mat.myMatrix[1][1] = AssimpMatrix.b2; mat.myMatrix[1][2] = AssimpMatrix.b3; mat.myMatrix[1][3] = AssimpMatrix.b4;
	mat.myMatrix[2][0] = AssimpMatrix.c1; mat.myMatrix[2][1] = AssimpMatrix.c2; mat.myMatrix[2][2] = AssimpMatrix.c3; mat.myMatrix[2][3] = AssimpMatrix.c4;
	mat.myMatrix[3][0] = AssimpMatrix.d1; mat.myMatrix[3][1] = AssimpMatrix.d2; mat.myMatrix[3][2] = AssimpMatrix.d3; mat.myMatrix[3][3] = AssimpMatrix.d4;
	return mat;
}

int CFBXLoader::DetermineAndLoadVerticies(aiMesh* fbxMesh, CLoaderMesh* aLoaderMesh)
{
	unsigned int modelBluePrintType = 0;

	modelBluePrintType |= (fbxMesh->HasPositions() ? EModelBluePrint_Position : 0);
	modelBluePrintType |= (fbxMesh->HasNormals() ? EModelBluePrint_Normal : 0);
	modelBluePrintType |= (fbxMesh->HasTangentsAndBitangents() ? EModelBluePrint_BinormTan : 0);
	modelBluePrintType |= (fbxMesh->HasTextureCoords(0) ? EModelBluePrint_UV : 0);
	modelBluePrintType |= (fbxMesh->HasBones() ? EModelBluePrint_Bones : 0);

	int vertexSize = 0;
	vertexSize += (fbxMesh->HasPositions() ? sizeof(float) * 4 : 0);
	vertexSize += (fbxMesh->HasNormals() ? sizeof(float) * 4 : 0);
	vertexSize += (fbxMesh->HasTangentsAndBitangents() ? sizeof(float) * 8 : 0);
	vertexSize += (fbxMesh->HasTextureCoords(0) ? sizeof(float) * 2 : 0);
	vertexSize += (fbxMesh->HasBones() ? sizeof(float) * 8 : 0); // Better with an UINT, but this works

	aLoaderMesh->myShaderType = modelBluePrintType;
	aLoaderMesh->myVertexSize = vertexSize;

	aLoaderMesh->myVertices = new char[vertexSize * fbxMesh->mNumVertices];
	aLoaderMesh->myVertexCount = fbxMesh->mNumVertices;

	std::vector<VertexBoneData> collectedBoneData;
	if (fbxMesh->HasBones())
	{
		collectedBoneData.resize(fbxMesh->mNumVertices);

		unsigned int BoneIndex = 0;
		for (unsigned int i = 0; i < fbxMesh->mNumBones; i++) 
		{
			std::string BoneName(fbxMesh->mBones[i]->mName.data);
			if (aLoaderMesh->myModel->myBoneNameToIndex.find(BoneName) == aLoaderMesh->myModel->myBoneNameToIndex.end())
			{
				BoneIndex = aLoaderMesh->myModel->myNumBones;
				aLoaderMesh->myModel->myNumBones++;
				BoneInfo bi;
				aLoaderMesh->myModel->myBoneInfo.push_back(bi);


				Matrix44f NodeTransformation = ConvertToEngineMatrix44(fbxMesh->mBones[i]->mOffsetMatrix);

				aLoaderMesh->myModel->myBoneInfo[BoneIndex].BoneOffset = NodeTransformation;
				aLoaderMesh->myModel->myBoneNameToIndex[BoneName] = BoneIndex;
			}
			else {
				BoneIndex = aLoaderMesh->myModel->myBoneNameToIndex[BoneName];
			}

			for (unsigned int j = 0; j < fbxMesh->mBones[i]->mNumWeights; j++) 
			{
				unsigned int VertexID = fbxMesh->mBones[i]->mWeights[j].mVertexId;
				float Weight = fbxMesh->mBones[i]->mWeights[j].mWeight;
				collectedBoneData[VertexID].AddBoneData(BoneIndex, Weight);
			}
		}
	}
	

	SVertexCollection vertexCollection;
	for (unsigned int i = 0; i < fbxMesh->mNumVertices; i++)
	{
		if (fbxMesh->HasPositions())
		{
			aiVector3D& mVertice = fbxMesh->mVertices[i];
			vertexCollection.PushVec4(Vector4f(mVertice.x, mVertice.y, mVertice.z, 1));
		}
		if (fbxMesh->HasNormals())
		{
			aiVector3D& mNorm = fbxMesh->mNormals[i];
			vertexCollection.PushVec4(Vector4f(mNorm.x, mNorm.y, mNorm.z, 1));
		}
		if (fbxMesh->HasTangentsAndBitangents())
		{
			aiVector3D& mTangent = fbxMesh->mTangents[i];
			aiVector3D& biTangent = fbxMesh->mBitangents[i];

			vertexCollection.PushVec4(Vector4f(mTangent.x, mTangent.y, mTangent.z, 1));
			vertexCollection.PushVec4(Vector4f(biTangent.x, biTangent.y, biTangent.z, 1));
		}
		if (fbxMesh->HasTextureCoords(TEXTURE_SET_0))		//HasTextureCoords(texture_coordinates_set)
		{
			vertexCollection.PushVec2(Vector2f(fbxMesh->mTextureCoords[TEXTURE_SET_0][i].x, fbxMesh->mTextureCoords[TEXTURE_SET_0][i].y));
		}
		if (fbxMesh->HasBones())
		{
			VertexBoneData& boneData = collectedBoneData[i];

			// UINTS woudl be better
			aiVector3D bones;
			vertexCollection.PushVec4(Vector4f((float)boneData.IDs[0], (float)boneData.IDs[1], (float)boneData.IDs[2], (float)boneData.IDs[3]));

			aiVector3D weights;
			vertexCollection.PushVec4(Vector4f(boneData.Weights[0], boneData.Weights[1], boneData.Weights[2], boneData.Weights[3]));
		}
	}

	memcpy(aLoaderMesh->myVertices, &vertexCollection.myData[0], vertexSize * fbxMesh->mNumVertices);

	return vertexSize;
}

CLoaderModel* CFBXLoader::LoadModel(const char* aModel)
{
	CLoaderModel* newModel = new CLoaderModel();
	newModel->SetData(aModel);

	if (!LoadModelInternal(newModel))
	{
		delete newModel;
		newModel = nullptr;
	}

	return newModel;
}

bool does_file_exist(const char *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void* CFBXLoader::LoadModelInternal(CLoaderModel* someInput)
{
	CLoaderModel* model = someInput;
	const struct aiScene* scene = NULL;

	if (!does_file_exist(model->myModelPath.c_str()))
	{
		OutputDebugStringA("File not found");
		return nullptr;
	}
	scene = aiImportFile(model->myModelPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);

	OutputDebugStringA(model->myModelPath.c_str());
	
	if (!scene)
	{
		OutputDebugStringA(aiGetErrorString());
		return nullptr;
	}

	model->myScene = scene;
	
	for (unsigned int n = 0; n < scene->mNumMeshes; ++n)
	{
		CLoaderMesh* mesh = model->CreateMesh();

		aiMesh* fbxMesh = scene->mMeshes[n];

		DetermineAndLoadVerticies(fbxMesh, mesh);

		for (unsigned int i = 0; i < fbxMesh->mNumFaces; i++)
		{
			for (uint j = 0; j < fbxMesh->mFaces[i].mNumIndices; j++)
			{
				mesh->myIndices.push_back(fbxMesh->mFaces[i].mIndices[j]);
			}
		}
	}

	// CHange to support multiple animations
	if (scene->mNumAnimations > 0)
	{
		model->myAnimationDuration = (float)scene->mAnimations[0]->mDuration;
	}
	
	LoadMaterials(scene, model);

	model->myGlobalInverseTransform = ConvertToEngineMatrix44(scene->mRootNode->mTransformation);

	return model;

}


void CFBXLoader::LoadMaterials(const struct aiScene *sc, CLoaderModel* aModel)
{

	for (unsigned int m = 0; m < sc->mNumMaterials; m++)
	{
		LoadTexture(aiTextureType_DIFFUSE, aModel->myTextures, sc->mMaterials[m]); // TEXTURE_DEFINITION_ALBEDO
		LoadTexture(aiTextureType_SPECULAR, aModel->myTextures, sc->mMaterials[m]); // TEXTURE_DEFINITION_ROUGHNESS
		LoadTexture(aiTextureType_AMBIENT, aModel->myTextures, sc->mMaterials[m]); // TEXTURE_DEFINITION_AMBIENTOCCLUSION
		LoadTexture(aiTextureType_EMISSIVE, aModel->myTextures, sc->mMaterials[m]); // TEXTURE_DEFINITION_EMISSIVE
		LoadTexture(aiTextureType_HEIGHT, aModel->myTextures, sc->mMaterials[m]);
		LoadTexture(aiTextureType_NORMALS, aModel->myTextures, sc->mMaterials[m]); // TEXTURE_DEFINITION_NORMAL
		LoadTexture(aiTextureType_SHININESS, aModel->myTextures, sc->mMaterials[m]);
		LoadTexture(aiTextureType_OPACITY, aModel->myTextures, sc->mMaterials[m]);
		LoadTexture(aiTextureType_DISPLACEMENT, aModel->myTextures, sc->mMaterials[m]);
		LoadTexture(aiTextureType_LIGHTMAP, aModel->myTextures, sc->mMaterials[m]);
		LoadTexture(aiTextureType_REFLECTION, aModel->myTextures, sc->mMaterials[m]); // TEXTURE_DEFINITION_METALNESS
	}
}

void CFBXLoader::LoadTexture(int aType, std::vector<std::string>& someTextures, aiMaterial* aMaterial)
{
	int texIndex = 0;
	aiReturn texFound = AI_SUCCESS;

	aiString path;	// filename

	texFound = aMaterial->GetTexture((aiTextureType)aType, texIndex, &path);
	if (texFound == AI_FAILURE)
	{
		someTextures.push_back("");
		return;
	}

	std::string filePath = std::string(path.data);

	const size_t last_slash_idx = filePath.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		filePath.erase(0, last_slash_idx + 1);
	}

	someTextures.push_back(filePath);
}