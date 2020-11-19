#include "stdafx.h"
#include "AnimationComponent.h"

#include "GameObject.h"
#include "ModelComponent.h"
#include "Model.h"
#include "Animation.h"
#include "AnimationController.h"//TEMP
#include "Timer.h"

CAnimationComponent::CAnimationComponent(CGameObject& aParent) 
	: CBehaviour(aParent)
	, myAnimation(new CAnimation())
{}

CAnimationComponent::~CAnimationComponent()
{
	delete myAnimation;
	myAnimation = nullptr;
}

void CAnimationComponent::Awake()
{
	SetBonesToIdentity();
	myAnimation->SetCurAnimationScene(0);
}

void CAnimationComponent::Start()
{}

void CAnimationComponent::Update()
{
	float dt = CTimer::Dt();

#ifdef USING_BLENDED_ANIMATIONS
	UpdateBlended(dt);
#else
	UpdateNonBlended(dt);
#endif
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
	myAnimation->BoneTransforms(transforms);
}

void CAnimationComponent::SetBlend(int anAnimationIndex, int anAnimationIndexTwo, float aBlend)
{
	myBlend.myFirst		= anAnimationIndex;
	myBlend.mySecond	= anAnimationIndexTwo;
	myBlend.myBlendLerp = aBlend;
}

void CAnimationComponent::PlayAnimation(const int anAnimationIndex, bool anIsLooping)
{

	///
	/// myCurSceneIndex
	/// myLoopingSceneIndex
	/// 
	/// if anIslooping == true
	///		myCurSceneIndex = anAnimationIndex.
	/// 	myLoopingSceneIndex = anAnimationIndex.
	/// else
	///		myCureSceneIndex = anAnimationIndex
	/// 
	/// Once non-looping anim is complete switch myCureSceneIndex = myLoopingSceneIndex
	/// 
	/// How do we check that an animation is complete?
	/// 


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

// Pre 2020 11 15
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