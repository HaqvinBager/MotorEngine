#pragma once
#include "RandomNumberGenerator.h"

enum class EHitType
{
	Normal,
	Crit,
	UltraCrit,
	Enemy
};

class CDamageUtility
{
public:
	static float CalculateDamage(EHitType& aHitType, float aBase, float anAbilityMultiplier = 1.0f, float aCriticalHitChance = -1.0f, float aCriticalHitMultiplier = 1.0f, float aLowerBound = -2.0f, float anUpperBound = 2.0f)
	{
		float critRoll = Random(0.0f, 1.0f);
		bool isCrit = critRoll < aCriticalHitChance;

		aBase += Random(aLowerBound, anUpperBound);

		if (isCrit) {
			if (critRoll < (0.25f * aCriticalHitChance))
			{
				aHitType = EHitType::UltraCrit;
				return aBase * anAbilityMultiplier * aCriticalHitMultiplier * 1.75f;
			}

			aHitType = EHitType::Crit;
			return aBase * anAbilityMultiplier * aCriticalHitMultiplier;
		}

		aHitType = EHitType::Normal;
		if (aCriticalHitChance < 0.0f) { aHitType = EHitType::Enemy; }

		return aBase;
	}
};

