#include "stdafx.h"
#include "InstancedModelComponent.h"
#include "Model.h"
#include "ModelFactory.h"

CInstancedModelComponent::CInstancedModelComponent(CGameObject& aParent, std::string aModelPath, int aCount, std::vector<DirectX::SimpleMath::Matrix> aInstancedTransforms, bool aRenderWithAlpha)
	: CBehaviour(aParent)
	, myModel(CModelFactory::GetInstance()->GetInstancedModel(aModelPath, aCount))
	, myIntancedTransforms(aInstancedTransforms)
	, myRenderWithAlpha(aRenderWithAlpha)
{
}

CInstancedModelComponent::~CInstancedModelComponent()
{
}

void CInstancedModelComponent::Awake()
{
}

void CInstancedModelComponent::Start()
{
}

void CInstancedModelComponent::Update()
{
}

void CInstancedModelComponent::OnEnable()
{
}

void CInstancedModelComponent::OnDisable()
{
}
