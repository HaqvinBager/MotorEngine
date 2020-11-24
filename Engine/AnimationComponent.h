#pragma once
#include "Behaviour.h"

#include "SimpleMath.h"
#include "ModelMath.h"

// REMINDER: CANIMATION IS UNUSED, CMODELCOMPONENT HAS ITS OWN CANIMATION THAT IT USES

//#define USING_BLENDED_ANIMATIONS

/// <To do>
/// Model Viewer part complete.
/// 1 - Can stop looping animation using myIsLooping = false
/// 2 - Using string IDs to load animations map<int,int> <string id, index to my animation>
/// </To do>

struct SAnimationBlend
{
	int myFirst = -1;
	int mySecond = -1;
	float myBlendLerp = 0;
};

class CGameObject;
class CAnimation;
class CAnimationComponent : public CBehaviour
{
public:
	CAnimationComponent(CGameObject& aParent, const std::string& aModelFilePath, std::vector<std::string>& someAnimationPaths);
	~CAnimationComponent() override;

	void Awake() override;
	void Start() override;
	void Update() override;

	void OnEnable() override;
	void OnDisable() override;

public:
	CAnimation* GetMyAnimation() { return myAnimation; }
	const float GetBlend() const { return myBlend.myBlendLerp; }

public:
	std::array<SlimMatrix44, 64> GetBones() { return myBones; }
	void GetAnimatedBlendTransforms(float dt, SlimMatrix44* transforms);
	void GetAnimatedTransforms(float dt, SlimMatrix44* transforms);
	void SetBlend(int anAnimationIndex, int anAnimationIndexTwo, float aBlend);
	void PlayAnimation(const int anAnimationIndex, bool anIsLooping = false);

public:
	//Intended use is in Model viewer. Careful using it anywhere else. Deletes the previous myAnimation, news and inits a new CAnimation.
	bool ReplaceAnimation(const char* aRig, std::vector<std::string>& somePathsToAnimations);
	
private:
	void SetBonesToIdentity();
	void UpdateBlended(const float dt);
	void UpdateNonBlended(const float dt);

	CAnimation* myAnimation;
	std::array<SlimMatrix44, 64> myBones { };
	SAnimationBlend myBlend;
	bool myIsLooping ;


};