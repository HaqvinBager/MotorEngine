#include "stdafx.h"
#include "EnemyFactory.h"
#include "GameObject.h"
#include "AnimationComponent.h"
#include "Animation.h"
#include "StatsComponent.h"
#include "TransformComponent.h"
#include "CapsuleColliderComponent.h"
#include "CircleColliderComponent.h"
#include "ModelComponent.h"
#include "Scene.h"
#include "Model.h"
#include "MainSingleton.h"

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

CGameObject& CEnemyFactory::CreateEnemy(const int aInstanceID, DirectX::SimpleMath::Vector3 aPosition, float aHealth, float aDamage, float aMoveSpeed, float aCooldown) const
{
	CGameObject* enemy = new CGameObject(aInstanceID);

	enemy->myTransform->Scale(1.0f);
	enemy->myTransform->Position(aPosition);
	enemy->myTransform->Rotation({ 0.0f, 0.0f, 0.0f });


	enemy->AddComponent<CCircleColliderComponent>(*enemy, 0.5f, ECollisionLayer::ENEMY, static_cast<int>(ECollisionLayer::ENEMY) | static_cast<int>(ECollisionLayer::FLOOR));

	/*auto modelComponent =*/ enemy->AddComponent<CModelComponent>(*enemy, "Assets/3D/Character/CH_NPC_enemy_01_19G4_1_19/CH_NPC_enemy_01_19G4_1_19.fbx");

	enemy->AddComponent<CStatsComponent>(*enemy, aHealth, aDamage, aMoveSpeed, aCooldown);

	//---------Animation-----------
	//CAnimationComponent* animation = enemy->AddComponent<CAnimationComponent>(*enemy);
	//std::vector<std::string> somePathsToAnimations;
	//somePathsToAnimations.push_back("ani/CH_NPC_Undead@Walk_01_17G3_AN.fbx");
	//somePathsToAnimations.push_back("ani/CH_NPC_Undead@Idle_01_17G3_AN.fbx");
	//
	//const std::string rigModel = "Ani/CH_NPC_Undead_17G3_SK.fbx";
	//animation->GetMyAnimation()->Init(rigModel.c_str(), somePathsToAnimations);
	//modelComponent->GetMyModel()->AddAnimation(animation->GetMyAnimation());
	//animation->SetBlend(0, 1, 1.0f);
	//------------------------------

	//CScene::GetInstance()->AddInstance(enemy);

	return *enemy;
}
