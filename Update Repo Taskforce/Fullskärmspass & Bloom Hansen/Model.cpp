#include "stdafx.h"
#include "Model.h"

CModel::CModel() : myModelData(CModel::SModelData()){

}

CModel::~CModel() {

}

void CModel::Init(SModelData aModelData) {
	myModelData = aModelData;
}

CModel::SModelData CModel::GetModelData() {
	return myModelData;
}
