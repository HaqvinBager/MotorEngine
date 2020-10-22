#include "UtilityTimer.hpp"

#include <string>

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

CommonUtilities::UtilityTimer::UtilityTimer() 
{
	myNow = std::chrono::high_resolution_clock::now();
	myLastUpdate = myNow;

	myDeltaTime = std::chrono::duration_cast<std::chrono::duration<float>> (std::chrono::milliseconds::zero());
	myTotalTime = myDeltaTime;
}

CommonUtilities::UtilityTimer::~UtilityTimer()
{}

void CommonUtilities::UtilityTimer::Update()
{
	myLastUpdate	= std::chrono::high_resolution_clock::now();
	myDeltaTime		= std::chrono::duration_cast<std::chrono::duration<float>> (myLastUpdate - myNow);
	myTotalTime		+= myDeltaTime;
	myNow			= myLastUpdate;
}

void CommonUtilities::UtilityTimer::Reset()
{
#ifdef  _DEBUG
	std::cout << "Resetting Timer: " << this << std::endl << std::endl;
#endif //  _DEBUG

	myNow = std::chrono::high_resolution_clock::now();
	myLastUpdate = myNow;

	myDeltaTime = std::chrono::duration_cast<std::chrono::duration<float>> (std::chrono::milliseconds::zero());
	myTotalTime = myDeltaTime;
}

float CommonUtilities::UtilityTimer::GetDeltaTime() const 
{
	return myDeltaTime.count();
}

double CommonUtilities::UtilityTimer::GetTotalTime() const 
{
	return myTotalTime.count();
}

const std::string CommonUtilities::UtilityTimer::GetDeltaTimeString()
{
	//char text[256];
	//sprintf_s(text, "%1.f", myDeltaTime.count());
	//return text;
	return  std::to_string(myDeltaTime.count());
}

const std::string CommonUtilities::UtilityTimer::GetTotalTimeString()
{
	//char text[256];
	//sprintf_s(text, "%.f", myTotalTime.count());
	
	 return  std::to_string(myTotalTime.count());
}
