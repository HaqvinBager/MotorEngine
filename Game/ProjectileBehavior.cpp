#include "stdafx.h"
#include "ProjectileBehavior.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "MouseTracker.h"
#include "Engine.h"

namespace SM = DirectX::SimpleMath;

CProjectileBehavior::CProjectileBehavior(float aSpeed, float aDuration, float aDamage)
{
	myDirection = {0.0f, 0.0f, 0.0f};
	mySpeed = aSpeed;

	myDuration = aDuration;
	myTimer = 0.0f;
	myDamage = aDamage;
}

CProjectileBehavior::~CProjectileBehavior()
{
}

void CProjectileBehavior::Update(CGameObject* aParent)
{
	myTimer += CTimer::Dt();
	if (myTimer > myDuration) {
		myTimer = 0.0f;
		aParent->Active(false);
	}
	aParent->GetComponent<CTransformComponent>()->Move(myDirection * mySpeed * CTimer::Dt());
}

void CProjectileBehavior::Init(CGameObject* aCaster)
{
	myDirection = MouseTracker::ScreenPositionToWorldPosition() - aCaster->GetComponent<CTransformComponent>()->Position();
	myDirection.Normalize();
}
