#include "stdafx.h"
#include "Timer.h"

CTimer* CTimer::ourInstance = nullptr;
CTimer::CTimer()
{
	ourInstance = this;
	myLast = std::chrono::steady_clock::now();
	myFirst = myLast;
}
CTimer::~CTimer()
{
	ourInstance = nullptr;
}

//Total duration in seconds since start
float CTimer::Time()
{
	return ourInstance->TotalTime();
}

//Delta time in seconds between the last two frames
float CTimer::Dt()
{
	return ourInstance->myDeltaTime;
}

float CTimer::Mark()
{
	const auto old = myLast;
	myLast = std::chrono::steady_clock::now();
	const std::chrono::duration<float> dt = myLast - old;
	myDeltaTime = dt.count();
	return myDeltaTime;
}

float CTimer::TotalTime() const
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - myFirst).count();
}