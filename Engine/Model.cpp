#include "stdafx.h"
#include "Model.h"
#include "Animation.h"

CModel::CModel() 
	: myModelData()
	, myModelInstanceData()
	, myInstanceCount(0)
{
}

CModel::~CModel() {
}

void CModel::Init(SModelData data) {
	myModelData = std::move(data);
}

void CModel::Init(SModelInstanceData data) {
	myModelInstanceData = std::move(data);
}

CModel::SModelData& CModel::GetModelData() {
	return myModelData;
}

void CModel::InstanceCount(int aCount)
{
	myInstanceCount = aCount;
	//myTransforms.reserve(aCount);
}

//bool CModel::AddInstancedTransform(DirectX::SimpleMath::Matrix aTransform)
//{
//	if (myTransforms.size() < myInstanceCount) {
//		myTransforms.emplace_back(aTransform);
//	} else {
//		return false;
//	}
//	return true;
//}
