#include "stdafx.h"
#include "NavMeshComponent.h"
#include "GameObject.h"
#include "AStar.h"
#include "TransformComponent.h"
#include "Engine.h"
#include "Scene.h"

#include <iostream>
CNavMeshComponent::CNavMeshComponent(CGameObject& aParent) 
	: CBehaviour(aParent)
{
	myEnabled = true;
	myNavMesh = nullptr;
}

CNavMeshComponent::~CNavMeshComponent()
{
}

void CNavMeshComponent::Awake()
{
	myNavMesh = CEngine::GetInstance()->GetActiveScene().GetNavMesh();
}

void CNavMeshComponent::Start()
{
}

void CNavMeshComponent::Update()
{
	GameObject().myTransform->MoveAlongPath();
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