#include "stdafx.h"
#include "DestructibleComponent.h"

#include "AnimationComponent.h"

DestructibleComponent::DestructibleComponent(CGameObject& aParent)
	: CBehaviour(aParent)
{}

void DestructibleComponent::Awake()
{}

void DestructibleComponent::Start()
{}

void DestructibleComponent::Update()
{}

void DestructibleComponent::Collided(CGameObject* /*aCollidedGameObject*/)
{
	GameObject().GetComponent<CAnimationComponent>()->DeadState();
}

void DestructibleComponent::OnEnable()
{}

void DestructibleComponent::OnDisable()
{}
