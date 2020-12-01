#include "stdafx.h"
#include "GameObject.h"
#include "StatsComponent.h"
#include "TokenPool.h"

CStatsComponent::CStatsComponent(CGameObject& aParent, float aHealth, float aDamage, float aMoveSpeed, float aDamageCooldown, float aVisionRange, float aAttackRange)
	: CBehaviour(aParent)
	, myBaseStats({aHealth, aDamage, aMoveSpeed, aDamageCooldown, aVisionRange, aAttackRange})
	, myStats({aHealth, aDamageCooldown, true, false, nullptr})
{
	//std::cout << "Enemy base stats " << std::endl
	//	<< "base hp " << myBaseStats.myBaseHealth << std::endl
	//	<< "base dmg " << myBaseStats.myDamage << std::endl
	//	<< "base speed " << myBaseStats.myMoveSpeed << std::endl
	//	<< "base dmg cd " << myBaseStats.myBaseDamageCooldown << std::endl
	//	<< "base vision " << myBaseStats.myBaseVisionRange << std::endl
	//	<< "base range " << myBaseStats.myBaseAttackRange << std::endl;
	//std::cout << "Enemy stats " << std::endl
	//	<< "hp " << myStats.myHealth << std::endl;
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