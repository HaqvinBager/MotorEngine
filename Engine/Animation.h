#include "ModelMath.h"
#include "ModelMath.h"

class SceneAnimator;
class AnimationController;

class CAnimation
{
public:
	CAnimation() = default;
	~CAnimation();

	void Init(const char* aRig, std::vector<std::string>& somePathsToAnimations);

	// ANIMATION STUFF
	//void BoneTransform(SlimMatrix44* Transforms); 
	void BoneTransformWithBlend(SlimMatrix44* Transforms, float aBlendFactor);	// Necessary (2020 11 11 note)
	void Step(float aDelta);	// Necessary (2020 11 11 note)

	//void SetAnimator(SceneAnimator* anAnimator) { myAnimator = anAnimator; }	// commented away 2020 11 11
	//void SetBindPose(SceneAnimator* aBindPose) { myBindPose = aBindPose; }	// commented away 2020 11 11
	//void SetActiveAnimations(std::vector<int>& someActiveAnimations) { myActiveAnimations = someActiveAnimations; }	// commented away 2020 11 11
	//void SetTotalAnimationTime(float aTotalAnimationTime) { myTotalAnimationTime = aTotalAnimationTime; }		// commented away 2020 11 11
	//void SetAnimationTime(float anAnimationTime) { myAnimTime = anAnimationTime; }		// commented away 2020 11 11
	//void SetAnimationSpeed(int anAnimationSpeed) { myAnimSpeed = anAnimationSpeed; }		// commented away 2020 11 11

	//SceneAnimator* GetAnimator() const { return myAnimator; }		// commented away 2020 11 11
	//SceneAnimator* GetBindPose() const { return myBindPose; }		// commented away 2020 11 11
	//const std::vector<int>& GetActiveAnimations() const { return myActiveAnimations; }		// commented away 2020 11 11s
	//const float GetTotalAnimationTime() const { return myTotalAnimationTime; }		// commented away 2020 11 11
	//const float GetAnimationTime() const { return myAnimTime; }		// commented away 2020 11 11
	//const int GetAnimationSpeed() const { return myAnimSpeed; }		// commented away 2020 11 11
	
	const size_t GetNrOfAnimations() const;// added 2020 11 11

private:
	//SceneAnimator* myAnimator; // commented away 2020 11 11
	//SceneAnimator* myBindPose = nullptr; // commented away 2020 11 11
	//std::vector<int> myActiveAnimations; // commented away 2020 11 11
	float myTotalAnimationTime = 0;// used in cpp
	//float myAnimTime = 0; // commented away 2020 11 11
	//int myAnimSpeed = 60; // commented away 2020 11 11
	AnimationController* controller;// used in cpp
};