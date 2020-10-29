#pragma once
#include "LineInstance.h"

namespace DirectX {
	namespace SimpleMath {
		struct Vector;
	}
}

struct SLineTime
{
public:
	float myTime;
	CLineInstance myLine;
};

class CDebug
{
public:
	CDebug();
	~CDebug();

	static CDebug* GetInstance();

	void DrawLine(DirectX::SimpleMath::Vector3 aPositionA, DirectX::SimpleMath::Vector3 aPositionB);
	const std::vector<CLineInstance>& GetLines() const;
	const std::vector<SLineTime>& GetLinesTime() const;

	void Update();

private:
	static CDebug* ourInstance;

private:
	std::vector<CLineInstance> myLineInstances;
	std::vector<SLineTime> myLines;

};