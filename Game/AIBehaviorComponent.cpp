#include "stdafx.h"
#include "AIBehaviorComponent.h"
#include "GameObject.h"
#include "AIBehavior.h"

CAIBehaviorComponent::CAIBehaviorComponent(CGameObject& aParent, IAIBehavior* aBehavior)
	: CBehaviour(aParent), myBehavior(aBehavior)
{
}

CAIBehaviorComponent::~CAIBehaviorComponent()
{
	delete myBehavior;
	myBehavior = nullptr;
}

void CAIBehaviorComponent::Awake()
{
}

void CAIBehaviorComponent::Start()
{
}

void CAIBehaviorComponent::Update()
{
	myBehavior->Update(&GameObject());
}

void CAIBehaviorComponent::Collided(CGameObject* aCollidedGameObject)
{
	myBehavior->Collided(aCollidedGameObject);
}

void CAIBehaviorComponent::OnEnable()
{
}

void CAIBehaviorComponent::OnDisable()
{
}
