#include "stdafx.h"
#include "MenuState.h"
#include "SpriteInstance.h"
#include "SpriteFactory.h"
#include "Scene.h"
#include "CameraFactory.h"

CMenuState::CMenuState(CStateStack& aStateStack) : CState(aStateStack) {
	myBackground = new CSpriteInstance();
	myBackground->Init(CSpriteFactory::GetInstance()->GetSprite("MenuBackground.dds"));
	myBackground->SetSize({ 1.8f,1.0f });
	//CScene::GetInstance()->AddInstance(myBackground);
}

CMenuState::~CMenuState() {
}

void CMenuState::Awake() {

	
}

void CMenuState::Start() {
}

void CMenuState::Update() {
}
