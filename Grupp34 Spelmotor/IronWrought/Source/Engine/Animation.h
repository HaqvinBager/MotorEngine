#include "ModelMath.h"
#include "ModelMath.h"

class SceneAnimator;
class AnimationController;

class CAnimation
{
public:
	void Init(const char* aRig, std::vector<std::string>& somePathsToAnimations);

	// ANIMATION STUFF
	//void BoneTransform(SlimMatrix44* Transforms);
	void BoneTransformWithBlend(SlimMatrix44* Transforms, float aBlendFactor);
	void Step(float aDelta);

	void SetAnimator(SceneAnimator* anAnimator) { myAnimator = anAnimator; }
	void SetBindPose(SceneAnimator* aBindPose) { myBindPose = aBindPose; }
	void SetActiveAnimations(std::vector<int>& someActiveAnimations) { myActiveAnimations = someActiveAnimations; }
	void SetTotalAnimationTime(float aTotalAnimationTime) { myTotalAnimationTime = aTotalAnimationTime; }
	void SetAnimationTime(float anAnimationTime) { myAnimTime = anAnimationTime; }
	void SetAnimationSpeed(int anAnimationSpeed) { myAnimSpeed = anAnimationSpeed; }

	SceneAnimator* GetAnimator() const { return myAnimator; }
	SceneAnimator* GetBindPose() const { return myBindPose; }
	const std::vector<int>& GetActiveAnimations() const { return myActiveAnimations; }
	const float GetTotalAnimationTime() const { return myTotalAnimationTime; }
	const float GetAnimationTime() const { return myAnimTime; } 
	const int GetAnimationSpeed() const { return myAnimSpeed; }

private:
	SceneAnimator* myAnimator;
	SceneAnimator* myBindPose = nullptr;
	std::vector<int> myActiveAnimations;
	float myTotalAnimationTime = 0;
	float myAnimTime = 0;
	int myAnimSpeed = 60;
	AnimationController* controller;
};