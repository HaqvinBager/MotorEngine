#include "stdafx.h"
#include "NavMeshComponent.h"
#include "GameObject.h"
//#include "NavmeshLoader.h"
#include "AStar.h"
#include "TransformComponent.h"

#include <iostream>
CNavMeshComponent::CNavMeshComponent(CGameObject& aParent, SNavMesh* aNavMesh) 
	: CBehaviour(aParent), myNavMesh(aNavMesh)
{
	myEnabled = true;
}

CNavMeshComponent::~CNavMeshComponent()
{
}

void CNavMeshComponent::Awake()
{
}

void CNavMeshComponent::Start()
{
}

void CNavMeshComponent::Update()
{
	if (!myNavMesh) {
		std::cout << "navmesh gonne D:" << std::endl;
	}
}

void CNavMeshComponent::OnEnable()
{
}

void CNavMeshComponent::OnDisable()
{
}

std::vector<DirectX::SimpleMath::Vector3> CNavMeshComponent::CalculatePath(DirectX::SimpleMath::Vector3 aDestination)
{
	std::vector<DirectX::SimpleMath::Vector3> path;
	path = CAStar::AStar(myNavMesh, myNavMesh->GetTriangleAtPoint(GameObject().myTransform->Position()), myNavMesh->GetTriangleAtPoint(aDestination));
	return std::move(path);
}

std::vector<DirectX::SimpleMath::Vector3> CNavMeshComponent::CalculatePath(DirectX::SimpleMath::Ray aRay, DirectX::SimpleMath::Vector3& anOutPosition)
{
	STriangle* triangle = nullptr;
	for (unsigned int i = 0; i < myNavMesh->myTriangles.size(); ++i) {
		triangle = myNavMesh->myTriangles[i];
		float dist = 0;
		if (aRay.Intersects(triangle->myVertexPositions[0], triangle->myVertexPositions[1], triangle->myVertexPositions[2], dist)) {
			anOutPosition = aRay.position + aRay.direction * dist;
			break;
		}
	}

	std::vector<DirectX::SimpleMath::Vector3> path;
	path = CAStar::AStar(myNavMesh, myNavMesh->GetTriangleAtPoint(GameObject().myTransform->Position()), triangle);
	return std::move(path);
}