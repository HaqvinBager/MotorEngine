#include "stdafx.h"
#include "Timer.h"

CTimer* CTimer::ourInstance = nullptr;

float CTimer::Time()
{
	return ourInstance->TotalTime();
}

float CTimer::Dt()
{
	return ourInstance->myDeltaTime;
}

CTimer::CTimer()
{
	ourInstance = this;
	myFirst = std::chrono::steady_clock::now();
	myLast = myFirst;
}

CTimer::~CTimer()
{
	ourInstance = nullptr;
}

float CTimer::Mark() 
{
	const auto old = myLast;
	myLast = std::chrono::steady_clock::now();
	const std::chrono::duration<float> dt = myLast - old;
	myDeltaTime = dt.count();
	return myDeltaTime;
}

float CTimer::Peek() const 
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - myLast).count();
}

float CTimer::TotalTime() const
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - myFirst).count();
}
