#include "stdafx.h"
#include "AIBehaviorComponent.h"
#include "GameObject.h"
#include "AIBehavior.h"
#include "DialogueSystem.h"

CAIBehaviorComponent::CAIBehaviorComponent(CGameObject& aParent, IAIBehavior* aBehavior)
	: CBehaviour(aParent), myBehavior(aBehavior)
{
}

CAIBehaviorComponent::~CAIBehaviorComponent()
{
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
	if (!CMainSingleton::DialogueSystem().Active()) {
		myBehavior->Update(&GameObject());
	}
}

void CAIBehaviorComponent::Collided(CGameObject* aCollidedGameObject)
{
	myBehavior->Collided(&GameObject(), aCollidedGameObject);
}

void CAIBehaviorComponent::OnEnable()
{
}

void CAIBehaviorComponent::OnDisable()
{
}

IAIBehavior* CAIBehaviorComponent::AIBehavior()
{
	return myBehavior;
}
