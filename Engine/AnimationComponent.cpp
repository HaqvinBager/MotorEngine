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
	myAnimation->BoneTransforms(transforms, myAnimationSpeed);
}

void CAnimationComponent::PlayAnimation(const int anAnimationIndex, bool anIsLooping, const float anAnimSpeed)
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

void CAnimationComponent::PlayAnimation(const EPlayerAnimationID anAnimationID, bool anIsLooping, const float anAnimSpeed)
{
	int id = static_cast<int>(anAnimationID);
	if (HasID(id))
	{
		PlayAnimation(GetIndexFromList(id), anIsLooping, anAnimSpeed);
	}
}

void CAnimationComponent::PlayAnimation(const EEnemyAnimationID anAnimationID, bool anIsLooping, const float anAnimSpeed)
{
	int id = static_cast<int>(anAnimationID);
	if (HasID(id))
	{
		PlayAnimation(GetIndexFromList(id), anIsLooping, anAnimSpeed);
	}
}

void CAnimationComponent::PlayAnimation(const EBossAnimationID anAnimationID, bool anIsLooping, const float anAnimSpeed)
{
	int id = static_cast<int>(anAnimationID);
	if (HasID(id))
	{
		PlayAnimation(GetIndexFromList(id), anIsLooping, anAnimSpeed);
	}
}

void CAnimationComponent::PlayAnimation(const ECrateAnimationID anAnimationID, bool anIsLooping, const float anAnimSpeed)
{
	int id = static_cast<int>(anAnimationID);
	if (HasID(id))
	{
		PlayAnimation(GetIndexFromList(id), anIsLooping, anAnimSpeed);
	}
}

void CAnimationComponent::SetBlend(int anAnimationIndex, int anAnimationIndexTwo, float aBlend)
{
	myBlend.myFirst		= anAnimationIndex;
	myBlend.mySecond	= anAnimationIndexTwo;
	myBlend.myBlendLerp = aBlend;
}

bool CAnimationComponent::WithinIDRange(const int anID)
{
	return (anID <= myAnimationIds.back().ID() && anID >= myAnimationIds.front().ID()); 
}

bool CAnimationComponent::HasID(const int anID)
{
	for (auto& id : myAnimationIds)
	{
		if (id.ID() == anID)
		{
			return true;
		}
	}
	return false;
}

const int CAnimationComponent::GetIndexFromID(const int anID)
{
	int index = abs(myAnimationIds[0].ID() - anID ) + 1;
	return index;
}

const int CAnimationComponent::GetIndexFromList(const int anID)
{
	for (int i = 0; i < myAnimationIds.size(); ++i)
	{
		if (myAnimationIds[i].ID() == anID)
		{
			return i + 1; // Animations inside CAnimation start at 1, 0 is a static civillian/ tpose.
		}
	}
	return 0;
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


