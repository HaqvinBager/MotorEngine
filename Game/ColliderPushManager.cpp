#include "stdafx.h"
#include "ColliderPushManager.h"
#include "CircleColliderComponent.h"
#include "Engine.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "AStar.h"

CColliderPushManager::CColliderPushManager()
{
}

CColliderPushManager::~CColliderPushManager()
{
}

void CColliderPushManager::EnemiesPushOutEnemies()
{
	std::vector<CGameObject*> enemies = CEngine::GetInstance()->GetActiveScene().GetEnemies();
	for (int i = 0; i < enemies.size(); ++i) {
		for (int y = i + 1; y < enemies.size(); ++y) {
			if (enemies[i]->GetComponent<CCircleColliderComponent>()->Collided(enemies[y]->GetComponent<CCircleColliderComponent>())) {
				Vector3 penLength = enemies[i]->myTransform->Position() - enemies[y]->myTransform->Position();
				float len = penLength.Length();

				float penDepth = enemies[i]->GetComponent<CCircleColliderComponent>()->GetRadius() + enemies[y]->GetComponent<CCircleColliderComponent>()->GetRadius() - len;

				DirectX::SimpleMath::Vector3 enemyAPos = { enemies[i]->myTransform->Position().x + (0.5f * penDepth), enemies[i]->myTransform->Position().y, enemies[i]->myTransform->Position().z + (0.5f * penDepth) };
				if (CEngine::GetInstance()->GetActiveScene().GetNavMesh()->GetTriangleAtPoint(enemyAPos)) {
					enemies[i]->myTransform->Position(enemyAPos);
				}
				DirectX::SimpleMath::Vector3 enemyBPos = { enemies[y]->myTransform->Position().x - (0.5f * penDepth), enemies[y]->myTransform->Position().y, enemies[y]->myTransform->Position().z - (0.5f * penDepth) };
				if (CEngine::GetInstance()->GetActiveScene().GetNavMesh()->GetTriangleAtPoint(enemyBPos)) {
					enemies[y]->myTransform->Position(enemyBPos);
				}
			}
		}
	}
}

void CColliderPushManager::PlayerPushOutEnemies()
{
	std::vector<CGameObject*> enemies = CEngine::GetInstance()->GetActiveScene().GetEnemies();
	CGameObject* player = CEngine::GetInstance()->GetActiveScene().GetPlayer();
	for (int i = 0; i < enemies.size(); ++i) {
		if (enemies[i]->GetComponent<CCircleColliderComponent>()->Collided(player->GetComponent<CCircleColliderComponent>())) {
			Vector3 penLength = enemies[i]->myTransform->Position() - player->myTransform->Position();
			float len = penLength.Length();

			float penDepth = enemies[i]->GetComponent<CCircleColliderComponent>()->GetRadius() + player->GetComponent<CCircleColliderComponent>()->GetRadius() - len;

			DirectX::SimpleMath::Vector3 enemyPos = { enemies[i]->myTransform->Position().x + (0.5f * penDepth), enemies[i]->myTransform->Position().y, enemies[i]->myTransform->Position().z + (0.5f * penDepth) };
			if (CEngine::GetInstance()->GetActiveScene().GetNavMesh()->GetTriangleAtPoint(enemyPos)) {
				enemies[i]->myTransform->Position(enemyPos);
			}
			DirectX::SimpleMath::Vector3 playerPos = { player->myTransform->Position().x - (0.5f * penDepth), player->myTransform->Position().y, player->myTransform->Position().z - (0.5f * penDepth) };
			if (CEngine::GetInstance()->GetActiveScene().GetNavMesh()->GetTriangleAtPoint(playerPos)) {
				player->myTransform->Position(playerPos);
			}
		}
	}
}
