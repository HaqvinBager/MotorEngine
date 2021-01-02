#include "stdafx.h"
#include "MouseSelection.h"
#include "AStar.h"
#include "MouseTracker.h"
#include "CircleColliderComponent.h"
#include "HealthBarComponent.h"
#include "StatsComponent.h"
#include "ModelComponent.h"
#include "DestructibleComponent.h"

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

CGameObject* CMouseSelection::FindSelectedEnemy()
{
	Vector3 pos = GetPositionAtNavmesh();
	for (auto& enemy : CEngine::GetInstance()->GetActiveScene().GetEnemies()) {
		//float dist = DirectX::SimpleMath::Vector3::DistanceSquared(pos, enemy->GetComponent<CTransformComponent>()->Position());
		CHealthBarComponent* healthBar = enemy->GetComponent<CHealthBarComponent>();
		if (healthBar == nullptr)
			continue;

		if (enemy->GetComponent<CCircleColliderComponent>()->Collided(1.5f, pos) && enemy->GetComponent<CStatsComponent>()->GetStats().myHealth > 0.f) {

			enemy->GetComponent<CHealthBarComponent>()->OnEnable();
			//this bad :( 
			for (auto& enemy2 : CEngine::GetInstance()->GetActiveScene().GetEnemies()) {
				if (enemy != enemy2 && enemy2->Active() == true) {
					enemy2->GetComponent<CHealthBarComponent>()->OnDisable();
				}
			}
			
			return enemy;
		}
		else {
			enemy->GetComponent<CHealthBarComponent>()->OnDisable();
		}
	}
	return nullptr;
}

CGameObject* CMouseSelection::FindSelectedBoss()
{
	Vector3 pos = GetPositionAtNavmesh();
	CGameObject* boss = CEngine::GetInstance()->GetActiveScene().GetBoss();
	if (boss != nullptr) {
		if (boss->GetComponent<CCircleColliderComponent>()) {
			if (boss->GetComponent<CCircleColliderComponent>()->Collided(1.5f, pos) && boss->GetComponent<CStatsComponent>()->GetStats().myHealth > 0.f) {

				return boss;
			}
		}
	}
	return nullptr;
}

CGameObject* CMouseSelection::FindSelectedDestructible()
{
	Vector3 pos = GetPositionAtNavmesh();
	for (auto& destructible : CEngine::GetInstance()->GetActiveScene().GetDestructibles()) {
		if (destructible->GetComponent<CCircleColliderComponent>()->Collided(1.5f, pos) && destructible->GetComponent<CDestructibleComponent>()->IsDead() == false) {
			return destructible;
		}
	}
	return nullptr;
}
