#include "stdafx.h"
#include "HealthBarComponent.h"
#include "Canvas.h"
#include "AnimatedUIElement.h"

CHealthBarComponent::CHealthBarComponent(CGameObject& aParant, CScene& aScene, std::string aPath)
	: CBehaviour(aParant)
{
	myCanvas = new CCanvas();
	myCanvas->Init(aPath, aScene, false);
}

CHealthBarComponent::~CHealthBarComponent()
{
	delete myCanvas;
	myCanvas = nullptr;
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
	if (myEnabled) {
		for (auto sprite : myCanvas->GetAnimatedUI()) {
			sprite->GetInstance()->SetShouldRender(true);
		}
	}
}

void CHealthBarComponent::OnDisable()
{
	for (const auto& sprite : myCanvas->GetAnimatedUI()) {
		sprite->GetInstance()->SetShouldRender(false);
	}
}
