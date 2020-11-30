#pragma once
#include "Behaviour.h"
#include "StringID.hpp"
#include "animationEnums.h"

#include "SimpleMath.h"
#include "ModelMath.h"

//#define USING_BLENDED_ANIMATIONS

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

	//Used mainly for Modelviewer. Please use PlayAnimation that uses a EAnimationID
	void PlayAnimation(const int anAnimationIndex, bool anIsLooping = false, const float anAnimSpeed = 1.0f);

	void PlayAnimation(const EPlayerAnimationID anAnimationID, bool anIsLooping = false, const float anAnimSpeed = 1.0f);
	void PlayAnimation(const EEnemyAnimationID anAnimationID, bool anIsLooping = false, const float anAnimSpeed = 1.0f);
	void PlayAnimation(const EBossAnimationID anAnimationID, bool anIsLooping = false, const float anAnimSpeed = 1.0f);
	void PlayAnimation(const ECrateAnimationID anAnimationID, bool anIsLooping = false, const float anAnimSpeed = 1.0f);

	// 2020 11 26
	// Didnt work properly 
	//template <class T>
	//void PlayAnimationUsingID(const T anEAnimationID, bool anIsLooping = false);

public:
	//Intended use is in Model viewer. Careful using it anywhere else. Deletes the previous myAnimation, news and inits a new CAnimation.
	bool ReplaceAnimation(const char* aRig, std::vector<std::string>& somePathsToAnimations);

private:
	void SetBonesToIdentity();
	void UpdateBlended(const float dt);
	void UpdateNonBlended(const float dt);
	void SetBlend(int anAnimationIndex, int anAnimationIndexTwo, float aBlend);// Does nothing atm.
	
	// This works if the ids are in a sorted list. But if animations are added at random to the project the ids dont follow eachother. Cant do a bounds check.
	bool WithinIDRange(const int anID);
	bool HasID(const int anID);

	const int GetIndexFromID(const int anID);

	CAnimation* myAnimation;
	std::array<SlimMatrix44, 64> myBones { };
	SAnimationBlend myBlend;
	bool myIsLooping;
	float myAnimationSpeed;
	std::vector<CStringID> myAnimationIds;

};

// 2020 11 26
// Doesn't seem safe to use. High potential for weird errors.
//template <class T>
//void CAnimationComponent::PlayAnimationUsingID(const T anEAnimationID, bool anIsLooping)
//{
//	int id = static_cast<int>(anEAnimationID);
//	if (WithinIDRange(id))
//	{
//		PlayAnimation(GetIndexFromID(id), anIsLooping);
//	}
//}