#include "stdafx.h"
#include "AbilityBehavior.h"

IAbilityBehavior::IAbilityBehavior() : 
	myDuration(0.0f), 
	myTimer(0.0f), 
	myResourceCost(0.0f),
	myDamage(0.0f)
{
}

IAbilityBehavior::~IAbilityBehavior()
{
}
