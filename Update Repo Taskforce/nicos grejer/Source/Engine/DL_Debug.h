#pragma once
#include "DL_StackWalker.h"
#include <string>
#include <cassert>
#include <assert.h>
#include <fstream>
#include <stdarg.h>
#include <map>
#include "Macro.h"

#define DL_ASSERT(string) DL_Debug::CDebug::GetInstance()->AssertMessage(__FILE__, __LINE__, __FUNCTION__, string);
#define DL_PRINT(string) DL_Debug::CDebug::GetInstance()->PrintMessage(string);
#define DL_DEBUG( ... ) DL_Debug::CDebug::GetInstance()->DebugMessage(__LINE__, __FUNCTION__, __VA_ARGS__);
#define DL_WRITELOG(log, ...) DL_Debug::CDebug::GetInstance()->WriteLog(log, __VA_ARGS__);

#ifdef USE_FILTERLOG
#define RESOURCE_LOG(...) DL_WRITELOG("resource", __VA_ARGS__)
#define INGAME_LOG(...) DL_WRITELOG("ingame", __VA_ARGS__)
#define ENGINE_LOG(...) DL_WRITELOG("engine", __VA_ARGS__)
#else
#define RESOURCE_LOG(...)
#define INGAME_LOG(...)
#define ENGING_LOG(...)
#endif // USE_FILTERLOG


namespace DL_Debug {
    class CDebug {
    public:
        static void Create(const std::string &aFile = "DebugLogs/DebugLog.txt", const std::string aCatalogue = "DebugLogs/");
        static void Destroy();
        static CDebug *GetInstance();
        void AssertMessage(const char *aFilename, int aLine, const char *aFunctionName, const char * aMessage);
        void PrintMessage(const char * aMessage);
        void DebugMessage(const int aLine, const char *aFilename, const char *aFormattedString, ...);
        void WriteLog(const char* aLog, const char* aFormattedString, ...);
        void ActivateFilterLog(const char* aFilterLog);
        void DeactivateFilterLog(const char* aFilterLog);
        void ReadCommandLineArguments(LPWSTR lpCmdLine);

    private:
        static CDebug *ourInstance;
        CDebug() {};
        ~CDebug() {};
        std::ofstream myDebugFile;
        std::string myDebugCatalogue;
        std::map<const char*, std::ofstream> myLogs;
        std::map<const char*, bool> myLogFilter;
    };
}