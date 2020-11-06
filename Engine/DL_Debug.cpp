#include "stdafx.h"
#include "DL_Debug.h"
#include <iostream>
#include <time.h>

namespace DL_Debug {
    CDebug *CDebug::ourInstance = nullptr;

    void CDebug::Create(const std::string &aFile, const std::string aCatalogue) {
        assert(ourInstance == nullptr && "Debug object already created");
        ourInstance = new CDebug();
        ourInstance->myDebugFile = std::ofstream(aFile);

        ourInstance->myDebugCatalogue = aCatalogue;

        ourInstance->myLogs.emplace("resource", std::ofstream(aCatalogue + "resource.txt"));
        ourInstance->myLogFilter.emplace("resource", true);

        ourInstance->myLogs.emplace("ingame", std::ofstream(aCatalogue + "ingame.txt"));
        ourInstance->myLogFilter.emplace("ingame", true);

        ourInstance->myLogs.emplace("engine", std::ofstream(aCatalogue + "engine.txt"));
        ourInstance->myLogFilter.emplace("engine", true);
    }

    void CDebug::Destroy() {
        assert(ourInstance != nullptr && "No DL_DEBUG created!");
        ourInstance->myDebugFile.close();

        for (auto i = ourInstance->myLogs.begin(); i != ourInstance->myLogs.end(); i++) {
            (*i).second.close();
        }

        SAFE_DELETE(ourInstance);
    }

    CDebug *CDebug::GetInstance() {
        assert(ourInstance != nullptr && "DL_DEBUG NOT CREATED!");
        return(ourInstance);
    }

    void CDebug::AssertMessage(const char* aFilename, int aLine, const char* aFunctionName, const char* aMessage) {
        DL_Debug::StackWalker stack_walker;
        ourInstance->myDebugFile << "******** CALL STACK ********\n";
        stack_walker.ShowCallstack();
        ourInstance->myDebugFile << "****************************\n\n";
        ourInstance->myDebugFile << "======== CRASH AT ========\nFile: " << aFilename << "\nLine: " << aLine << "\nFunction: " << aFunctionName << "\nMessage: " << aMessage << "\n==========================\n\n";
        ourInstance->myDebugFile.close();
        exit(EXIT_FAILURE);
    }

    void CDebug::PrintMessage(const char* aMessage) {
        ourInstance->myDebugFile << aMessage << "\n\n";
    }

    void CDebug::DebugMessage(const int aLine, const char *aFilename, const char *aFormattedString, ...) {
        char buffer[256];
        va_list args;
        va_start(args, aFormattedString);
        vsprintf_s(buffer, aFormattedString, args);

        ourInstance->myDebugFile << "======== DEBUG AT ========\nLine: " << aLine << "\nFile : " << aFilename << "\nMessage: " << buffer << "\n==========================\n\n";
    }

    void CDebug::WriteLog(const char* aLog, const char* aFormattedString, ...) {
        std::string log(aLog);
        char buffer[256];
        va_list args;
        va_start(args, aFormattedString);
        vsprintf_s(buffer, aFormattedString, args);

        time_t now = time(0);
        #pragma warning(suppress : 4996)
        tm* ltm = localtime(&now);

        if (ourInstance->myLogs.find(aLog) == ourInstance->myLogs.end()) {
            ourInstance->myLogs.emplace(aLog, std::ofstream(ourInstance->myDebugCatalogue + log + ".txt"));
            ourInstance->myLogFilter.emplace(aLog, true);
        }
        
        if (ourInstance->myLogFilter[aLog]) {
            ourInstance->myLogs[aLog] << "[" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "]" << "[" << log << "] " << buffer << "\n\n";
            std::cout << "[" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "]" << "[" << log << "] " << buffer << "\n\n";
        }
    }

    void CDebug::ActivateFilterLog(const char* aFilterLog) {
        if (ourInstance->myLogFilter.find(aFilterLog) != ourInstance->myLogFilter.end()) 
            ourInstance->myLogFilter[aFilterLog] = true;
    }

    void CDebug::DeactivateFilterLog(const char* aFilterLog) {
        if (ourInstance->myLogFilter.find(aFilterLog) != ourInstance->myLogFilter.end()) 
            ourInstance->myLogFilter[aFilterLog] = false;
    }
    void CDebug::ReadCommandLineArguments(LPWSTR lpCmdLine)
    {
        std::wstring wideString(lpCmdLine);
        #pragma warning(suppress : 4244)
        std::string arguments(wideString.begin(), wideString.end());

        int argumentStride = 4; // e.g. -rd , whitespace included
        int sizeOfArgument = 3;
        for (unsigned int i = 0; i < arguments.size(); i += argumentStride) {
            std::string argument = arguments.substr(i, sizeOfArgument);

            if (argument == "-rd") 
            {
                DeactivateFilterLog("resource");
            }
            else if (argument == "-re")
            {
                ActivateFilterLog("resource");
            }
            else if (argument == "-gd")
            {
                DeactivateFilterLog("ingame");
            }
            else if (argument == "-ge")
            {
                ActivateFilterLog("ingame");
            }
            else if (argument == "-ed")
            {
                DeactivateFilterLog("engine");
            }
            else if (argument == "-ee")
            {
                ActivateFilterLog("engine");
            }
            else if (argument == "-ad")
            {
                DeactivateFilterLog("resource");
                DeactivateFilterLog("ingame");
                DeactivateFilterLog("engine");
            }
            else if (argument == "-ae")
            {
                ActivateFilterLog("resource");
                ActivateFilterLog("ingame");
                ActivateFilterLog("engine");
            }
        }
    }
}