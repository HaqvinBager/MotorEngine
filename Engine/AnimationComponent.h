#pragma once
#include "Behaviour.h"
#include "StringID.hpp"
#include "animationEnums.h"

#include "SimpleMath.h"
#include "ModelMath.h"

//#define USING_BLENDED_ANIMATIONS // Don't toggle this. There is no support for it atm. Anything can happen if active.

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
	std::array<SlimMatrix44, 64> GetBones() { return myBones; }
	void GetAnimatedBlendTransforms(float dt, SlimMatrix44* transforms);
	void GetAnimatedTransforms(float dt, SlimMatrix44* transforms);

	void MovingState();
	bool DeadState();// After calling this no other animation will be able to be called.
	void ForceToIdleState();

	// Temporary for project 6
	void PlayAttack01ID();
	void PlayAttack02ID();
	void PlayAbility01ID();
	void PlayAbility02ID();
	void PlayAbility03ID();
	void PlayAbility04ID();
	void PlayAbility05ID();
	void PlayAbility06ID();

	void SetIdleID(const int anIdleID);
	void SetMovingID(const int aMovingID);
	void SetDyingID(const int aDyingID);

	void SetAttack01ID(const int aID);
	void SetAttack02ID(const int aID);
	void SetAbility01ID(const int aID);
	void SetAbility02ID(const int aID);
	void SetAbility03ID(const int aID);
	void SetAbility04ID(const int aID);
	void SetAbility05ID(const int aID);
	void SetAbility06ID(const int aID);
	// ! Temporary for project 6

	// Use with E- Player/Enemy/Boss/Other -AnimationID
	//template <class T>
	//void PlayAnimation(const T anEAnimationID);
	//template <class T>
	//void SetIdleID(const T anIdleID);
	//template <class T>
	//void SetMovingID(const T aMovingID);
	//template <class T>
	//void SetDyingID(const T aDyingID);
	//template<class T>
	//void SetStateIDs(const T anIdleID, const T aMovingID, const T aDyingID);

	// Intended use: internal and Modelviewer. Use <T> version of this function if used elsewhere.
	void ModelViewerPlayAnimation(const int anAnimationIndex, bool anIsLooping = false, const float anAnimSpeed = 1.0f);

public:
	CAnimation* GetMyAnimation() { return myAnimation; }
	const float GetBlend() const { return myBlend.myBlendLerp; }
	const int CurrentAnimationState() { return myCurrentAnimationID; }

public:
	//Intended use is in Model viewer. Careful using it anywhere else. Deletes the previous myAnimation, news and inits a new CAnimation.
	bool ReplaceAnimation(const char* aRig, std::vector<std::string>& somePathsToAnimations);

private:
	void SetBonesToIdentity();
	void UpdateBlended(const float dt);
	void UpdateNonBlended(const float dt);
	void SetBlend(int anAnimationIndex, int anAnimationIndexTwo, float aBlend);// Does nothing atm.
	inline bool UpdateIdleTimer();
	inline void ResetIdleTimer();
	void SwitchBackToIdle();

	void PlayAnimation(const int anAnimationID, bool anIsLooping = false, const float anAnimSpeed = 1.0f);

private:
	CAnimation* myAnimation;
	std::array<SlimMatrix44, 64> myBones { };
	SAnimationBlend myBlend;
	bool myIsLooping;
	float myAnimationSpeed;
	std::vector<CStringID> myAnimationIds;


	int myCurrentAnimationID;// Current State
	int myIdleAnimationID;// Idle State
	int myMovingAnimationID;// Moving State
	int myDyingAnimationID;// Dying / Dead State
	
	float myReturnToIdleTimer;

	// Temporary for project 6
	int myAttack01ID;
	int myAttack02ID;
	int myAbility01ID;
	int myAbility02ID;
	int myAbility03ID;
	int myAbility04ID;
	int myAbility05ID;
	int myAbility06ID;

private: // Needed for template functions
	// This works if the ids are in a sorted list. But if the animations are added at random to the project the ids aren't sorted, making this function unsafe.
	bool WithinIDRange(const int anID)
	{
		return (anID <= myAnimationIds.back().ID() && anID >= myAnimationIds.front().ID()); 
	}

	// Goes through std::vector myAnimationID: returns true if it finds the id within the list.
	bool HasID(const int anID)
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

	// This works if the ids are in a sorted list. Uses first abs(id - anID) +1. Unsafe.
	const int GetIndexFromID(const int anID)
	{
		int index = abs(myAnimationIds[0].ID() - anID ) + 1;
		return index;
	}

	// Checks for ID withing std::vector myAnimationIds and returns index of ID
	const int GetIndexFromList(const int anID)
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
};

//template <class T>
//void CAnimationComponent::PlayAnimation(const T anEAnimationID)
//{
//	int id = static_cast<int>(anEAnimationID);
//	if (this->HasID(id))
//	{
//		this->PlayAnimation(id, false);
//		myCurrentAnimationID = id;
//	}
//	else
//	{
//		this->PlayAnimation(0, false);
//		assert(false && "Does not contain animation ID");
//	}
//}
//
//template <class T>
//void CAnimationComponent::SetIdleID(const T anIdleID)
//{
//	int id = static_cast<int>(anIdleID);
//	if (this->HasID(id))
//		this->myIdleAnimationID = id;
//	else
//		this->myIdleAnimationID = 0;
//}
//template <class T>
//void CAnimationComponent::SetMovingID(const T aMovingID)
//{
//	int id = static_cast<int>(aMovingID);
//	if (this->HasID(id))
//		this->myMovingAnimationID = id;
//	else
//		this->myMovingAnimationID = 0;
//}
//template <class T>
//void CAnimationComponent::SetDyingID(const T aDyingID)
//{
//	int id = static_cast<int>(aDyingID);
//	if (this->HasID(id))
//		this->myDyingAnimationID = id;
//	else
//		this->myDyingAnimationID = 0;
//}
//template<class T>
//void CAnimationComponent::SetStateIDs(const T anIdleID, const T aMovingID, const T aDyingID)
//{
//	this->SetIdleID(anIdleID);
//	this->SetMovingID(aMovingID);
//	this->SetDyingID(aDyingID);
//}