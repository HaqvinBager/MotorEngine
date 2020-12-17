#include "stdafx.h"
#include "DestructibleComponent.h"

#include "MainSingleton.h"
#include "PostMaster.h"
#include "AnimationComponent.h"

CDestructibleComponent::CDestructibleComponent(CGameObject& aParent)
	: CBehaviour(aParent)
	, myIsDead(false)
{}

void CDestructibleComponent::Awake()
{}

void CDestructibleComponent::Start()
{}

void CDestructibleComponent::Update()
{}

void CDestructibleComponent::Collided(CGameObject* /*aCollidedGameObject*/)
{
	//GameObject().GetComponent<CAnimationComponent>()->DeadState();
}

void CDestructibleComponent::OnEnable()
{}

void CDestructibleComponent::OnDisable()
{
}

void CDestructibleComponent::IsDead(bool isDead)
{
	myIsDead = isDead;
	if (myIsDead == true) {
		GameObject().GetComponent<CAnimationComponent>()->DeadState();
		CMainSingleton::PostMaster().Send({ EMessageType::HitDestructible, nullptr });
	}
}
