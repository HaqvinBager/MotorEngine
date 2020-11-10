#pragma once

/// CModelComponent
/// 
///  Håller en CModelIstance
///
/// CModelInstance kommer brytas upp i: 
///		CModelComponent
///     AnimationComponent
///		CTransformComponent -> OK!
/// To do:
/// Byt ut CModelInstance till en CModel
///

#include "Behaviour.h"

class CModelInstance;
class CGameObject;
class CModel;

class CModelComponent : public CBehaviour
{
public:
	CModelComponent(CGameObject& aParent, std::string aFBXPath);
	~CModelComponent() override;

	void Awake() override;
	void Start() override;
	void Update() override;

	void SetModel(const std::string& aFBXPath);

	CModel *GetMyModel() const;

	void OnEnable() override;
	void OnDisable() override;
private:

	CModel* myModel;
};


//public:
//	CModel* GetModel() { return myModel; }
//	// Temp
//	CModel* SetModel(CModel* aModel)
//	{
//		CModel* oldModel = myModel;
//		myModel = aModel;
//		return oldModel;
//	}
//	// !Temp
//	DirectX::SimpleMath::Matrix GetTransform() { return myTransform; };
//	DirectX::SimpleMath::Vector2 GetUVOffset() { return myUVOffset; };
//	void SetUVOffset(DirectX::SimpleMath::Vector2 aUVOffset) { myUVOffset = aUVOffset; };
//
//private:
//	CModel* myModel;
//	DirectX::SimpleMath::Matrix myTransform;
//	DirectX::SimpleMath::Vector2 myUVOffset;
//	DirectX::SimpleMath::Vector3 myScale;
//	std::array<SlimMatrix44, 64> myBones{ };
//  SModelInstanceAnimationBlend myBlend;
//};