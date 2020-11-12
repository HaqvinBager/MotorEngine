#include "stdafx.h"
#include "Animation.h"
#include "AnimationController.h"


CAnimation::~CAnimation()
{
	delete myController;
	myController = nullptr;
}

void CAnimation::Init(const char* aRig, std::vector<std::string>& somePathsToAnimations)
{
	myController = new AnimationController(aRig);
	myController->Import3DFromFile(aRig);
	for (std::string s : somePathsToAnimations)
	{
		myController->Add3DAnimFromFile(s);
	}

	myController->SetAnimIndex(1, true, 0.0f);
	myController->SetAnimIndex(2, true, 5.0f);
}
void CAnimation::BoneTransformWithBlend(SlimMatrix44* Transforms, float aBlendFactor)
{
	std::vector<aiMatrix4x4> trans;
	myController->BoneTransform(trans);
	myController->SetBlendTime(aBlendFactor);

	memcpy(&Transforms[0], &trans[0], (sizeof(float) * 16) * trans.size());
}

void CAnimation::Step(float aDelta)
{
	// commented 2020 11 12 - The if check is used for nothing. The commented contents were already not in use.
	//if (myController->IsDoneBlending())
	//{
	//	//myController->SetAnimIndex();
	//	//myController->SetAnimIndex(2, true, 5);
	//}

	myTotalAnimationTime += aDelta;
	myController->Update();
}

const size_t CAnimation::GetNrOfAnimations() const
{
	return myController->GetNrOfAnimations(); 
}