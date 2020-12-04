#include "stdafx.h"
#include "AnimationComponent.h"

#include "GameObject.h"
#include "ModelComponent.h"
#include "Model.h"
#include "Animation.h"
#include "AnimationController.h"//TEMP
#include "Timer.h"

CAnimationComponent::CAnimationComponent(CGameObject& aParent, const std::string& aModelFilePath, std::vector<std::string>& someAnimationPaths)
	: CBehaviour(aParent)
	, myAnimationSpeed(1.0f)
	, myIsLooping(false)
	, myReturnToIdleTimer(-1.0f)
	, myCurrentAnimationID(0)
	, myMovingAnimationID(0)
	, myDyingAnimationID(0)
{
	myAnimation = new CAnimation();
	myAnimation->Init(aModelFilePath.c_str(), someAnimationPaths);
	myAnimationIds.reserve(someAnimationPaths.size());
	for (auto& str : someAnimationPaths)
	{
		myAnimationIds.emplace_back(CStringID(str, CStringIDLoader::EStringIDFiles::AnimFile));
	}
	myAnimationIds.shrink_to_fit();
}

CAnimationComponent::~CAnimationComponent()
{
	delete myAnimation;
	myAnimation = nullptr;
}
#include <iostream>
void CAnimationComponent::Awake()
{
#ifdef _DEBUG
	//for (auto& strID : myAnimationIds)
	//{
	//	std::cout << __FUNCTION__ << "  " << strID.String() << " " << strID.ID() << std::endl;
	//}
#endif
	SetBonesToIdentity();
	myAnimation->SetCurAnimationScene(0);
}

void CAnimationComponent::Start()
{
	myCurrentAnimationID = myIdleAnimationID;
	SwitchBackToIdle();
}

void CAnimationComponent::Update()
{
	float dt = CTimer::Dt();

#ifdef USING_BLENDED_ANIMATIONS
	UpdateBlended(dt);
#else
	UpdateNonBlended(dt);
#endif

	if (UpdateIdleTimer())
	{
		SwitchBackToIdle();
	}
}

void CAnimationComponent::OnEnable()
{}
void CAnimationComponent::OnDisable()
{}

void CAnimationComponent::GetAnimatedBlendTransforms(float dt, SlimMatrix44 * transforms)
{
	dt;
	myAnimation->BoneTransformsWithBlend(transforms, myBlend.myBlendLerp);
}
void CAnimationComponent::GetAnimatedTransforms(float dt, SlimMatrix44 * transforms)
{
	dt;
	myAnimation->BoneTransforms(transforms, myAnimationSpeed);
}

void CAnimationComponent::MovingState()
{
	if (myCurrentAnimationID == myDyingAnimationID)
		return;
	if (myCurrentAnimationID != myIdleAnimationID)
		return;
	if (myCurrentAnimationID != myMovingAnimationID)
	{
		PlayAnimation(GetIndexFromList(myMovingAnimationID), true);
		myCurrentAnimationID = myMovingAnimationID;
	}
	ResetIdleTimer();
}

void CAnimationComponent::DeadState()
{
	PlayAnimation(GetIndexFromList(myDyingAnimationID));
	myCurrentAnimationID = myDyingAnimationID;
	myAnimation->GetMyController().ResetAnimationTimeCurrent();
	myAnimation->GetMyController().SetLoopingSceneIndex(0);
	//myAnimation->GetMyController().SetLoopingSceneIndex(myDyingAnimationID);
}

void CAnimationComponent::ForceToIdleState()
{
	myCurrentAnimationID = myIdleAnimationID;
}

void CAnimationComponent::SetBlend(int anAnimationIndex, int anAnimationIndexTwo, float aBlend)
{
	myBlend.myFirst		= anAnimationIndex;
	myBlend.mySecond	= anAnimationIndexTwo;
	myBlend.myBlendLerp = aBlend;
}

bool CAnimationComponent::UpdateIdleTimer()
{
	if (myCurrentAnimationID == myIdleAnimationID)
		return false;

	myReturnToIdleTimer -= CTimer::Dt();
	if (myReturnToIdleTimer <= 0.0f)
		return true;
	return false;
}

void CAnimationComponent::ResetIdleTimer()
{
	myReturnToIdleTimer = CTimer::Dt() * 3.0f;
}

