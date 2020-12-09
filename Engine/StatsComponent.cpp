#include "stdafx.h"
#include "GameObject.h"
#include "StatsComponent.h"
#include "TokenPool.h"
#include "Timer.h"

CStatsComponent::CStatsComponent(CGameObject& aParent, float aHealth, float aDamage, float aMoveSpeed, float aDamageCooldown, float aVisionRange, float aAttackRange, float anExperienceAmount)
	: CBehaviour(aParent)
	, myBaseStats({aHealth, 100.0f, aDamage, aMoveSpeed, aDamageCooldown, aVisionRange, aAttackRange, 1.5f, 100.f, 3, 1.f})
	, myStats({aHealth, 100.0f, aDamageCooldown, 0.0f, true, false, nullptr, myBaseStats.myBaseRandomWalkTime, anExperienceAmount, 0, false, myBaseStats.myNextTokenBaseCooldown})
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

	if (myStats.hadToken) {
		myStats.myNextTokenCooldown -= CTimer::Dt();
		if (myStats.myNextTokenCooldown <= 0.f) {
			myStats.hadToken = false;
			myStats.myNextTokenCooldown = myBaseStats.myNextTokenBaseCooldown;
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

void CStatsComponent::NextTokenCooldown()
{
	if (myStats.hadToken == false) {
		myStats.hadToken = true;
	} 
}
