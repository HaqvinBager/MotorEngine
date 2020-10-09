#include "stdafx.h"
#include "ModelInstance.h"
#include "Animation.h"
#include "Model.h"

using namespace DirectX::SimpleMath;

#define ENGINE_SCALE 0.01f

CModelInstance::CModelInstance() {
	myModel = nullptr;
}

CModelInstance::~CModelInstance() {
	myModel = nullptr;
}

void CModelInstance::Init(CModel* aModel) {
	myModel = aModel;
	myTransform = Matrix();
}

void CModelInstance::SetTransform(Vector3 aPosition,Vector3 aRotation)
{
	SetRotation(aRotation);
	SetPosition(aPosition);
}

void CModelInstance::SetPosition(Vector3 aPosition)
{
	myTransform.Translation(aPosition);
}

void CModelInstance::SetRotation(Vector3 aRotation)
{
	Vector3 tempTranslation = myTransform.Translation();

	Matrix tempRotation = Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(aRotation.y),
		DirectX::XMConvertToRadians(aRotation.x),
		DirectX::XMConvertToRadians(aRotation.z)
	);
	myTransform = tempRotation;
	myTransform *= Matrix::CreateScale(ENGINE_SCALE);
	myTransform.Translation(tempTranslation);
}

void CModelInstance::Move(Vector3 aMovement)
{
	myTransform.Translation(myTransform.Translation() + myTransform.Right() * aMovement.x);
	myTransform.Translation(myTransform.Translation() + myTransform.Up() * aMovement.y);
	myTransform.Translation(myTransform.Translation() - myTransform.Forward() * aMovement.z);
}

void CModelInstance::Rotate(Vector3 aRotation)
{
	Vector3 tempTranslation = myTransform.Translation();
	Matrix tempRotation = Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
	myTransform *= tempRotation;
	myTransform.Translation(tempTranslation);
}

void CModelInstance::SetScale(Vector3 aScale)
{
	myScale = aScale;
	myTransform *= Matrix::CreateScale(ENGINE_SCALE);

}

void CModelInstance::UpdateAnimation(float dt)
{
	for (CAnimation* anim : myModel->GetAnimations())
	{
		anim->Step(dt);
	}

	for (int i = 0; i < 64; i++)
	{
		myBones[i].SetIdentity();
	}

	GetAnimatedTransforms(dt, myBones.data());	
}

void CModelInstance::GetAnimatedTransforms(float dt, SlimMatrix44* transforms)
{
	dt;
	if(myModel->GetAnimations().size() > 0)
	{
		CAnimation* first = myModel->GetAnimations()[0];
		first->BoneTransformWithBlend(transforms, myBlend.myBlendLerp);
	}
}

void CModelInstance::SetBlend(int anAnimationIndex, int anAnimationIndexTwo, float aBlend)
{
	myBlend.myFirst = anAnimationIndex;
	myBlend.mySecond = anAnimationIndexTwo;
	myBlend.myBlendLerp = aBlend;
}

