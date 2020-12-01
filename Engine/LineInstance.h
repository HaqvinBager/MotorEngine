#pragma once

#include "SimpleMath.h"

class CLine;
class CLineInstance
{
public:
	static const DirectX::SimpleMath::Vector3 ourInactivePos;

public:
	CLineInstance();
	~CLineInstance();

	CLineInstance& Init(CLine* aLine);

	// Copy constructor.
	CLineInstance(const CLineInstance& other)
		: myLine(other.myLine)
		, myTransform(other.myTransform)
		, myIsActive(other.myIsActive)
	{
		//std::copy();
	}

	CLineInstance(CLineInstance&& other) noexcept : myLine(nullptr), myTransform() { 
		myLine = other.myLine;
		myTransform = other.myTransform;
		myIsActive = other.myIsActive;
		other.myLine = nullptr;
	}

	CLineInstance& operator=(CLineInstance&& other) noexcept
	{
		myLine = other.myLine;
		myTransform = other.myTransform;
		myIsActive = other.myIsActive;
		other.myLine = nullptr;
		return *this;
	}
	
	void SetTransform(const DirectX::SimpleMath::Vector3& aPosition, const DirectX::SimpleMath::Vector3& aRotation);
	void SetPosition(const DirectX::SimpleMath::Vector3& aPosition);
	void SetRotation(const DirectX::SimpleMath::Vector3& aRotation);
	void SetRotation(DirectX::SimpleMath::Quaternion aQuaternion);
	void SetScale(const DirectX::SimpleMath::Vector3& aScale);
	void SetScale(const float aScale);
	void Move(const DirectX::SimpleMath::Vector3& aMovement);
	void Rotate(const DirectX::SimpleMath::Vector3& aRotation);

	void SetIsActive(const bool anIsActive);
	bool GetIsActive() const;
	
public:
	const CLine* GetLine() const{ return myLine; }
	const DirectX::SimpleMath::Matrix GetTransform() const { return myTransform; }

private:
	CLine*						myLine;
	DirectX::SimpleMath::Matrix myTransform;
	bool myIsActive;
};

