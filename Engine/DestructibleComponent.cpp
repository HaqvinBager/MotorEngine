#include "stdafx.h"
#include "DestructibleComponent.h"

#include "AnimationComponent.h"

CDestructibleComponent::CDestructibleComponent(CGameObject& aParent)
	: CBehaviour(aParent)
{}

void CDestructibleComponent::Awake()
{}

void CDestructibleComponent::Start()
{}

void CDestructibleComponent::Update()
{}

void CDestructibleComponent::Collided(CGameObject* /*aCollidedGameObject*/)
{
	GameObject().GetComponent<CAnimationComponent>()->DeadState();
}

void CDestructibleComponent::OnEnable()
{}

void CDestructibleComponent::OnDisable()
{}
