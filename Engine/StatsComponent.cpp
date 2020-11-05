#include "stdafx.h"
#include "GameObject.h"
#include "StatsComponent.h"
#include "ModelComponent.h"
#include "Scene.h"
#include "ObjectPool.h"

#include <iostream>

CStatsComponent::CStatsComponent(CGameObject& aParent, float aHealth, float aDamage, float aMoveSpeed, float aDamageCooldown)
	: CComponent(aParent)
	, myHealth(aHealth)
	, myDamage(aDamage)
	, myMoveSpeed(aMoveSpeed)
	, myBaseDamageCooldown(aDamageCooldown)
	, canTakeDamage(true)
{
	myDamageCooldown = myBaseDamageCooldown;
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
	if (canTakeDamage == false) {
		myDamageCooldown -= CTimer::Dt();
		if (myDamageCooldown <= 0) {
			canTakeDamage = true;
			myDamageCooldown = myBaseDamageCooldown;
		}
	}

	if (myHealth <= 0) {
		CScene::GetInstance()->RemoveInstance(&GetParent());
		CObjectPool::GetInstance()->Remove(&GetParent());
	}
}

void CStatsComponent::TakeDamage(float aDamage)
{
 	if (canTakeDamage == true) {
		myHealth -= aDamage;
		canTakeDamage = false;
		std::cout << myHealth << std::endl;
	}
}

float CStatsComponent::GetDamage() const
{
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
