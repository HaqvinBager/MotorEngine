#include "stdafx.h"
#include "ModelComponent.h"
#include "ModelInstance.h"
#include "GameObject.h"
#include "Model.h"

CModelComponent::CModelComponent(CGameObject& aParent) : CComponent(aParent), myModelInstance(new CModelInstance())
{
	
}

CModelComponent::~CModelComponent()
{
	myModelInstance = nullptr;
}

void CModelComponent::Awake()
{
}

void CModelComponent::Start()
{
}

void CModelComponent::Update()
{
}


void CModelComponent::SetMyModel(CModel* aModel)
{
	myModelInstance->Init(aModel);
}

CModelInstance* CModelComponent::GetMyModel() const
{
	return myModelInstance;
}
