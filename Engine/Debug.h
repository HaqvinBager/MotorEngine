#pragma once
#include "LineInstance.h"

namespace DirectX {
	namespace SimpleMath {
		struct Vector;
	}
}

class CDebug
{
public:
	CDebug();
	~CDebug();

	static CDebug* GetInstance();

	void DrawLine(DirectX::SimpleMath::Vector3 aPositionA, DirectX::SimpleMath::Vector3 aPositionB);
	const std::vector<CLineInstance>& GetLines() const;

private:
	static CDebug* ourInstance;

private:
	std::vector<CLineInstance> myLineInstances;

};