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
	, myAttack01ID(0)
	, myAttack02ID(0)
	, myAbility01ID(0)
	, myAbility02ID(0)
	, myAbility03ID(0)
	, myAbility04ID(0)
	, myAbility05ID(0)
	, myAbility06ID(0)
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
		PlayAnimation(myMovingAnimationID, true);
	}
	ResetIdleTimer();
}

bool CAnimationComponent::IsUsingAbility()
{
	return 
		(  
		   myCurrentAnimationID == myAttack01ID
		|| myCurrentAnimationID == myAttack02ID
		|| myCurrentAnimationID == myAbility01ID
		|| myCurrentAnimationID == myAbility02ID
		|| myCurrentAnimationID == myAbility03ID
		|| myCurrentAnimationID == myAbility04ID
		|| myCurrentAnimationID == myAbility05ID
		|| myCurrentAnimationID == myAbility06ID
		);
}

bool CAnimationComponent::DeadState()
{
	if (myCurrentAnimationID == myDyingAnimationID)
		return false;

	PlayAnimation(myDyingAnimationID);
	myAnimation->GetMyController().ResetAnimationTimeCurrent();
	myAnimation->GetMyController().SetLoopingSceneIndex(0);

	return false;//myAnimation->GetMyController().CurrentAnimationDuration() > 0.0f;
}

void CAnimationComponent::ModelViewerPlayAnimation(const int anAnimationIndex, bool anIsLooping, const float anAnimSpeed)
{
	myAnimationSpeed = anAnimSpeed;
	myIsLooping = anIsLooping;
	if (anIsLooping)
	{
		myAnimation->GetMyController().SetLoopingSceneIndex(anAnimationIndex);
	}
	else
	{
		myAnimation->GetMyController().ResetAnimationTimeCurrent();
	}
	myAnimation->SetCurAnimationScene(anAnimationIndex);
}

void CAnimationComponent::ForceToIdleState()
{ 
	myCurrentAnimationID = myIdleAnimationID; 
}
// Temporary for project 6
void CAnimationComponent::PlayAttack01ID() { PlayAnimation(myAttack01ID); }
void CAnimationComponent::PlayAttack02ID() { PlayAnimation(myAttack02ID); }
void CAnimationComponent::PlayAbility01ID(){ PlayAnimation(myAbility01ID); }
void CAnimationComponent::PlayAbility02ID(){ PlayAnimation(myAbility02ID); }
void CAnimationComponent::PlayAbility03ID(){ PlayAnimation(myAbility03ID); }
void CAnimationComponent::PlayAbility04ID(){ PlayAnimation(myAbility04ID); }
void CAnimationComponent::PlayAbility05ID(){ PlayAnimation(myAbility05ID); }
void CAnimationComponent::PlayAbility06ID(){ PlayAnimation(myAbility06ID); }

// Temporary for project 6
void CAnimationComponent::SetIdleID(const int anIdleID)
{
	myIdleAnimationID = anIdleID;
}
void CAnimationComponent::SetMovingID(const int aMovingID)
{
	myMovingAnimationID = aMovingID;
}
void CAnimationComponent::SetDyingID(const int aDyingID)
{
	myDyingAnimationID = aDyingID;
}
void CAnimationComponent::SetAttack01ID(const int aID){ myAttack01ID = aID; }
void CAnimationComponent::SetAttack02ID(const int aID) { myAttack02ID = aID;  }
void CAnimationComponent::SetAbility01ID(const int aID){ myAbility01ID = aID; }
void CAnimationComponent::SetAbility02ID(const int aID){ myAbility02ID = aID; }
void CAnimationComponent::SetAbility03ID(const int aID){ myAbility03ID = aID; }
void CAnimationComponent::SetAbility04ID(const int aID){ myAbility04ID = aID; }
void CAnimationComponent::SetAbility05ID(const int aID){ myAbility05ID = aID; }
void CAnimationComponent::SetAbility06ID(const int aID){ myAbility06ID = aID; }

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
	if (myIdleAnimationID == 0)
		return; 

	if (myCurrentAnimationID != myDyingAnimationID)
	{
		PlayAnimation(myIdleAnimationID, true);
	}
}

void CAnimationComponent::PlayAnimation(const int anAnimationID, bool anIsLooping, const float anAnimSpeed)
{
	if (myCurrentAnimationID == myDyingAnimationID)
		return;

	myAnimationSpeed = anAnimSpeed;
	myIsLooping = anIsLooping;
	if (anIsLooping)
	{
		myAnimation->GetMyController().SetLoopingSceneIndex(GetIndexFromList(anAnimationID));
	}
	else
	{
		myAnimation->GetMyController().SetLoopingSceneIndex(GetIndexFromList(myIdleAnimationID));

		if (myCurrentAnimationID != anAnimationID)
			myAnimation->GetMyController().ResetAnimationTimeCurrent();
	}
	myAnimation->SetCurAnimationScene(GetIndexFromList(anAnimationID));
	myReturnToIdleTimer = myAnimation->GetMyController().CurrentAnimationDuration() / myAnimation->GetMyController().CurrentAnimationTicksPerSecond();//23.0f;// AnimationController has it set to 24 ticks per seconds -> 24fps 
	myCurrentAnimationID = anAnimationID;
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