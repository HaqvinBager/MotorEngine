#include "stdafx.h"
#include "GameObject.h"
#include "StatsComponent.h"
#include "TokenPool.h"

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
}

void CStatsComponent::OnEnable()
{
}

void CStatsComponent::OnDisable()
{
}

const SBaseStats& CStatsComponent::GetBaseStats() const 
{
	return myBaseStats;
}

SStats& CStatsComponent::GetStats()
{
	return myStats;
}