#include "stdafx.h"
#include "NavmeshLoader.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <fstream>
#include <string>
#include <iostream>
#include <assimp/scene.h>
#include "AStar.h"

CNavmeshLoader::CNavmeshLoader()
{

}

CNavmeshLoader::~CNavmeshLoader()
{

}

bool CNavmeshLoader::Init()
{
	return false;
}

CNavmeshLoader::SNavMesh* CNavmeshLoader::LoadNavmesh(std::string aFilepath)
{
	std::ifstream infile(aFilepath);
	if (!infile.good())
	{
		OutputDebugStringA("File not found");
		return nullptr;
	}

	SNavMesh* navMesh = new SNavMesh();
	const struct aiScene* scene = NULL;

	scene = aiImportFile(aFilepath.c_str(), aiProcess_ValidateDataStructure | aiProcess_MakeLeftHanded | aiProcess_FlipUVs | aiProcess_FlipWindingOrder | 0);

	if (!scene) {
		ENGINE_ERROR_BOOL_MESSAGE(false, "Navmesh could not be loaded.")
	}

	aiMesh* mesh = scene->mMeshes[0];
	MakeTriangles(mesh, navMesh);

	return navMesh;
}

void CNavmeshLoader::MakeTriangles(aiMesh* aMesh, SNavMesh* aNavMesh)
{
	std::vector</*CNavmeshLoader::*/STriangle*> triangles;

	UINT numberOfFaces = aMesh->mNumFaces;

	for (UINT i = 0; i < numberOfFaces; ++i)
	{
		triangles.emplace_back();
		for (UINT j = 0; j < 3; ++j) {
			triangles.back()->myVertexPositions[j] = {
				aMesh->mVertices[aMesh->mFaces[i].mIndices[j]].x,
				aMesh->mVertices[aMesh->mFaces[i].mIndices[j]].y,
				aMesh->mVertices[aMesh->mFaces[i].mIndices[j]].z
			};
		}
		triangles.back()->myCenterPosition = GetCentroid(triangles.back()->myVertexPositions[0], triangles.back()->myVertexPositions[1], triangles.back()->myVertexPositions[2]);
	}

	for (UINT i = 0; i < triangles.size() - 1; ++i) {
		for (UINT j = i + 1; j < triangles.size(); ++j) {
			if (AreNeighbors(triangles[i]->myIndices, triangles[j]->myIndices)) {
				triangles[i]->myNeighbors.emplace_back(triangles[j]);
				triangles[j]->myNeighbors.emplace_back(triangles[i]);
			}
		}
	}

	aNavMesh->myTriangles = triangles;
}

DirectX::SimpleMath::Vector3 CNavmeshLoader::GetCentroid(DirectX::SimpleMath::Vector3& aVectorOne, DirectX::SimpleMath::Vector3& aVectorTwo, DirectX::SimpleMath::Vector3& aVectorThree)
{
	return { 
		(aVectorOne.x + aVectorTwo.x + aVectorThree.x) / 3.0f, 
		(aVectorOne.y + aVectorTwo.y + aVectorThree.y) / 3.0f, 
		(aVectorOne.z + aVectorTwo.z + aVectorThree.z) / 3.0f 
	};
}

bool CNavmeshLoader::AreNeighbors(UINT* someIndices, UINT* someOtherIndices)
{
	UINT counter = 0;
	for (UINT i = 0; i < 3; ++i)
	{
		for (UINT j = 0; j < 3; ++j)
		{
			if (someIndices[i] == someOtherIndices[j]) {
				++counter;
			}		
		}
	}

	return counter >= 2;
}