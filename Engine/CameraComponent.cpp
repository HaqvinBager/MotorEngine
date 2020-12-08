#include "stdafx.h"
#include "CameraComponent.h"
#include "RandomNumberGenerator.h"
#include "TransformComponent.h"

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
}

CCameraComponent::~CCameraComponent()
{
}

void CCameraComponent::Awake()
{
}

void CCameraComponent::Start()
{
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
}

void CCameraComponent::SetTrauma(float aValue)
{
	myTrauma = aValue;
}

void CCameraComponent::SetStartingRotation(DirectX::SimpleMath::Vector3 aRotation)
{
	myStartingRotation = aRotation;
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