void CAnimationComponent::SwitchBackToIdle()
{
	if (myCurrentAnimationID != myDyingAnimationID)
	{
		PlayAnimation(GetIndexFromList(myIdleAnimationID), true);
		myCurrentAnimationID = myIdleAnimationID;
	}
}

void CAnimationComponent::PlayAnimation(const int anAnimationIndex, bool anIsLooping, const float anAnimSpeed)
{
	if (myCurrentAnimationID == myDyingAnimationID)
		return;

	myAnimationSpeed = anAnimSpeed;
	myIsLooping = anIsLooping;
	if (anIsLooping)
	{
		myAnimation->GetMyController().SetLoopingSceneIndex(anAnimationIndex);
	}
	else
	{
		if(GetIndexFromList(myCurrentAnimationID) != anAnimationIndex)
			myAnimation->GetMyController().ResetAnimationTimeCurrent();
	}
	myAnimation->SetCurAnimationScene(anAnimationIndex);
	myReturnToIdleTimer = myAnimation->GetMyController().CurrentAnimationDuration() / 59.0f;
}

bool CAnimationComponent::ReplaceAnimation(const char* aRig, std::vector<std::string>& somePathsToAnimations)
{
	if (myAnimation != nullptr)
	{
		delete myAnimation;
		myAnimation = new CAnimation();

		myAnimation->Init(aRig, somePathsToAnimations);

		SetBonesToIdentity();
		myAnimation->SetCurAnimationScene(0);

		return true;
	}

	return false;
}

void CAnimationComponent::SetBonesToIdentity()
{
	for (int i = 0; i < 64; i++)
	{
		myBones[i].SetIdentity();
	}
}
void CAnimationComponent::UpdateBlended(const float dt)
{
	myAnimation->BlendStep(dt);
	SetBonesToIdentity();
	GetAnimatedBlendTransforms(dt, myBones.data());
}
void CAnimationComponent::UpdateNonBlended(const float dt)
{
	myAnimation->Step();
	SetBonesToIdentity();
	GetAnimatedTransforms(dt, myBones.data());
}




// Pre 2020 11 15 - Original Blended
/*
void CAnimationComponent::PlayAnimation(const int anAnimationIndex, bool anIsLooping)
{
	myIsLooping = anIsLooping;
	
	//CAnimation* anim = GameObject().GetComponent<CModelComponent>()->GetMyModel()->GetAnimations()[0];
	
	//anim->SetCurAnimationScene(anAnimationIndex);
	myAnimation->SetCurAnimationScene(anAnimationIndex);
}
void CAnimationComponent::GetAnimatedBlendTransforms(float dt, SlimMatrix44 * transforms)
{
	dt;
	myAnimation->BoneTransformsWithBlend(transforms, myBlend.myBlendLerp);
	//CModel* model = GameObject().GetComponent<CModelComponent>()->GetMyModel();
	//if(model->GetAnimations().size() > 0)
	//{
	//	CAnimation* first = model->GetAnimations()[0];
	//	first->BoneTransformsWithBlend(transforms, myBlend.myBlendLerp);
	//}
}
void CAnimationComponent::GetAnimatedTransforms(float dt, SlimMatrix44 * transforms)
{
	dt;
	myAnimation->BoneTransforms(transforms);
	//CModel* model = GameObject().GetComponent<CModelComponent>()->GetMyModel();
	//if(model->GetAnimations().size() > 0)
	//{
	//	CAnimation* first = model->GetAnimations()[0];
	//	first->BoneTransforms(transforms);
	//}
}
void CAnimationComponent::Update()
{
	float dt = CTimer::Dt();

	//USE FOR BLENDED ANIMATIONS

	//for (CAnimation* anim : GameObject().GetComponent<CModelComponent>()->GetMyModel()->GetAnimations())
	//{
	//	anim->BlendStep(dt);
	//}
	//
	//SetBonesToIdentity();
	//
	//GetAnimatedBlendTransforms(dt, myBones.data());


	//-----------

	//for (CAnimation* anim : GameObject().GetComponent<CModelComponent>()->GetMyModel()->GetAnimations())
	//{
	//	anim->Step(dt);
	//}

	myAnimation->Step(dt);
	SetBonesToIdentity();

	GetAnimatedTransforms(dt, myBones.data());
}
*/