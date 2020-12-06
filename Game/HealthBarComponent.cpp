#include "stdafx.h"
#include "HealthBarComponent.h"
#include "Canvas.h"
#include "AnimatedUIElement.h"

CHealthBarComponent::CHealthBarComponent(CGameObject& aParant, std::string aPath) 
	: CBehaviour(aParant)
{
	myCanvas = new CCanvas();
	myCanvas->Init(aPath, false);
}

CHealthBarComponent::~CHealthBarComponent()
{
}

void CHealthBarComponent::Awake()
{
	for (const auto& sprite : myCanvas->GetAnimatedUI()) {
		CEngine::GetInstance()->GetActiveScene().AddInstance(sprite);
	}
}

void CHealthBarComponent::Start()
{
}

void CHealthBarComponent::Update()
{
}

void CHealthBarComponent::OnEnable()
{
	for (auto sprite : myCanvas->GetAnimatedUI()) {
		sprite->GetInstance()->SetShouldRender(true);
	}
}

void CHealthBarComponent::OnDisable()
{
	for (const auto& sprite : myCanvas->GetAnimatedUI()) {
		sprite->GetInstance()->SetShouldRender(false);
	}
}
