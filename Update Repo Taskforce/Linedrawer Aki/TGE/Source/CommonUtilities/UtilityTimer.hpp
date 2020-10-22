#pragma once

#include <chrono>
#include <string>

namespace CommonUtilities {
	class UtilityTimer {
	public:
		UtilityTimer();
		UtilityTimer(const UtilityTimer& other)			  = delete;
		UtilityTimer& operator= (const UtilityTimer& other) = delete;
		~UtilityTimer();

		void Update();
		void Reset();

		float GetDeltaTime() const;
		double GetTotalTime() const;

		const std::string GetDeltaTimeString();
		//const char* GetTotalTimeString();
		const std::string GetTotalTimeString();

	private:
		std::chrono::high_resolution_clock::time_point myNow;
		std::chrono::high_resolution_clock::time_point myLastUpdate;

		std::chrono::duration<float>  myDeltaTime;
		std::chrono::duration<double> myTotalTime;
	};
}