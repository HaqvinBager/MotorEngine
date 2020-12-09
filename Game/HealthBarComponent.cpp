#include "stdafx.h"
#include "HealthBarComponent.h"
#include "Canvas.h"
#include "AnimatedUIElement.h"
#include "Utility.h"
#include "TransformComponent.h"
#include "StatsComponent.h"

CHealthBarComponent::CHealthBarComponent(CGameObject& aParant, CScene& aScene, std::string aPath)
	: CBehaviour(aParant)
{
	//myCanvas = new CCanvas();
	//myCanvas->Init(aPath, aScene, false);

	myCanvas2 = new CCanvas();
	myCanvas2->Init(aPath, aScene, false);
	for (const auto& sprite : myCanvas2->GetAnimatedUI()) {
		sprite->SetPosition({0.f, 0.1f});
	}
}

CHealthBarComponent::~CHealthBarComponent()
{
	delete myCanvas;
	myCanvas = nullptr;
}

void CHealthBarComponent::Awake()
{
	//for (const auto& sprite : myCanvas->GetAnimatedUI()) {
	//	CEngine::GetInstance()->GetActiveScene().AddInstance(sprite);
	//}
	for (const auto& sprite : myCanvas2->GetAnimatedUI()) {
		CEngine::GetInstance()->GetActiveScene().AddInstance(sprite);
	}
}

void CHealthBarComponent::Start()
{
}

void CHealthBarComponent::Update()
{
	//if (GameObject().GetComponent<CStatsComponent>()->GetStats().myHealth < GameObject().GetComponent<CStatsComponent>()->GetBaseStats().myBaseHealth && GameObject().GetComponent<CStatsComponent>()->GetStats().myHealth > 0.f) {
	//	DirectX::SimpleMath::Vector2 screen = CUtility::WorldToScreen(GameObject().myTransform->Position());
	//	screen.y -= 0.35f;
	//	for (auto sprite : myCanvas->GetAnimatedUI()) {
	//		sprite->GetInstance()->SetNormalPosition({ screen.x, -screen.y });
	//		sprite->GetInstance()->SetShouldRender(true);
	//	}
	//} 
	//if(GameObject().GetComponent<CStatsComponent>()->GetStats().myHealth <= 0.f) {
	//	for (auto sprite : myCanvas->GetAnimatedUI()) {
	//		sprite->GetInstance()->SetShouldRender(false);
	//	}
	//}
}

void CHealthBarComponent::OnEnable()
{
	if (myEnabled) {
		for (auto sprite : myCanvas2->GetAnimatedUI()) {
			sprite->GetInstance()->SetShouldRender(true);
		}
	}
}

void CHealthBarComponent::OnDisable()
{
	for (const auto& sprite : myCanvas2->GetAnimatedUI()) {
		sprite->GetInstance()->SetShouldRender(false);
	}
}
