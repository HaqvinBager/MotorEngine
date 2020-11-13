#include "stdafx.h"
#include "AnimationComponent.h"

#include "GameObject.h"
#include "ModelComponent.h"
#include "Model.h"
#include "Animation.h"
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
}

void CAnimationComponent::Start()
{}

void CAnimationComponent::Update()
{
	float dt = CTimer::Dt();

	//USE FOR BLENDED ANIMATIONS

	/*for (CAnimation* anim : GameObject().GetComponent<CModelComponent>()->GetMyModel()->GetAnimations())
	{
		anim->BlendStep(dt);
	}

	SetBonesToIdentity();

	GetAnimatedBlendTransforms(dt, myBones.data());	*/


	//-----------

	for (CAnimation* anim : GameObject().GetComponent<CModelComponent>()->GetMyModel()->GetAnimations())
	{
		anim->Step(dt);
	}

	SetBonesToIdentity();

	GetAnimatedTransforms(dt, myBones.data());

}

void CAnimationComponent::OnEnable()
{}
void CAnimationComponent::OnDisable()
{}

void CAnimationComponent::GetAnimatedBlendTransforms(float dt, SlimMatrix44 * transforms)
{
	dt;
	CModel* model = GameObject().GetComponent<CModelComponent>()->GetMyModel();
	if(model->GetAnimations().size() > 0)
	{
		CAnimation* first = model->GetAnimations()[0];
		first->BoneTransformsWithBlend(transforms, myBlend.myBlendLerp);
	}
}
void CAnimationComponent::GetAnimatedTransforms(float dt, SlimMatrix44 * transforms)
{
	dt;
	CModel* model = GameObject().GetComponent<CModelComponent>()->GetMyModel();
	if(model->GetAnimations().size() > 0)
	{
		CAnimation* first = model->GetAnimations()[0];
		first->BoneTransforms(transforms);
	}
}

void CAnimationComponent::SetBlend(int anAnimationIndex, int anAnimationIndexTwo, float aBlend)
{
	myBlend.myFirst		= anAnimationIndex;
	myBlend.mySecond	= anAnimationIndexTwo;
	myBlend.myBlendLerp = aBlend;
}

void CAnimationComponent::PlayAnimation(const int anAnimationIndex, bool aIsLooping)
{
	myIsLooping = aIsLooping;

	CAnimation* anim = GameObject().GetComponent<CModelComponent>()->GetMyModel()->GetAnimations()[0];

	anim->SetCurAnimationScene(anAnimationIndex);
}

void CAnimationComponent::SetBonesToIdentity()
{
	for (int i = 0; i < 64; i++)
	{
		myBones[i].SetIdentity();
	}
}
