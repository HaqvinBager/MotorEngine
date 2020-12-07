#include "stdafx.h"
#include "AbilityBehavior.h"
#include "PlayerControllerComponent.h"
#include "AIBehavior.h"
#include "AIBehaviorComponent.h"

IAbilityBehavior::IAbilityBehavior() : 
	myDuration(0.0f), 
	myTimer(0.0f), 
	myResourceCost(0.0f),
	myDamageMultiplier(0.0f)
{
	myDirection = {0.0f, 0.0f, 0.0f};
}

IAbilityBehavior::~IAbilityBehavior()
{
}

void IAbilityBehavior::Collided(CGameObject* aGameObject)
{
	CPlayerControllerComponent* aPlayer = aGameObject->GetComponent<CPlayerControllerComponent>();
	if (aPlayer) {
		aPlayer->TakeDamage(myDamageMultiplier, aGameObject);
		return;
	}
	CAIBehaviorComponent* AIBehavior = aGameObject->GetComponent<CAIBehaviorComponent>();
	if (AIBehavior) {
		AIBehavior->AIBehavior()->TakeDamage(myDamageMultiplier, aGameObject);
		return;
	}
	
}
