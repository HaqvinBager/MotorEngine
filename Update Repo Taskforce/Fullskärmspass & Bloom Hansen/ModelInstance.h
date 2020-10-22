#pragma once
#include "..\DirectXTK\Inc\SimpleMath.h"
#include "Model.h"

namespace SM = DirectX::SimpleMath;

class CModelInstance {
public:
	CModelInstance();
	~CModelInstance();

	void Init(CModel* aModel);
	void SetTransform(SM::Vector3 aPosition, SM::Vector3 aRotation);
	void SetRotation(SM::Vector3 aRotation);
	void SetPosition(SM::Vector3 aPosition);
	void Move(SM::Vector3 aMovement);
	void Rotate(SM::Vector3 aRotation);
	SM::Matrix GetTransform();
	SM::Matrix GetProjection();
	CModel* GetModel();

private:
	CModel* myModel;
	SM::Matrix myTransform;
	SM::Matrix myProjection;
};
