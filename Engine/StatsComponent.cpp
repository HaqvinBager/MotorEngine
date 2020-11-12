#include "stdafx.h"
#include "GameObject.h"
#include "StatsComponent.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "Scene.h"
#include "ObjectPool.h"
#include "TokenPool.h"
#include "PointLight.h"
#include "LightFactory.h"
#include "MainSingleton.h"

#include <iostream>

CStatsComponent::CStatsComponent(CGameObject& aParent, float aHealth, float aDamage, float aMoveSpeed, float aDamageCooldown)
	: CBehaviour(aParent)
	, myHealth(aHealth)
	, myDamage(aDamage)
	, myMoveSpeed(aMoveSpeed)
	, myBaseDamageCooldown(aDamageCooldown)
	, canTakeDamage(true)
	, canAttack(false)
	, myTokenSlot(nullptr)
{
	myDamageCooldown = myBaseDamageCooldown;
	myPointLight = CLightFactory::GetInstance()->CreatePointLight();
	myPointLight->SetPosition(GameObject().GetComponent<CTransformComponent>()->Position());
	myPointLight->SetColor({ 255.f, 0.f ,0.f });
	myPointLight->SetIntensity(5.f);
	myPointLight->SetRange(5.f);
	CScene::GetInstance()->AddInstance(myPointLight);
	CMainSingleton::PostMaster().Subscribe(EMessageType::EnemyDied, this);
}

CStatsComponent::~CStatsComponent()
{
}

void CStatsComponent::Awake()
{
}

void CStatsComponent::Start()
{
}

void CStatsComponent::Update()
{
	myPointLight->SetPosition(GameObject().GetComponent<CTransformComponent>()->Position());

	if (myTokenSlot == nullptr) {
		myPointLight->SetColor({ 255.f, 0.f ,0.f });
	} else {
		myPointLight->SetColor({0.f, 255.f, 0.f});
	}

	if (canTakeDamage == false) {
		myDamageCooldown -= CTimer::Dt();
		if (myDamageCooldown <= 0) {
			canTakeDamage = true;
			myDamageCooldown = myBaseDamageCooldown;
		}
	}

	if (myHealth <= 0) {
		CMainSingleton::PostMaster().Send({EMessageType::EnemyDied, this});
		if (myTokenSlot != nullptr) {
			CTokenPool::GetInstance()->GiveBack(*myTokenSlot, false);
			myTokenSlot = nullptr;
		}
		CScene::GetInstance()->RemoveInstance(&GameObject());
		CObjectPool::GetInstance()->Remove(&GameObject());
		CScene::GetInstance()->RemoveInstance(myPointLight);
		myPointLight = nullptr;
	}
}

void CStatsComponent::OnEnable()
{
	//CMainSingleton::PostMaster().Subscribe(EMessageType::EnemyDied, this);
}

void CStatsComponent::OnDisable()
{
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::EnemyDied, this);
}

void CStatsComponent::Receive(const SMessage& /*aMessage*/)
{
	//std::cout << "HELLO" << std::endl;
}

void CStatsComponent::FindATarget(CGameObject& aTarget)
{
	float dist = DirectX::SimpleMath::Vector3::DistanceSquared(GameObject().GetComponent<CTransformComponent>()->Position(), aTarget.GetComponent<CTransformComponent>()->Position());
	if (dist <= 20.f) {
		DirectX::SimpleMath::Vector3 dir = aTarget.GetComponent<CTransformComponent>()->Position() - GameObject().GetComponent<CTransformComponent>()->Position();
		dir.Normalize();
		GameObject().GetComponent<CTransformComponent>()->Move(dir * 1.f * CTimer::Dt());
		if (dist <= 5.f) {
			if (myTokenSlot == nullptr) {
				myTokenSlot = CTokenPool::GetInstance()->Request();
			}
		} else {
			if (myTokenSlot != nullptr) {
				CTokenPool::GetInstance()->GiveBack(*myTokenSlot, false);
				myTokenSlot = nullptr;
			}
		}
	}
}

void CStatsComponent::TakeDamage(float aDamage)
{
 	if (canTakeDamage == true) {
		myHealth -= aDamage;
		canTakeDamage = false;
	}
}

float CStatsComponent::GetDamage()
{
	if (myTokenSlot != nullptr) {
		CTokenPool::GetInstance()->GiveBack(*myTokenSlot, true);
		myTokenSlot = nullptr;
	}
	return myDamage;
}

float CStatsComponent::GetHealth() const
{
	return myHealth;
}

float CStatsComponent::GetMoveSpeed() const
{
	return myMoveSpeed;
}

CToken* CStatsComponent::GetToken() const
{
	return myTokenSlot;
}