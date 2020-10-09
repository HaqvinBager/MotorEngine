#include "FBXLoaderCustom.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <map>
#include <string>
#include <windows.h>
#include <fstream>
#include <chrono>



#include <memory.h>


// Change to your path
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc140-mt.lib")
#else
#pragma comment (lib, "assimp-vc140-mt.lib")
#endif

#define TEXTURE_SET_0 0

CFBXLoaderCustom::CFBXLoaderCustom()
{
}


CFBXLoaderCustom::~CFBXLoaderCustom()
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

int CFBXLoaderCustom::DetermineAndLoadVerticies(aiMesh* fbxMesh, CLoaderMesh* aLoaderMesh)
{
	unsigned int modelBluePrintType = 0;

	modelBluePrintType |= (fbxMesh->HasPositions() ? EModelBluePrint_Position : 0);
	modelBluePrintType |= (fbxMesh->HasTextureCoords(0) ? EModelBluePrint_UV : 0);
	modelBluePrintType |= (fbxMesh->HasNormals() ? EModelBluePrint_Normal : 0);
	modelBluePrintType |= (fbxMesh->HasTangentsAndBitangents() ? EModelBluePrint_BinormTan : 0);
	modelBluePrintType |= (fbxMesh->HasBones() ? EModelBluePrint_Bones : 0);

	int vertexBufferSize = 0;
	vertexBufferSize += (fbxMesh->HasPositions() ? sizeof(float) * 4 : 0);
	vertexBufferSize += (fbxMesh->HasTextureCoords(0) ? sizeof(float) * 2 : 0);
	vertexBufferSize += (fbxMesh->HasNormals() ? sizeof(float) * 4 : 0);
	vertexBufferSize += (fbxMesh->HasTangentsAndBitangents() ? sizeof(float) * 8 : 0);
	vertexBufferSize += (fbxMesh->HasBones() ? sizeof(float) * 8 : 0); // Better with an UINT, but this works

	aLoaderMesh->myShaderType = modelBluePrintType;
	aLoaderMesh->myVertexBufferSize = vertexBufferSize;

	aLoaderMesh->myVerticies = new char[vertexBufferSize * fbxMesh->mNumVertices];
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
				aLoaderMesh->myModel->myBoneInfo.emplace_back(bi);


				Matrix44f NodeTransformation = ConvertToEngineMatrix44(fbxMesh->mBones[i]->mOffsetMatrix);

				aLoaderMesh->myModel->myBoneInfo[BoneIndex].BoneOffset = NodeTransformation;
				aLoaderMesh->myModel->myBoneNameToIndex[BoneName] = BoneIndex;
			}
			else
			{
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

	bool hasPositions = false;
	bool hasNormals = false;
	bool hasTangents = false;
	bool hasTextures = false;
	bool hasBones = false;

	hasPositions = fbxMesh->HasPositions();
	hasNormals = fbxMesh->HasNormals();
	hasTangents = fbxMesh->HasTangentsAndBitangents();
	hasTextures = fbxMesh->HasTextureCoords(TEXTURE_SET_0);
	hasBones = fbxMesh->HasBones();

	

	float* data = new float[(vertexBufferSize / 4) * fbxMesh->mNumVertices];
	if (hasPositions && hasNormals && hasTangents && hasTextures && hasBones) {
		for (unsigned int i = 0, dataIndex = 0; i < fbxMesh->mNumVertices; i++, dataIndex += (vertexBufferSize / 4)) {
			
			aiVector3D& mVertice = fbxMesh->mVertices[i];
			data[dataIndex] = mVertice.x;
			data[dataIndex + 1] = mVertice.y;
			data[dataIndex + 2] = mVertice.z;
			data[dataIndex + 3] = 1.0f;

			aiVector3D& mNorm = fbxMesh->mNormals[i];
			data[dataIndex + 4] = mNorm.x;
			data[dataIndex + 5] = mNorm.y;
			data[dataIndex + 6] = mNorm.z;
			data[dataIndex + 7] = 1.0f;

			aiVector3D& mTangent = fbxMesh->mTangents[i];
			data[dataIndex + 8] = mTangent.x;
			data[dataIndex + 9] = mTangent.y;
			data[dataIndex + 10] = mTangent.z;
			data[dataIndex + 11] = 1.0f;

			aiVector3D& biTangent = fbxMesh->mBitangents[i];
			data[dataIndex + 12] = biTangent.x;
			data[dataIndex + 13] = biTangent.y;
			data[dataIndex + 14] = biTangent.z;
			data[dataIndex + 15] = 1.0f;

			data[dataIndex + 16] = fbxMesh->mTextureCoords[TEXTURE_SET_0][i].x;
			data[dataIndex + 17] = fbxMesh->mTextureCoords[TEXTURE_SET_0][i].y;

			VertexBoneData& boneData = collectedBoneData[i];
			data[dataIndex + 18] = (float)boneData.IDs[0];
			data[dataIndex + 19] = (float)boneData.IDs[1];
			data[dataIndex + 20] = (float)boneData.IDs[2];
			data[dataIndex + 21] = (float)boneData.IDs[3];
			//CONFJURMED by Haqvin

			data[dataIndex + 22] = boneData.Weights[0];
			data[dataIndex + 23] = boneData.Weights[1];
			data[dataIndex + 24] = boneData.Weights[2];
			data[dataIndex + 25] = boneData.Weights[3];
		}
	}
	else if (hasPositions && hasNormals && hasTangents && hasTextures) {
		for (unsigned int i = 0, dataIndex = 0; i < fbxMesh->mNumVertices; i++, dataIndex += (vertexBufferSize / 4)) {
			aiVector3D& mVertice = fbxMesh->mVertices[i];
			data[dataIndex] = mVertice.x;
			data[dataIndex + 1] = mVertice.y;
			data[dataIndex + 2] = mVertice.z;
			data[dataIndex + 3] = 1.0f;

			aiVector3D& mNorm = fbxMesh->mNormals[i];
			data[dataIndex + 4] = mNorm.x;
			data[dataIndex + 5] = mNorm.y;
			data[dataIndex + 6] = mNorm.z;
			data[dataIndex + 7] = 1.0f;

			aiVector3D& mTangent = fbxMesh->mTangents[i];
			data[dataIndex + 8] = mTangent.x;
			data[dataIndex + 9] = mTangent.y;
			data[dataIndex + 10] = mTangent.z;
			data[dataIndex + 11] = 1.0f;

			aiVector3D& biTangent = fbxMesh->mBitangents[i];
			data[dataIndex + 12] = biTangent.x;
			data[dataIndex + 13] = biTangent.y;
			data[dataIndex + 14] = biTangent.z;
			data[dataIndex + 15] = 1.0f;

			data[dataIndex + 16] = fbxMesh->mTextureCoords[TEXTURE_SET_0][i].x;
			data[dataIndex + 17] = fbxMesh->mTextureCoords[TEXTURE_SET_0][i].y;
		}
	}
	else if (hasPositions && hasNormals && hasTangents) {
		for (unsigned int i = 0, dataIndex = 0; i < fbxMesh->mNumVertices; i++, dataIndex += (vertexBufferSize / 4)) {
			aiVector3D& mVertice = fbxMesh->mVertices[i];
			data[dataIndex] = mVertice.x;
			data[dataIndex + 1] = mVertice.y;
			data[dataIndex + 2] = mVertice.z;
			data[dataIndex + 3] = 1.0f;

			aiVector3D& mNorm = fbxMesh->mNormals[i];
			data[dataIndex + 4] = mNorm.x;
			data[dataIndex + 5] = mNorm.y;
			data[dataIndex + 6] = mNorm.z;
			data[dataIndex + 7] = 1.0f;

			aiVector3D& mTangent = fbxMesh->mTangents[i];
			data[dataIndex + 8] = mTangent.x;
			data[dataIndex + 9] = mTangent.y;
			data[dataIndex + 10] = mTangent.z;
			data[dataIndex + 11] = 1.0f;

			aiVector3D& biTangent = fbxMesh->mBitangents[i];
			data[dataIndex + 12] = biTangent.x;
			data[dataIndex + 13] = biTangent.y;
			data[dataIndex + 14] = biTangent.z;
			data[dataIndex + 15] = 1.0f;		
		}
	}
	else if (hasPositions && hasNormals) {
		for (unsigned int i = 0, dataIndex = 0; i < fbxMesh->mNumVertices; i++, dataIndex += (vertexBufferSize / 4)) {

			aiVector3D& mVertice = fbxMesh->mVertices[i];
			data[dataIndex] = mVertice.x;
			data[dataIndex + 1] = mVertice.y;
			data[dataIndex + 2] = mVertice.z;
			data[dataIndex + 3] = 1.0f;

			aiVector3D& mNorm = fbxMesh->mNormals[i];
			data[dataIndex + 4] = mNorm.x;
			data[dataIndex + 5] = mNorm.y;
			data[dataIndex + 6] = mNorm.z;
			data[dataIndex + 7] = 1.0f;
		}
	}
	else if (hasPositions) {
		for (unsigned int i = 0, dataIndex = 0; i < fbxMesh->mNumVertices; i++, dataIndex += (vertexBufferSize / 4)) {
			aiVector3D& mVertice = fbxMesh->mVertices[i];
			data[dataIndex] = mVertice.x;
			data[dataIndex + 1] = mVertice.y;
			data[dataIndex + 2] = mVertice.z;
			data[dataIndex + 3] = 1.0f;
		}
	}

	memmove(aLoaderMesh->myVerticies, data, vertexBufferSize* fbxMesh->mNumVertices);
	return vertexBufferSize;
}

