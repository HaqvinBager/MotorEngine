#pragma once

#include "SimpleMath.h"

class CLine;
class CLineInstance
{
public:
	CLineInstance();
	~CLineInstance();

	void Init(CLine* aLine);
	
	void SetTransform(const DirectX::SimpleMath::Vector3& aPosition, const DirectX::SimpleMath::Vector3& aRotation);
	void SetPosition(const DirectX::SimpleMath::Vector3& aPosition);
	void SetRotation(const DirectX::SimpleMath::Vector3& aRotation);
	void SetScale(const DirectX::SimpleMath::Vector3& aScale);
	void Move(const DirectX::SimpleMath::Vector3& aMovement);
	void Rotate(const DirectX::SimpleMath::Vector3& aRotation);
	
public:
	CLine* GetLine(){ return myLine; }
	DirectX::SimpleMath::Matrix GetTransform(){ return myTransform; }

private:
	CLine*						myLine;
	DirectX::SimpleMath::Matrix myTransform;

};

