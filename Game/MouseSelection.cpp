#include "stdafx.h"
#include "MouseSelection.h"
#include "AStar.h"
#include "MouseTracker.h"
#include "CircleColliderComponent.h"
#include "HealthBarComponent.h"

CMouseSelection::CMouseSelection()
{
}

CMouseSelection::~CMouseSelection()
{
}

DirectX::SimpleMath::Vector3 CMouseSelection::GetPositionAtNavmesh()
{
	SNavMesh* myNavMesh = CEngine::GetInstance()->GetActiveScene().GetNavMesh();
	if (!myNavMesh) { return DirectX::SimpleMath::Vector3(); }
	
	STriangle* triangle = nullptr;
	DirectX::SimpleMath::Ray ray = MouseTracker::WorldSpacePick();
	
	for (unsigned int i = 0; i < myNavMesh->myTriangles.size(); ++i) {
		triangle = myNavMesh->myTriangles[i];
		float dist = 0;
		if (ray.Intersects(triangle->myVertexPositions[0], triangle->myVertexPositions[1], triangle->myVertexPositions[2], dist)) {
			DirectX::SimpleMath::Vector3 finalPosition = ray.position + ray.direction * dist;
	
			std::vector<DirectX::SimpleMath::Vector3> path = CAStar::GetInstance()->GetPath(finalPosition, finalPosition, myNavMesh, myNavMesh->GetTriangleAtPoint(finalPosition), triangle);
			DirectX::SimpleMath::Vector3 aPath = path[0];
			return aPath;
		}
	}
	return DirectX::SimpleMath::Vector3();
}

void CMouseSelection::FindSelectedEnemy()
{
	Vector3 pos = GetPositionAtNavmesh();
	for (const auto& enemy : CEngine::GetInstance()->GetActiveScene().GetEnemies()) {
		//float dist = DirectX::SimpleMath::Vector3::DistanceSquared(pos, enemy->GetComponent<CTransformComponent>()->Position());
		if (enemy->GetComponent<CCircleColliderComponent>()->Collided(1.5f, pos)) {
			enemy->GetComponent<CHealthBarComponent>()->OnEnable();
		}
		else {
			enemy->GetComponent<CHealthBarComponent>()->OnDisable();
		}
	}
}