CLoaderModel* CFBXLoaderCustom::LoadModel(const char* aModel)
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

bool does_file_exist(const char* fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void* CFBXLoaderCustom::LoadModelInternal(CLoaderModel* someInput)
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


	CLoaderMesh* mesh = model->CreateMesh();
	for (unsigned int n = 0; n < scene->mNumMeshes; ++n)
	{
		aiMesh* fbxMesh = scene->mMeshes[n];
		DetermineAndLoadVerticies(fbxMesh, mesh);
		for (unsigned int i = 0; i < fbxMesh->mNumFaces; i++)
		{
			mesh->myIndexes.insert(mesh->myIndexes.end(), std::make_move_iterator(&fbxMesh->mFaces[i].mIndices[0]), std::make_move_iterator(&fbxMesh->mFaces[i].mIndices[fbxMesh->mFaces[i].mNumIndices]));
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


void CFBXLoaderCustom::LoadMaterials(const struct aiScene* sc, CLoaderModel* aModel)
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

void CFBXLoaderCustom::LoadTexture(int aType, std::vector<std::string>& someTextures, aiMaterial* aMaterial)
{
	int texIndex = 0;
	aiReturn texFound = AI_SUCCESS;

	aiString path;	// filename

	texFound = aMaterial->GetTexture((aiTextureType)aType, texIndex, &path);
	if (texFound == AI_FAILURE)
	{
		someTextures.emplace_back("");
		return;
	}

	std::string filePath = std::string(path.data);

	const size_t last_slash_idx = filePath.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		filePath.erase(0, last_slash_idx + 1);
	}

	someTextures.emplace_back(filePath);
}