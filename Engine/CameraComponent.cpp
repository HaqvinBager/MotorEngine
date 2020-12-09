#include "stdafx.h"
#include "CameraComponent.h"
#include "RandomNumberGenerator.h"
#include "TransformComponent.h"
#include "SpriteInstance.h"

#include "Engine.h"
#include "Scene.h"
#include "PostMaster.h"
#include "MainSingleton.h"

CCameraComponent::CCameraComponent(CGameObject& aParent, const float aFoV/*, float aNearPlane, float aFarPlane, DirectX::SimpleMath::Vector2 aResolution*/)
	: CComponent(aParent)
{
	myProjection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(aFoV), (1600.0f / 900.0f), 10.0f, 10000.0f);
	myView = DirectX::XMMatrixLookAtLH(GameObject().myTransform->Position(), Vector3::Forward, Vector3::Up);

	myTrauma = 0.0f;
	myShake = 0.0f;
	myDecayInSeconds = 1.0f;
	myShakeSpeed = 15.0f;
	myMaxShakeRotation = { 2.0f, 2.0f, 2.0f };
	myNoise = PerlinNoise(214125213);
	myShakeTimer = 0.0f;
	
	myFadingPlane = nullptr;
	myFadeTimer = 1.0f;
	myFadeSpeed = 0.75f;
	myFadingIn = true;
	myFadingPlaneActive = false;
}

CCameraComponent::~CCameraComponent()
{
	delete myFadingPlane;
	myFadingPlane = nullptr;
}

void CCameraComponent::Awake()
{
	myFadingPlane = new CSpriteInstance(/*CEngine::GetInstance()->GetActiveScene()*/);
	myFadingPlane->Init(CSpriteFactory::GetInstance()->GetSprite("Assets/3D/UI/Ingame/CameraFadeScreen.dds"));
	myFadingPlane->SetSize({ 15.1f, 8.5f });
	myFadingPlane->SetRenderOrder(ERenderOrder::PauseLayer);
	myFadingPlane->SetShouldRender(false);
}

void CCameraComponent::Start()
{
}

float LogEaseOut(float x) {
	return 1 - (log(x) / log(0.01f));
}

float LogEaseIn(float x) {
	return (log(x) / log(0.01f));
}

void CCameraComponent::Update()
{
	if (myTrauma > 0.0f) {
		myShakeTimer += CTimer::Dt();
		myTrauma -= (1 / myDecayInSeconds) * CTimer::Dt();
		Shake();
	}

	if (myTrauma < 0.0f) {
		myTrauma = 0.0f;
	}

	if (myFadingPlaneActive) {
		
		if (!myFadingPlane->GetShouldRender())
			myFadingPlane->SetShouldRender(true);

		myFadeTimer -= myFadeSpeed * CTimer::Dt();
		if (myFadingIn) {
			DirectX::SimpleMath::Vector4 color = myFadingPlane->GetColor();
			float alpha = color.w;
			alpha = LogEaseOut(myFadeTimer);
			if (alpha <= 0.01f) {
				alpha = 0.0f;
				myFadingPlaneActive = false;
				myFadeTimer = 1.0f;
				myFadingPlane->SetShouldRender(false);
				CMainSingleton::PostMaster().Send({ EMessageType::FadeInComplete, 0 });
			}
			myFadingPlane->SetColor({ color.x, color.y, color.z, alpha });
		}
		else {
			DirectX::SimpleMath::Vector4 color = myFadingPlane->GetColor();
			float alpha = color.w;
			alpha = LogEaseIn(myFadeTimer);
			if (alpha >= 1.0f) {
				alpha = 1.0f;
				myFadingPlaneActive = false;
				myFadeTimer = 1.0f;
				myFadingPlane->SetShouldRender(false);
				CMainSingleton::PostMaster().Send({ EMessageType::FadeOutComplete, 0 });
			}
			myFadingPlane->SetColor({ color.x, color.y, color.z, alpha });
		}
	}
}

void CCameraComponent::SetTrauma(float aValue)
{
	myTrauma = aValue;
}

void CCameraComponent::SetStartingRotation(DirectX::SimpleMath::Vector3 aRotation)
{
	myStartingRotation = aRotation;
}

void CCameraComponent::Fade(bool aShouldFadeIn)
{
	myFadingPlaneActive = true;
	myFadingIn = aShouldFadeIn;
}

const bool CCameraComponent::IsFading() const
{
	return myFadingPlaneActive;
}

void CCameraComponent::EmplaceSprites(std::vector<CSpriteInstance*>& someSprites) const
{
	if (myFadingPlaneActive)
	{
		someSprites.emplace_back(myFadingPlane);
	}
}

const Matrix& CCameraComponent::GetViewMatrix()
{
	myView = DirectX::XMMatrixLookAtLH(GameObject().myTransform->Position(), GameObject().myTransform->Position() - GameObject().myTransform->Transform().Forward(), GameObject().myTransform->Transform().Up());
	return myView;
}

void CCameraComponent::Shake()
{
	myShake = myTrauma * myTrauma * myTrauma;

	if (myShakeTimer > 0.0167f) {
		float newRotX = myMaxShakeRotation.x * myShake * (float(myNoise.noise(Random(-myShakeSpeed, myShakeSpeed), CTimer::Time() * myShakeSpeed, 0.0f) - 0.5f)) * 2;
		float newRotY = myMaxShakeRotation.y * myShake * (float(myNoise.noise(Random(-myShakeSpeed, myShakeSpeed), CTimer::Time() * myShakeSpeed, 0.0f) - 0.5f)) * 2;
		float newRotZ = myMaxShakeRotation.z * myShake * (float(myNoise.noise(Random(-myShakeSpeed, myShakeSpeed), CTimer::Time() * myShakeSpeed, 0.0f) - 0.5f)) * 2;

		DirectX::SimpleMath::Vector3 newRotation = { newRotX, newRotY, newRotZ };
		newRotation += myStartingRotation;
		GameObject().myTransform->Rotation(newRotation);
		myShakeTimer -= 0.0167f;
	}
}
