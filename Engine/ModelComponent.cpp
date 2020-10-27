#include "stdafx.h"
#include "ModelComponent.h"
#include "ModelFactory.h"
#include "GameObject.h"
#include "Model.h"

CModelComponent::CModelComponent(CGameObject& aParent, std::string aFBXPath) : CComponent(aParent) {
	myModel = CModelFactory::GetInstance()->GetModelPBR(aFBXPath);

}

CModelComponent::~CModelComponent()
{

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

CModel* CModelComponent::GetMyModel() const
{
	return myModel;
}
