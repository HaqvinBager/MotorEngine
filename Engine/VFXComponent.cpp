#include "stdafx.h"
#include "VFXComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

namespace SM = DirectX::SimpleMath;
#define ENGINE_SCALE 0.01f

CVFXComponent::CVFXComponent(CGameObject& aParent): CComponent(aParent), myVFXBase(nullptr) {
	SetScale(1.0f);
	myTransform.Translation(GameObject().myTransform->Position());
}

CVFXComponent::~CVFXComponent() {
    myVFXBase = nullptr;
}

void CVFXComponent::Awake() {
}

void CVFXComponent::Start() {
}

void CVFXComponent::Update() {

	myTextureScroll += {0.15f * CTimer::Dt(), 0.15f * CTimer::Dt()};
	myTextureScroll2 += {0.15f * CTimer::Dt(), 0.15f * CTimer::Dt()};
	SetPosition(GameObject().myTransform->Position());
}

bool CVFXComponent::Init(CVFXBase* aVFXBase) {
	myVFXBase = aVFXBase;
	if (!myVFXBase) {
		return false;
	}

	return true;
}

void CVFXComponent::SetTransform(DirectX::SimpleMath::Vector3 aPosition, DirectX::SimpleMath::Vector3 aRotation) {
    SetRotation(aRotation);
    SetPosition(aPosition);
}

void CVFXComponent::SetRotation(DirectX::SimpleMath::Vector3 aRotation) {
	SM::Vector3 translation = myTransform.Translation();
	myTransform = SM::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(aRotation.y),
		DirectX::XMConvertToRadians(aRotation.x),
		DirectX::XMConvertToRadians(aRotation.z)
	);
	myTransform *= SM::Matrix::CreateScale(myScale * ENGINE_SCALE);
	myTransform.Translation(translation);
}

void CVFXComponent::SetPosition(DirectX::SimpleMath::Vector3 aPosition) {
	myTransform.Translation(aPosition);
}

void CVFXComponent::SetScale(float aScale) {
	myScale = aScale;
	SM::Vector3 scale;
	SM::Vector3 translation;
	SM::Quaternion rotation;
	myTransform.Decompose(scale, rotation, translation);
	myTransform = SM::Matrix::CreateFromQuaternion(rotation);
	myTransform *= SM::Matrix::CreateScale(myScale * ENGINE_SCALE);
	myTransform.Translation(translation);
}

void CVFXComponent::Move(DirectX::SimpleMath::Vector3 aMovement) {
	myTransform.Translation(myTransform.Translation() + myTransform.Right() * aMovement.x);
	myTransform.Translation(myTransform.Translation() + myTransform.Up() * aMovement.y);
	myTransform.Translation(myTransform.Translation() - myTransform.Forward() * aMovement.z);
}

void CVFXComponent::Rotate(DirectX::SimpleMath::Vector3 aRotation) {
	SM::Vector3 translation = myTransform.Translation();
	myTransform *= SM::Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
	myTransform.Translation(translation);
}

void CVFXComponent::Scroll(DirectX::SimpleMath::Vector2 aScrollVector1, DirectX::SimpleMath::Vector2 aScrollVector2) {
	myTextureScroll = aScrollVector1;
	myTextureScroll2 = aScrollVector2;
}
