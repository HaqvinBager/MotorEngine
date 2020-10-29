#pragma once

#include "SimpleMath.h"

class CLine;
class CLineInstance
{
public:
	CLineInstance();
	~CLineInstance();

	CLineInstance& Init(CLine* aLine);

	// Copy constructor.
	CLineInstance(const CLineInstance& other)
		: myLine(other.myLine)
		, myTransform(other.myTransform)
	{
		//std::copy();
	}

	CLineInstance(CLineInstance&& other) noexcept : myLine(nullptr), myTransform() { 
		myLine = other.myLine;
		myTransform = other.myTransform;
		other.myLine = nullptr;
	}

	CLineInstance& operator=(CLineInstance&& other) noexcept
	{
		myLine = other.myLine;
		myTransform = other.myTransform;
		other.myLine = nullptr;
		return *this;
	}
	
	void SetTransform(const DirectX::SimpleMath::Vector3& aPosition, const DirectX::SimpleMath::Vector3& aRotation);
	void SetPosition(const DirectX::SimpleMath::Vector3& aPosition);
	void SetRotation(const DirectX::SimpleMath::Vector3& aRotation);
	void SetScale(const DirectX::SimpleMath::Vector3& aScale);
	void Move(const DirectX::SimpleMath::Vector3& aMovement);
	void Rotate(const DirectX::SimpleMath::Vector3& aRotation);
	
public:
	const CLine* GetLine() const{ return myLine; }
	const DirectX::SimpleMath::Matrix GetTransform() const { return myTransform; }

private:
	CLine*						myLine;
	DirectX::SimpleMath::Matrix myTransform;

};

