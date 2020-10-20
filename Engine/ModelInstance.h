#pragma once
#include "SimpleMath.h"
#include "ModelMath.h"
#include <array>

class CModel;

struct SModelInstanceAnimationBlend
{
	int myFirst = -1;
	int mySecond = -1;
	float myBlendLerp = 0;
};

class CModelInstance {
public:
	CModelInstance();
	~CModelInstance();	

	void Init(CModel* aModel);
	void SetTransform(DirectX::SimpleMath::Vector3 aPosition, DirectX::SimpleMath::Vector3 aRotation);
	void SetPosition(DirectX::SimpleMath::Vector3 aPosition);
	void SetRotation(DirectX::SimpleMath::Vector3 aRotation);
	void Move(DirectX::SimpleMath::Vector3 aMovement);
	void Rotate(DirectX::SimpleMath::Vector3 aRotation);
	void SetScale(DirectX::SimpleMath::Vector3 aScale);

public:
	void UpdateAnimation(float dt);

	std::array<SlimMatrix44, 64> GetBones() { return myBones; }
	void GetAnimatedTransforms(float dt, SlimMatrix44* transforms);
	void SetBlend(int anAnimationIndex, int anAnimationIndexTwo, float aBlend);

public:
	CModel* GetModel() { return myModel; }
	// Temp
	CModel* SetModel(CModel* aModel)
	{
		CModel* oldModel = myModel;
		myModel = aModel;
		return oldModel;
	}
	// !Temp
	DirectX::SimpleMath::Matrix GetTransform() { return myTransform; };
	DirectX::SimpleMath::Vector2 GetUVOffset() { return myUVOffset; };
	void SetUVOffset(DirectX::SimpleMath::Vector2 aUVOffset) { myUVOffset = aUVOffset; };

private:
	CModel* myModel;
	DirectX::SimpleMath::Matrix myTransform;
	DirectX::SimpleMath::Vector2 myUVOffset;
	DirectX::SimpleMath::Vector3 myScale;
	std::array<SlimMatrix44, 64> myBones { };
	SModelInstanceAnimationBlend myBlend;
};