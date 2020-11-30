#include "stdafx.h"
#include "AbilityBehaviorComponent.h"
#include "GameObject.h"
#include "AbilityBehavior.h"

CAbilityBehaviorComponent::CAbilityBehaviorComponent(CGameObject& aParent, IAbilityBehavior* aBehavior, EAbilityType anAbilityType) 
	: CBehaviour(aParent), myBehavior(aBehavior), myAbilityType(anAbilityType)
{
}

CAbilityBehaviorComponent::~CAbilityBehaviorComponent()
{
	delete myBehavior;
	myBehavior = nullptr;
}

void CAbilityBehaviorComponent::Awake()
{
}

void CAbilityBehaviorComponent::Start()
{
}

void CAbilityBehaviorComponent::Update()
{
	if(myEnabled)
	myBehavior->Update(&GameObject());
}

void CAbilityBehaviorComponent::OnEnable()
{
	Enabled(true);
}

void CAbilityBehaviorComponent::OnDisable()
{
	Enabled(false);
}

EAbilityType CAbilityBehaviorComponent::GetAbilityType() const
{
	return myAbilityType;
}

void CAbilityBehaviorComponent::Init(DirectX::SimpleMath::Vector3 aCasterPosition)
{
	myBehavior->Init(aCasterPosition);
}
