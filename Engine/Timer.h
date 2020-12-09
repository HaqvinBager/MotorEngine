#pragma once
#include <chrono>

class CTimer {
public:
	static float Time();
	static float Dt();

	CTimer();
	~CTimer();

	static float Mark();

private:
	float NewFrame();
	float TotalTime() const;

private:
	static CTimer* ourInstance;
	std::chrono::steady_clock::time_point myFirst;
	std::chrono::steady_clock::time_point myLast;
	float myDeltaTime;
};