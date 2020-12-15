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

	auto startPosition = GameObject().myTransform->Position();
	auto startTriangle = myNavMesh->GetTriangleAtPoint(startPosition);

	if (startTriangle == nullptr)
	{
		ResolveStuck(startTriangle, startPosition, aDestination);
		if (startTriangle == nullptr)
		{
			// Return closest triangle if ResolveStuck doesn't work
			startTriangle = ReturnClosestTriangle(startPosition);
		}
	}

	path = CAStar::GetInstance()->GetPath(startPosition, aDestination, myNavMesh, startTriangle, myNavMesh->GetTriangleAtPoint(aDestination));
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

			auto startPosition = GameObject().myTransform->Position();
			auto startTriangle = myNavMesh->GetTriangleAtPoint(startPosition);

			if (startTriangle == nullptr)
			{
				ResolveStuck(startTriangle, startPosition, finalPosition);
				if (startTriangle == nullptr) 
				{
					// Return closest triangle if ResolveStuck doesn't work
					startTriangle = ReturnClosestTriangle(startPosition);
				}
			}

			std::vector<DirectX::SimpleMath::Vector3> path = 
				CAStar::GetInstance()->GetPath(startPosition, finalPosition, myNavMesh, startTriangle, triangle);
			
			this->GameObject().myTransform->SetPath(path, finalPosition);
			break;
		}
	}
}

STriangle* CNavMeshComponent::ReturnClosestTriangle(const DirectX::SimpleMath::Vector3& aStartPosition)
{	
	STriangle* closestTriangle = nullptr;
	float lastDistance = FLT_MAX;
	for (unsigned int i = 0; i < myNavMesh->myTriangles.size(); ++i)
	{
		float currentDistance = DirectX::SimpleMath::Vector3::DistanceSquared(aStartPosition, myNavMesh->myTriangles[i]->myCenterPosition);
		if (currentDistance < lastDistance)
		{
			lastDistance = currentDistance;
			closestTriangle = myNavMesh->myTriangles[i];
		}
	}
	return closestTriangle;
}

void CNavMeshComponent::ResolveStuck(STriangle* aStartTriangle, const DirectX::SimpleMath::Vector3& aStartPosition, const DirectX::SimpleMath::Vector3& aFinalPosition)
{
	// Try to find triangle in front, behind, to the left and right of player
	float tolerance = 0.1f;
	if (!aStartTriangle)
	{
		DirectX::SimpleMath::Vector3 dir = aFinalPosition;
		dir -= aStartPosition;
		dir.Normalize();
		dir *= tolerance;
		DirectX::SimpleMath::Vector3 newPos = aStartPosition;
		newPos += dir;
		aStartTriangle = myNavMesh->GetTriangleAtPoint(newPos);
	}
	if (!aStartTriangle)
	{
		DirectX::SimpleMath::Vector3 dir = aFinalPosition;
		dir -= aStartPosition;
		dir.Normalize();
		dir *= tolerance;
		DirectX::SimpleMath::Vector3 newPos = aStartPosition;
		newPos -= dir;
		aStartTriangle = myNavMesh->GetTriangleAtPoint(newPos);
	}
	if (!aStartTriangle)
	{
		DirectX::SimpleMath::Vector3 dir = aFinalPosition;
		dir -= aStartPosition;
		DirectX::SimpleMath::Vector3 temp;
		temp.x = -dir.y;
		temp.y = dir.x;
		dir = temp;
		dir.Normalize();
		dir *= tolerance;
		DirectX::SimpleMath::Vector3 newPos = aStartPosition;
		newPos += dir;
		aStartTriangle = myNavMesh->GetTriangleAtPoint(newPos);
	}
	if (!aStartTriangle)
	{
		DirectX::SimpleMath::Vector3 dir = aFinalPosition;
		dir -= aStartPosition;
		DirectX::SimpleMath::Vector3 temp;
		temp.x = -dir.y;
		temp.y = dir.x;
		dir = temp;
		dir.Normalize();
		dir *= tolerance;
		DirectX::SimpleMath::Vector3 newPos = aStartPosition;
		newPos -= dir;
		aStartTriangle = myNavMesh->GetTriangleAtPoint(newPos);
	}
}
