#include "stdafx.h"
#include "GameObject.h"
#include "StatsComponent.h"
#include "TokenPool.h"

CStatsComponent::CStatsComponent(CGameObject& aParent, float aHealth, float aDamage, float aMoveSpeed, float aDamageCooldown)
	: CBehaviour(aParent)
	, myBaseStats({aHealth, aDamage, aMoveSpeed, aDamageCooldown})
	, myStats({aHealth, aDamageCooldown, true, false, nullptr})
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