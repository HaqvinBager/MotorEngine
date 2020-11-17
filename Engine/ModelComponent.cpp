#include "stdafx.h"
#include "ModelComponent.h"
#include "ModelFactory.h"
#include "GameObject.h"
#include "Model.h"

CModelComponent::CModelComponent(CGameObject& aParent, std::string aFBXPath) : CBehaviour(aParent) {
	myModel = CModelFactory::GetInstance()->GetModelPBR(aFBXPath);

}

CModelComponent::~CModelComponent()
{}

void CModelComponent::Awake()
{}

void CModelComponent::Start()
{}

void CModelComponent::Update()
{}

void CModelComponent::SetModel(const std::string& aFBXPath)
{
	myModel = CModelFactory::GetInstance()->GetModelPBR(aFBXPath);
}

CModel* CModelComponent::GetMyModel() const
{
	return myModel;
}

void CModelComponent::OnEnable()
{}

void CModelComponent::OnDisable()
{}
