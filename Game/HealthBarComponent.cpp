#include "stdafx.h"
#include "HealthBarComponent.h"
#include "Canvas.h"
#include "AnimatedUIElement.h"
#include "Utility.h"
#include "TransformComponent.h"
#include "StatsComponent.h"
#include "TextInstance.h"

CHealthBarComponent::CHealthBarComponent(CGameObject& aParant, CScene& aScene)
	: CBehaviour(aParant)
	, isActive(false)
{
	myCanvas = new CCanvas();
	myCanvas->Init("Json/UI_InGame_Enemy_HealthBarSmall.json", aScene, false);

	myCanvas2 = new CCanvas();
	myCanvas2->Init("Json/UI_InGame_Enemy_HealthBar.json", aScene, false);

	if (aParant.GetComponent<CStatsComponent>()->GetBaseStats().myBaseHealth >= 80.f) {
		myCanvas2->GetTexts()[0]->SetText("Guard Captain Loghaine");
	}
}

CHealthBarComponent::~CHealthBarComponent()
{
	delete myCanvas;
	myCanvas = nullptr;

	delete myCanvas2;
	myCanvas2 = nullptr;
}

void CHealthBarComponent::Awake()
{
	for (const auto& sprite : myCanvas->GetAnimatedUI()) {
		CEngine::GetInstance()->GetActiveScene().AddInstance(sprite);
	}
	for (const auto& sprite : myCanvas2->GetAnimatedUI()) {
		CEngine::GetInstance()->GetActiveScene().AddInstance(sprite);
	}
	for (auto text : myCanvas2->GetTexts()) {
		CEngine::GetInstance()->GetActiveScene().AddInstance(text);
	}
}

void CHealthBarComponent::Start()
{
}

void CHealthBarComponent::Update()
{
	if (GameObject().GetComponent<CStatsComponent>()->GetStats().myHealth < GameObject().GetComponent<CStatsComponent>()->GetBaseStats().myBaseHealth && GameObject().GetComponent<CStatsComponent>()->GetStats().myHealth > 0.f) {
		DirectX::SimpleMath::Vector2 screen = CUtility::WorldToScreen(GameObject().myTransform->Position());
		screen.y -= 0.35f;
		for (auto sprite : myCanvas->GetAnimatedUI()) {
			sprite->GetInstance()->SetNormalPosition({ screen.x, -screen.y });
			sprite->GetInstance()->SetShouldRender(true);
		}
	} 
	if(GameObject().GetComponent<CStatsComponent>()->GetStats().myHealth <= 0.f) {
		for (auto sprite : myCanvas->GetAnimatedUI()) {
			sprite->GetInstance()->SetShouldRender(false);
		}
	}
}

void CHealthBarComponent::OnEnable()
{
	if (myEnabled) {
		for (auto sprite : myCanvas2->GetAnimatedUI()) {
			sprite->GetInstance()->SetShouldRender(true);
		}
		for (auto text : myCanvas2->GetTexts()) {
			text->SetShouldRender(true);
		}
		isActive = true;
	}
}

void CHealthBarComponent::OnDisable()
{
	for (const auto& sprite : myCanvas2->GetAnimatedUI()) {
		sprite->GetInstance()->SetShouldRender(false);
	}
	for (auto text : myCanvas2->GetTexts()) {
		text->SetShouldRender(false);
	}
	isActive = false;
}
