#include "stdafx.h"
#include "EnemyFactory.h"
#include "GameObject.h"
#include "AnimationComponent.h"
#include "Animation.h"
#include "StatsComponent.h"
#include "TransformComponent.h"
#include "CapsuleColliderComponent.h"
#include "ModelComponent.h"
#include "Scene.h"
#include "Model.h"

CEnemyFactory* CEnemyFactory::ourInstance = nullptr;

CEnemyFactory* CEnemyFactory::GetInstance()
{
	return ourInstance;
}

CEnemyFactory::CEnemyFactory() {
	ourInstance = this;
}

CEnemyFactory::~CEnemyFactory()
{
	ourInstance = nullptr;
}

CGameObject& CEnemyFactory::CreateEnemy(DirectX::SimpleMath::Vector3 aPosition, float aHealth, float aDamage, float aMoveSpeed, float aCooldown) const
{
	CGameObject* enemy = new CGameObject();

	CTransformComponent* transform = enemy->AddComponent<CTransformComponent>(*enemy);
	transform->Scale(1.0f);
	transform->Position(aPosition);
	transform->Rotation({ 0.0f, 0.0f, 0.0f });
	enemy->AddComponent<CCapsuleColliderComponent>(*enemy, 0.5f, 2.0f);
	auto modelComponent = enemy->AddComponent<CModelComponent>(*enemy, "ani/CH_NPC_Undead_17G3_SK.fbx");
	
	CAnimationComponent* animation = enemy->AddComponent<CAnimationComponent>(*enemy);
	std::vector<std::string> somePathsToAnimations;
	somePathsToAnimations.push_back("ani/CH_NPC_Undead@Walk_01_17G3_AN.fbx");
	somePathsToAnimations.push_back("ani/CH_NPC_Undead@Idle_01_17G3_AN.fbx");
	
	const std::string rigModel = "Ani/CH_NPC_Undead_17G3_SK.fbx";
	animation->GetMyAnimation()->Init(rigModel.c_str(), somePathsToAnimations);
	modelComponent->GetMyModel()->AddAnimation(animation->GetMyAnimation());
	animation->SetBlend(0, 1, 1.0f);

	enemy->AddComponent<CStatsComponent>(*enemy, aHealth, aDamage, aMoveSpeed, aCooldown);
	CScene::GetInstance()->AddInstance(enemy);

	return *enemy;
}
