#include "stdafx.h"
#include "LoadObjectComponent.h"
#include "TransformComponent.h"
#include "RandomNumberGenerator.h"

CLoadObjectComponent::CLoadObjectComponent(CGameObject& aParent)
	: CBehaviour(aParent)
	, myShrink(false)
{
}

CLoadObjectComponent::~CLoadObjectComponent()
{
}

void CLoadObjectComponent::Awake()
{
}

void CLoadObjectComponent::Start()
{
	int random = Random(0, 1);
	if (random == 1) {
		myShrink = true;
	}
	else {
		myShrink = false;
	}
}

void CLoadObjectComponent::Update()
{
	float scale = GameObject().GetComponent<CTransformComponent>()->Scale();
	if (myShrink == false && scale > 1.15f) {
		myShrink = true;
	}
	else if (myShrink == true && scale < 0.85f) {
		myShrink = false;
	}
	if (myShrink) {
		scale -= 0.03f * CTimer::Dt();
		GameObject().GetComponent<CTransformComponent>()->Scale(scale);
	} else {
		scale += 0.03f * CTimer::Dt();
		GameObject().GetComponent<CTransformComponent>()->Scale(scale);
	}
	
}

void CLoadObjectComponent::OnEnable()
{
}

void CLoadObjectComponent::OnDisable()
{
}
