#include "Engine.hpp"
#include "Game.hpp"

#include "DL_Debug.h"
// Temp?
#include "InputHandler.hpp"
#include "WindowHandler.hpp"
// !Temp?
#include <Windows.h>

#include <string>
#include <atlstr.h>

#ifndef NDEBUG
#define USE_CONSOLE_COMMAND
#endif

void InitConsole()
{
#pragma warning( push )
#pragma warning( disable : 4996 )
#pragma warning( disable : 6031 )
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
#pragma warning( pop )
}

void CloseConsole()
{
#pragma warning( push )
#pragma warning( disable : 4996 )
#pragma warning( disable : 6031 )
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);
#pragma warning( pop )
}

// temp
#include <iostream>
// !temp

void HandleCMD(std::string& aCmd);
bool TranslateCMDLineArgs(LPWSTR lpCmdLine);

using namespace CommonUtilities;
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    hInstance; hPrevInstance; lpCmdLine; nShowCmd;

#ifdef USE_CONSOLE_COMMAND
    InitConsole();
#endif

    Engine myEngine;
    Game   myGame;
    DL_Debug::Debug::Create();
    if (!TranslateCMDLineArgs(lpCmdLine))
    {
        //DL_ACTIVATE_ALL_LOGS
    }

    bool shouldRun = myEngine.Init();
    if (shouldRun){
        myGame.Init();
    }
    
    MSG windowMessage = { 0 };
    while(shouldRun)
    {
        while(PeekMessage(&windowMessage, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&windowMessage);
            DispatchMessage(&windowMessage);

            if(windowMessage.message == WM_QUIT)
            {
                shouldRun = false;
            }
        }

        myEngine.BeginFrame();
        myGame.Update(myEngine.GetDeltaTime(), myEngine.GetWindowHandler()->GetInputHandler());
        myEngine.RenderFrame();
        myEngine.EndFrame();
    }

#ifdef USE_CONSOLE_COMMAND
    CloseConsole();
#endif
    return 0;
}

void HandleCMD(std::string& aCmd)
{
    std::cout << "handling cmd: "<< aCmd << std::endl;
    if (aCmd.c_str()[0] == 'f')/*(aCmd.compare("fl") == 2)*//*(*aCmd.c_str() == *std::string("fl").c_str())*/
    {
        if (aCmd.c_str()[1] == 'l' && aCmd.size() == 3)
        {
#ifdef _DEBUG
            std::cout << "Using filter logs, all activated" << std::endl;
#endif
            DL_ACTIVATE_ALL_LOGS
            return;
        }
    }
    else if(*aCmd.c_str() == 'r')
    {
#ifdef _DEBUG
        std::cout << "Using resource log" << std::endl;
#endif
        DL_ACTIVATE_LOG(DL_RESOURCE_LOG_NAME)
    }
    else if (*aCmd.c_str() == 'e')
    {
#ifdef _DEBUG
        std::cout << "Using engine log" << std::endl;
#endif
        DL_ACTIVATE_LOG(DL_ENGINE_LOG_NAME)
    }
    else if(*aCmd.c_str() == 'g')
    {
#ifdef _DEBUG
        std::cout << "Using game log" << std::endl;
#endif
        DL_ACTIVATE_LOG(DL_GAME_LOG_NAME)
    }
    else
    {
#ifdef _DEBUG
        std::cout 
            << "Unrecognized command: " << aCmd << "\n"
            << "Available commands:\n"
            << "-fl : Use filter logs\n"
            << "-r  : Use resource log\n"
            << "-e  : Use enginge log\n"
            << "-g  : Use game log\n\n";
#endif
    }
}
bool TranslateCMDLineArgs(LPWSTR lpCmdLine)
{
    std::string s(CW2A(lpCmdLine/*,nShowCmd*/).m_szBuffer);//128byte => 31chars
    if (s.length() > 0)
    {
        for (size_t i = 0; i < s.length(); ++i)
        {
            if (s.c_str()[i] == '-')
            {
                std::string cmd = "";
                while (s.c_str()[i] != ' ' && i < s.length())
                {
                    ++i;
                    cmd += s.c_str()[i];
                    if (s.c_str()[i + 1] == '-')
                        break;
                }
                std::cout << cmd << std::endl;
                HandleCMD(cmd);
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}