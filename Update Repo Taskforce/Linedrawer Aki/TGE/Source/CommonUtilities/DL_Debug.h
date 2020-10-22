#pragma once

#include <fstream>

#define DL_RESOURCE_LOG_NAME	"resource_log.txt"
#define DL_ENGINE_LOG_NAME		"engine_log.txt"
#define DL_GAME_LOG_NAME		"game_log.txt"

#define USE_FILTERLOG

#ifdef USE_FILTERLOG
#define DL_RESOURCE_LOG(...)	DL_Debug::Debug::GetInstance()->WriteLog(DL_Debug::Debug::ELogFilter::Resource, DL_RESOURCE_LOG_NAME, __LINE__, __FUNCTION__, __VA_ARGS__);
#define DL_ENGINE_LOG(...)		DL_Debug::Debug::GetInstance()->WriteLog(DL_Debug::Debug::ELogFilter::Engine, DL_ENGINE_LOG_NAME, __LINE__, __FUNCTION__, __VA_ARGS__);
#define DL_GAME_LOG(...)		DL_Debug::Debug::GetInstance()->WriteLog(DL_Debug::Debug::ELogFilter::Game, DL_GAME_LOG_NAME, __LINE__, __FUNCTION__, __VA_ARGS__);

#define DL_DEACTIVATE_ALL_LOGS DL_Debug::Debug::GetInstance()->DeactivateFilterLog(DL_Debug::Debug::ELogFilter::Resource);DL_Debug::Debug::GetInstance()->DeactivateFilterLog(DL_Debug::Debug::ELogFilter::Engine);DL_Debug::Debug::GetInstance()->DeactivateFilterLog(DL_Debug::Debug::ELogFilter::Game);
#define DL_ACTIVATE_ALL_LOGS	DL_Debug::Debug::GetInstance()->ActivateFilterLog(DL_Debug::Debug::ELogFilter::Resource);DL_Debug::Debug::GetInstance()->ActivateFilterLog(DL_Debug::Debug::ELogFilter::Engine);DL_Debug::Debug::GetInstance()->ActivateFilterLog(DL_Debug::Debug::ELogFilter::Game);

#define DL_ACTIVATE_LOG(logString) DL_Debug::Debug::GetInstance()->ActivateFilterLog(DL_Debug::Debug::GetInstance()->TranslateStringTo(logString));
#define DL_DEACTIVATE_LOG(logString) DL_Debug::Debug::GetInstance()->DeactivateFilterLog(DL_Debug::Debug::GetInstance()->TranslateStringTo(logString));
#else
#define DL_WRITE(logString, ...)	DL_Debug::Debug::GetInstance()->WriteLog(DL_Debug::Debug::GetInstance()->TranslateStringTo(logString), logString, __LINE__, __FUNCTION__, __VA_ARGS__);
#endif

#define DL_ASSERT(string)	DL_Debug::Debug::GetInstance()->AssertMessage(__FILE__, __LINE__, __FUNCTION__, string);
#define DL_PRINT(string)	DL_Debug::Debug::GetInstance()->PrintMessage(string);
#define DL_DEBUG(...)		DL_Debug::Debug::GetInstance()->DebugMessage(__LINE__, __FUNCTION__, __VA_ARGS__);

namespace DL_Debug {
	class Debug {
	public:
		enum class ELogFilter
		{
			Custom		= 1 << 0,
			Resource	= 1 << 1,
			Engine		= 1 << 2,
			Game		= 1 << 3,
		};

	public:
		static void Create(const std::string& file = "debug_log.txt");
		static void Destroy();
		static Debug* GetInstance();

		void ActivateFilterLog(ELogFilter aFilter);
		void DeactivateFilterLog(ELogFilter aFilter);

		void AssertMessage(const char* aFilename, const int& aLine, const char* aFunctionName, const char* aString);
		void PrintMessage(const char* aMessage);
		void DebugMessage(const int& aLine, const char* aFilename, const char* aFormattedString, ...);
		// aLog must include .txt
		void WriteLog(DL_Debug::Debug::ELogFilter aFilter, const char* aLog, const int& aFunctionLine, const char* aFunctionName, const char* aFormattedString, ...);

		ELogFilter TranslateStringTo(const char* aString);

	private:
		Debug() { myActiveFilters = 0; };
		~Debug(){};

		static Debug* ourInstance;

		unsigned int    myActiveFilters;
		std::ofstream	myDebugFile;
		// The following ofstreams correspond to ELog enun class.
		std::ofstream myCustomFile;
		std::ofstream myResourceFile;
		std::ofstream myEngineFile;
		std::ofstream myGameFile;
		// !ELog enun class ofstreams.
	};
}