#include "stdafx.h"
#include "Model.h"
#include "Animation.h"

CModel::CModel() : myModelData() {
}

CModel::~CModel() {
}

void CModel::Init(SModelData data) {
	myModelData = std::move(data);
}

CModel::SModelData& CModel::GetModelData() {
	return myModelData;
}

int CModel::AddAnimation(CAnimation* aAnimation)
{
	myAnimations.push_back(aAnimation);
	return static_cast<int>(myAnimations.size());
}