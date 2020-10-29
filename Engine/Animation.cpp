#include "stdafx.h"
#include "Animation.h"
#include "AnimationController.h"


CAnimation::~CAnimation()
{
	delete controller;
	controller = nullptr;
	myActiveAnimations.clear();
}

void CAnimation::Init(const char* aRig, std::vector<std::string>& somePathsToAnimations)
{
	controller = new AnimationController(aRig);
	controller->Import3DFromFile(aRig);
	for (std::string s : somePathsToAnimations)
	{
		controller->Add3DAnimFromFile(s);
	}

	controller->SetAnimIndex(1, true, 0.0f);
	controller->SetAnimIndex(2, true, 5.0f);
}
//
//void CAnimation::BoneTransform(SlimMatrix44* Transforms)
//{
//
//}

void CAnimation::BoneTransformWithBlend(SlimMatrix44* Transforms, float aBlendFactor)
{
	std::vector<aiMatrix4x4> trans;
	controller->BoneTransform(trans);
	controller->SetBlendTime(aBlendFactor);

	memcpy(&Transforms[0], &trans[0], (sizeof(float) * 16) * trans.size());
}

void CAnimation::Step(float aDelta)
{
	if (controller->IsDoneBlending())
	{
		//controller->SetAnimIndex();
		//	controller->SetAnimIndex(2, true, 5);
	}

	myTotalAnimationTime += aDelta;
	//std::cout << myTotalAnimationTime << std::endl;
	controller->Update();
}

//void CAnimation::SetAnimator(SceneAnimator* anAnimator)
//{
//	myAnimator = anAnimator;
//}
//
//void CAnimation::SetBindPose(SceneAnimator* aBindPose)
//{
//	myBindPose = aBindPose;
//}
//
//void CAnimation::SetActiveAnimations(std::vector<int>& someActiveAnimations)
//{
//
//}
//
//void CAnimation::SetTotalAnimationTime(float aTotalAnimationTime)
//{
//}
//
//void CAnimation::SetAnimationTime(float anAnimationTime)
//{
//}
//
//void CAnimation::SetAnimationSpeed(float anAnimationSpeed)
//{
//}