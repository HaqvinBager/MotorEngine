#include "stdafx.h"
#include "ModelInstance.h"

CModelInstance::CModelInstance() :myModel(nullptr) {

}

CModelInstance::~CModelInstance() {

}

void CModelInstance::Init(CModel* aModel) {
	myModel = aModel;
}

void CModelInstance::SetTransform(SM::Vector3 aPosition, SM::Vector3 aRotation) {
	SetRotation(aRotation);
	SetPosition(aPosition);
}

void CModelInstance::SetRotation(SM::Vector3 aRotation) {
	SM::Vector3 lastPostion = myTransform.Translation();
	myTransform.Translation(SM::Vector3(0.0f, 0.0f, 0.0f));
	myTransform = SM::Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
	myTransform.Translation(lastPostion);
}

void CModelInstance::SetPosition(SM::Vector3 aPosition) {
	myTransform.Translation(aPosition);
}

void CModelInstance::Move(SM::Vector3 aMovement) {
	myTransform.Translation(myTransform.Translation() + myTransform.Right() * aMovement.x);
	myTransform.Translation(myTransform.Translation() + myTransform.Up() * aMovement.y);
	myTransform.Translation(myTransform.Translation() - myTransform.Forward() * aMovement.z);
}

void CModelInstance::Rotate(SM::Vector3 aRotation) { 
	SM::Vector3 lastPostion = myTransform.Translation();
	myTransform.Translation(SM::Vector3(0.0f, 0.0f, 0.0f));
	myTransform *= SM::Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
	myTransform.Translation(lastPostion);
}

SM::Matrix CModelInstance::GetTransform() {
	return myTransform;
}

SM::Matrix CModelInstance::GetProjection() {
	return myProjection;
}

CModel* CModelInstance::GetModel() {
	return myModel;
}
