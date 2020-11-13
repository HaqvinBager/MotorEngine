#pragma once
#include "Behaviour.h"

#include "SimpleMath.h"
#include "ModelMath.h"

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
	CAnimationComponent(CGameObject& aParent);
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
	void GetAnimatedTransforms(float dt, SlimMatrix44* transforms);
	void SetBlend(int anAnimationIndex, int anAnimationIndexTwo, float aBlend);
	void PlayAnimation(const int anAnimationIndex, bool aIsLooping = false);

private:
	void SetBonesToIdentity();

	CAnimation* myAnimation;
	std::array<SlimMatrix44, 64> myBones { };
	SAnimationBlend myBlend;

};