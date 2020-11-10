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
{}

void CAnimationComponent::Start()
{}

void CAnimationComponent::Update()
{
	float dt = CTimer::Dt();
	for (CAnimation* anim : GetParent().GetComponent<CModelComponent>()->GetMyModel()->GetAnimations())
	{
		anim->Step(dt);
	}

	for (int i = 0; i < 64; i++)
	{
		myBones[i].SetIdentity();
	}

	GetAnimatedTransforms(dt, myBones.data());	
}

void CAnimationComponent::OnEnable()
{

}

void CAnimationComponent::OnDisable()
{

}

void CAnimationComponent::GetAnimatedTransforms(float dt, SlimMatrix44 * transforms)
{
	dt;
	CModel* model = GetParent().GetComponent<CModelComponent>()->GetMyModel();
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
