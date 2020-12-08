#include "stdafx.h"
#include "GameObject.h"
#include "StatsComponent.h"
#include "TokenPool.h"
#include "Timer.h"

CStatsComponent::CStatsComponent(CGameObject& aParent, float aHealth, float aDamage, float aMoveSpeed, float aDamageCooldown, float aVisionRange, float aAttackRange)
	: CBehaviour(aParent)
	, myBaseStats({aHealth, 100.0f, aDamage, aMoveSpeed, aDamageCooldown, aVisionRange, aAttackRange, 1.5f})
	, myStats({aHealth, 100.0f, aDamageCooldown, 0.0f, true, false, nullptr, myBaseStats.mBaseRandomWalkTime})
{

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
	if (!myStats.myCanTakeDamage) {
		if (myStats.myDamageCooldown <= 0.0f) {
			myStats.myCanTakeDamage = true;
		} else {
			myStats.myDamageCooldown -= CTimer::Dt();
		}
	}
}

void CStatsComponent::OnEnable()
{
}

void CStatsComponent::OnDisable()
{
}

bool CStatsComponent::AddDamage(float someDamage)
{
	if (myStats.myCanTakeDamage) {
		myStats.myHealth -= someDamage;
		myStats.myCanTakeDamage = false;
		myStats.myDamageCooldown = myBaseStats.myBaseDamageCooldown;
		return true;
	}
	return false;
}

const SBaseStats& CStatsComponent::GetBaseStats() const 
{
	return myBaseStats;
}

SStats& CStatsComponent::GetStats()
{
	return myStats;
}