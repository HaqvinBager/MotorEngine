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

	void BoneTransformsWithBlend(SlimMatrix44* Transforms, float aBlendFactor);
	void BoneTransforms(SlimMatrix44* Transforms);
	void BlendStep(float aDelta);
	void Step(float aDelta);

	const size_t GetNrOfAnimations() const;

public: 

	void SetCurAnimationScene(const int aCurAnimScene);

private:
	float myTotalAnimationTime = 0;
	AnimationController* myController;

#pragma region COMMENTED 2020_11_11 UNUSED No defintions exist
public :
	//void BoneTransform(SlimMatrix44* Transforms); 
	//void SetAnimator(SceneAnimator* anAnimator) { myAnimator = anAnimator; }	
	//void SetBindPose(SceneAnimator* aBindPose) { myBindPose = aBindPose; }	
	//void SetActiveAnimations(std::vector<int>& someActiveAnimations) { myActiveAnimations = someActiveAnimations; }	
	//void SetTotalAnimationTime(float aTotalAnimationTime) { myTotalAnimationTime = aTotalAnimationTime; }		
	//void SetAnimationTime(float anAnimationTime) { myAnimTime = anAnimationTime; }		
	//void SetAnimationSpeed(int anAnimationSpeed) { myAnimSpeed = anAnimationSpeed; }		

	//SceneAnimator* GetAnimator() const { return myAnimator; }		
	//SceneAnimator* GetBindPose() const { return myBindPose; }		
	//const std::vector<int>& GetActiveAnimations() const { return myActiveAnimations; }
	//const float GetTotalAnimationTime() const { return myTotalAnimationTime; }		
	//const float GetAnimationTime() const { return myAnimTime; }		
	//const int GetAnimationSpeed() const { return myAnimSpeed; }		


private:
	//SceneAnimator* myAnimator; 
	//SceneAnimator* myBindPose = nullptr;
	//std::vector<int> myActiveAnimations;

	//int myAnimSpeed = 60;
	//float myAnimTime = 0;

#pragma endregion ! COMMENTED 2020_11_11 UNUSED
};