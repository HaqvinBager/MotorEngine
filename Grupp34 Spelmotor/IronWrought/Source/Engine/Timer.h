#pragma once
#include <chrono>

class CTimer {
public:
	static float Time();
	static float Dt();

	CTimer();
	~CTimer();

	float Mark();
	//float Peek() const;
	float TotalTime() const;
private:
	static CTimer* ourInstance;
	std::chrono::steady_clock::time_point myFirst;
	std::chrono::steady_clock::time_point myLast;
	float myDeltaTime;
};