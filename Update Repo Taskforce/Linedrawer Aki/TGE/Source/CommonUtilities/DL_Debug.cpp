#include "DL_Debug.h"

#include "common_macros.hpp"
#include "DL_Assert.h"
#include "DL_StackWalker.h"

//#include <cassert>
#include <cstdarg>
#include <chrono>
#include <ctime>
#include <time.h>

#ifdef _DEBUG
#include <iostream>
#endif
DL_Debug::Debug* DL_Debug::Debug::ourInstance = nullptr;

namespace DL_Debug
{
	void Debug::Create(const std::string& file) 
	{
		assert(ourInstance == nullptr && "DL_Debug Instance already created!");
	
		ourInstance = new Debug();

		using DFL = Debug::ELogFilter;
		ourInstance->myActiveFilters	|= static_cast<UINT>(DFL::Resource);
		ourInstance->myActiveFilters	|= static_cast<UINT>(DFL::Engine);
		ourInstance->myActiveFilters	|= static_cast<UINT>(DFL::Game);

		char timeString[26];
		std::time_t timeT = std::time(nullptr);
		ctime_s(timeString, sizeof(timeString), &timeT);
		//  from:	Wed Sep 30 12:00:32 2020 \n
		//  to:		Sep 30 12:00:32
		//	index=4 >Sep 30 12:00:32> index=19 -> Sep30_1200_
		std::string actualTimeString = { timeString[4], timeString[5], timeString[6], timeString[8], timeString[9]
										, '_', timeString[11], timeString[12],  timeString[14], timeString[15]
										, '_' }; //Easy and lazy way
		
		const wchar_t* folder = L"Debug Logs\\";
		if (CreateDirectory(folder, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
		{}
		ourInstance->myDebugFile		= std::ofstream("Debug Logs\\" + actualTimeString + file);
		ourInstance->myResourceFile		= std::ofstream("Debug Logs\\" + actualTimeString + "resource_log.txt");
		ourInstance->myEngineFile		= std::ofstream("Debug Logs\\" + actualTimeString + "engine_log.txt");
		ourInstance->myGameFile			= std::ofstream("Debug Logs\\" + actualTimeString + "game_log.txt");

		DL_DEACTIVATE_ALL_LOGS
	}
	
	void Debug::Destroy() 
	{
		assert(ourInstance != nullptr && "NO DL_DEBUG CREATED!");
	
		ourInstance->myDebugFile.close();
		SAFE_DELETE(ourInstance);
	}
	
	Debug* DL_Debug::Debug::GetInstance() 
	{
		assert(ourInstance != nullptr && "DL_DEBUG NOT CREATED!");
		return ourInstance;
	}

	void Debug::ActivateFilterLog(ELogFilter aFilter)
	{
		assert(ourInstance != nullptr && "DL_DEBUG NOT CREATED!");
		ourInstance->myActiveFilters |= static_cast<UINT>(aFilter);
	}
	void Debug::DeactivateFilterLog(ELogFilter aFilter)
	{
		assert(ourInstance != nullptr && "DL_DEBUG NOT CREATED!");
		if (ourInstance->myActiveFilters & static_cast<UINT>(aFilter))
		{
			ourInstance->myActiveFilters -= static_cast<UINT>(aFilter);
		}
	}

	void Debug::AssertMessage(const char* aFilename, const int& aLine, const char* aFunctionName, const char* aMessage) 
	{
		DL_PRINT("****CALL STACK****")
		DL_Debug::StackWalker stack_walker;
		stack_walker.ShowCallstack();
		DL_PRINT("****END OF CALL STACK****")
		ourInstance->myDebugFile 
			<< "Crash at\n File: "	<< aFilename
			<< "\n Line: "			<< aLine 
			<< "\n Function: "		<< aFunctionName 
			<< "\n Message: "		<< aMessage
			<< "\n";
		
		// AssertMessage causes a crash, therefore we are destroying ourInstance so that it is terminated correctly
		ourInstance->Destroy();
	
		dl_assert(false && "See debug log for more info.");
	}
	
	void Debug::PrintMessage(const char* aMessage) 
	{
		assert(ourInstance != nullptr && "DL_DEBUG NOT CREATED!");
		ourInstance->myDebugFile << aMessage << "\n";
#ifdef _DEBUG
		std::cout << aMessage << std::endl << std::endl;
#endif
	}
	
	void Debug::DebugMessage(const int& aLine, const char* aFilename, const char* aFormattedString, ...)
	{
		assert(ourInstance != nullptr && "DL_DEBUG NOT CREATED!");
		char buffer[256];
		va_list args;
		va_start(args, aFormattedString);
		vsprintf_s(buffer, aFormattedString, args);
	
		ourInstance->myDebugFile
			<< "File: "			<< aFilename
			<< " - Line: "		<< aLine
			<< " - Message: "	<< buffer
			<< "\n";
	}
	
	void Debug::WriteLog(DL_Debug::Debug::ELogFilter aFilter, const char* aLog, const int& aFunctionLine, const char* aFunctionName, const char* aFormattedString, ...)
	{
		assert(ourInstance != nullptr && "DL_DEBUG NOT CREATED!");
		if (aFilter != Debug::ELogFilter::Custom)
		{
			if (!(ourInstance->myActiveFilters & static_cast<UINT>(aFilter)))
			{
				switch (aFilter)
				{
					case Debug::ELogFilter::Resource:	PrintMessage("Filter: Resource is not active. Please activate before trying to write to it.");	break;
					case Debug::ELogFilter::Engine:		PrintMessage("Filter: Engine is not active. Please activate before trying to write to it.");	break;
					case Debug::ELogFilter::Game:		PrintMessage("Filter: Game is not active. Please activate before trying to write to it."); 		break;
					default:							PrintMessage("ERROR: No filter sent in."); break;
				}
				return;
			}
		}
		/*
		[TID][LOGGTYP] MEDDELANDE
		exempel:
		[15:38:32][resource] Model apa.fbx loaded in 57ms
		*/
		char timeString[26];
		std::time_t timeT = std::time(nullptr);
		ctime_s(timeString, sizeof(timeString), &timeT);
		//  from:	Wed Sep 30 12:00:32 2020 \n
		//  to:		Sep 30 12:00:32
		//	index=4 >Sep 30 12:00:32> index=19
		char actualTimeString[16] = { timeString[4], timeString[5], timeString[6], timeString[7], timeString[8]
									, timeString[9], timeString[10], timeString[11], timeString[12], timeString[13]
									, timeString[14], timeString[15], timeString[16], timeString[17], timeString[18] }; //Easy and lazy way

		char buffer[512];// 256 is most likely enough, maybe 512 is better?
		va_list args;
		va_start(args, aFormattedString);
		vsprintf_s(buffer, aFormattedString, args);

		std::ofstream* os = nullptr;
		switch (aFilter)
		{
			case Debug::ELogFilter::Resource:{ os = &ourInstance->myResourceFile; }
			break;
			case Debug::ELogFilter::Engine:{ os = &ourInstance->myEngineFile; }
			break;
			case Debug::ELogFilter::Game:{ os = &ourInstance->myGameFile; }
			break;
		}

		*os << " [" << actualTimeString << "]"
			<< " ["	<< aLog << "]"
			<< " [L:" << aFunctionLine << "]"
			<< " [F:" << aFunctionName << "] \n\t "
			<< buffer 
			<< "\n\n";

#ifdef _DEBUG
		std::cout 			
			<< " [" << actualTimeString << "]"
			<< " ["	<< aLog << "]"
			<< " [L:" << aFunctionLine << "]"
			<< " [F:" << aFunctionName << "] \n\t "
			<< buffer 
			<< "\n\n";
#endif
	}

	Debug::ELogFilter Debug::TranslateStringTo(const char* aString)
	{
		using LF = Debug::ELogFilter;
		if (std::string(aString) == DL_RESOURCE_LOG_NAME)
			return LF::Resource;
		if (std::string(aString) == DL_ENGINE_LOG_NAME)
			return LF::Engine;
		if (std::string(aString) == DL_GAME_LOG_NAME)
			return LF::Game;

		return LF::Custom;
	}
}



/*
	//using namespace std::chrono;
	//system_clock::time_point now = system_clock::now();
	//std::time_t now_c = system_clock::to_time_t(now);
	//std::tm now_tm = *std::localtime(&now_c);
	//char buffer[256];
	////std::strftime
*/