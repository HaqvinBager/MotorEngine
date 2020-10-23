#include "stdafx.h"
#include "VFXInstance.h"

namespace SM = DirectX::SimpleMath;

CVFXInstance::CVFXInstance() : myVFXBase(nullptr) {
}

CVFXInstance::~CVFXInstance() {
	myVFXBase = nullptr;
}

bool CVFXInstance::Init(CVFXBase* aVFXBase) {
	myVFXBase = aVFXBase;
	if (!myVFXBase) {
		return false;
	}

	return true;
}

void CVFXInstance::SetTransform(DirectX::SimpleMath::Vector3 aPosition, DirectX::SimpleMath::Vector3 aRotation) {
	SetRotation(aRotation);
	SetPosition(aPosition);
}

void CVFXInstance::SetRotation(DirectX::SimpleMath::Vector3 aRotation) {
	SM::Vector3 translation = myTransform.Translation();
	myTransform = SM::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(aRotation.y),
		DirectX::XMConvertToRadians(aRotation.x),
		DirectX::XMConvertToRadians(aRotation.z)
	);
	myTransform *= SM::Matrix::CreateScale(myScale);
	myTransform.Translation(translation);
}

void CVFXInstance::SetPosition(DirectX::SimpleMath::Vector3 aPosition) {
	myTransform.Translation(aPosition);
}

void CVFXInstance::SetScale(float aScale) {
	myScale = aScale;
	myTransform *= SM::Matrix::CreateScale(myScale);
}

void CVFXInstance::Move(DirectX::SimpleMath::Vector3 aMovement) {
	myTransform.Translation(myTransform.Translation() + myTransform.Right() * aMovement.x);
	myTransform.Translation(myTransform.Translation() + myTransform.Up() * aMovement.y);
	myTransform.Translation(myTransform.Translation() - myTransform.Forward() * aMovement.z);
}

void CVFXInstance::Rotate(DirectX::SimpleMath::Vector3 aRotation) {
	SM::Vector3 translation = myTransform.Translation();
	myTransform *= SM::Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
	myTransform.Translation(translation);
}

void CVFXInstance::Scroll(DirectX::SimpleMath::Vector2 aScrollVector1, DirectX::SimpleMath::Vector2 aScrollVector2) {
	myTextureScroll = aScrollVector1;
	myTextureScroll2 = aScrollVector2;
}
