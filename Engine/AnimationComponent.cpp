#include "stdafx.h"
#include "AnimationComponent.h"

#include "GameObject.h"
#include "ModelComponent.h"
#include "Model.h"
#include "ModelInstance.h"
#include "Animation.h"
#include "Timer.h"

CAnimationComponent::CAnimationComponent(CGameObject& aParent) 
	: CComponent(aParent)
	, myAnimation(new CAnimation())
{}

CAnimationComponent::~CAnimationComponent()
{
	delete myAnimation;
	myAnimation = nullptr;
}

void CAnimationComponent::Awake()
{}

void CAnimationComponent::Start()
{}

void CAnimationComponent::Update()
{
	float dt = CTimer::Dt();
	for (CAnimation* anim : GetParent().GetComponent<CModelComponent>()->GetMyModel()->GetModel()->GetAnimations())
	{
		anim->Step(dt);
	}

	for (int i = 0; i < 64; i++)
	{
		myBones[i].SetIdentity();
	}

	GetAnimatedTransforms(dt, myBones.data());	
}

void CAnimationComponent::GetAnimatedTransforms(float dt, SlimMatrix44 * transforms)
{
	dt;
	CModel* model = GetParent().GetComponent<CModelComponent>()->GetMyModel()->GetModel();
	if(model->GetAnimations().size() > 0)
	{
		CAnimation* first = model->GetAnimations()[0];
		first->BoneTransformWithBlend(transforms, myBlend.myBlendLerp);
	}
}

void CAnimationComponent::SetBlend(int anAnimationIndex, int anAnimationIndexTwo, float aBlend)
{
	myBlend.myFirst = anAnimationIndex;
	myBlend.mySecond = anAnimationIndexTwo;
	myBlend.myBlendLerp = aBlend;
}
