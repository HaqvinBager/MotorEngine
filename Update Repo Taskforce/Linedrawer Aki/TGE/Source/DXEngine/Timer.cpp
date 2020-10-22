#include "pch.h"
#include "Timer.hpp"


Timer::Timer()
{
	myLast = std::chrono::steady_clock::now();
}

float Timer::Mark()
{
	const auto old = myLast;
	myLast = std::chrono::steady_clock::now();
	const std::chrono::duration<float> delta_time = myLast - old;
	return delta_time.count();
}

float Timer::Peek() const
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - myLast).count();
}