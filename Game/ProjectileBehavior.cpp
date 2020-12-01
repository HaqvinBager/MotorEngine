#include "stdafx.h"
#include "ProjectileBehavior.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "MouseTracker.h"
#include "Engine.h"

namespace SM = DirectX::SimpleMath;

CProjectileBehavior::CProjectileBehavior(float aSpeed, float aDuration)
{
	myDirection = {0.0f, 0.0f, 0.0f};
	mySpeed = aSpeed;

	myDuration = aDuration;
	myTimer = 0.0f;
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

void CProjectileBehavior::Init(CGameObject* aGameObject)
{
	myDirection = MouseTracker::ScreenPositionToWorldPosition() - aGameObject->GetComponent<CTransformComponent>()->Position();
	myDirection.Normalize();
}
