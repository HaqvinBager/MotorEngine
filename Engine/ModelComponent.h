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

#include "Component.h"

class CModelInstance;
class CGameObject;
class CModel;

class CModelComponent : public CComponent 
{
public:
	CModelComponent(CGameObject& aParent);
	~CModelComponent() override;


	void Awake() override;
	void Start() override;
	void Update() override;

	void SetMyModel(CModel* aModel);


	CModelInstance* GetMyModel() const;

private:
	CModelInstance* myModelInstance;

};