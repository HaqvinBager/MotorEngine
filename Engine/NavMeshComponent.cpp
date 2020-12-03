#include "stdafx.h"
#include "NavMeshComponent.h"
#include "GameObject.h"
#include "AStar.h"
#include "TransformComponent.h"
#include "Engine.h"
#include "Scene.h"
#include "MouseTracker.h"


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

void CNavMeshComponent::CalculatePath(DirectX::SimpleMath::Vector3 aDestination)
{
	if (!myNavMesh) { return; }

	std::vector<DirectX::SimpleMath::Vector3> path;
	path = CAStar::GetInstance()->GetPath(GameObject().myTransform->Position(), aDestination, myNavMesh, myNavMesh->GetTriangleAtPoint(GameObject().myTransform->Position()), myNavMesh->GetTriangleAtPoint(aDestination));
	this->GameObject().myTransform->SetPath(path, aDestination);
}

void CNavMeshComponent::CalculatePath()
{
	if (!myNavMesh) { return; }

	STriangle* triangle = nullptr;
	DirectX::SimpleMath::Ray ray = MouseTracker::WorldSpacePick();

	for (unsigned int i = 0; i < myNavMesh->myTriangles.size(); ++i) {
		triangle = myNavMesh->myTriangles[i];
		float dist = 0;
		if (ray.Intersects(triangle->myVertexPositions[0], triangle->myVertexPositions[1], triangle->myVertexPositions[2], dist)) {
			DirectX::SimpleMath::Vector3 finalPosition = ray.position + ray.direction * dist;

			std::vector<DirectX::SimpleMath::Vector3> path = 
				CAStar::GetInstance()->GetPath(GameObject().myTransform->Position(), finalPosition ,myNavMesh, myNavMesh->GetTriangleAtPoint(GameObject().myTransform->Position()), triangle);
			
			this->GameObject().myTransform->SetPath(path, finalPosition);
			break;
		}
	}
}