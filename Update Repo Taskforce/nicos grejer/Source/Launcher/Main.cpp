#include "WinInclude.h"
#include "Engine.h"
#include "Game.h"
#include "Input.h"
#include "DL_Debug.h"

#ifdef _DEBUG
#pragma comment(lib, "Game_Debug.lib")
#endif // _DEBUG
#ifdef NDEBUG
#pragma comment(lib, "Game_Release.lib")
#endif // NDEBUG

#ifndef NDEBUG 
#define USE_CONSOLE_COMMAND
#define USE_DEBUG_LOG
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

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	hInstance;
	hPrevInstance;
	lpCmdLine;
	nShowCmd;

#ifdef USE_CONSOLE_COMMAND
	InitConsole();
#endif

	DL_Debug::CDebug::Create();
	DL_Debug::CDebug::GetInstance()->ReadCommandLineArguments(lpCmdLine);
	DL_Debug::CDebug::GetInstance()->WriteLog("resource", "resource message");
	DL_Debug::CDebug::GetInstance()->WriteLog("ingame", "ingame message");
	DL_Debug::CDebug::GetInstance()->WriteLog("engine", "engine message");

	CWindowHandler::SWindowData windowData;
	windowData.myX = 100;
	windowData.myY = 100;
	windowData.myWidth = 1600;
	windowData.myHeight = 900;

	CEngine engine;
	bool shouldRun = engine.Init(windowData);
	if (!shouldRun)
	{
		return 1;
	}

	CGame game;
	game.Init();

	MSG windowMessage = { 0 };
	while (shouldRun)
	{
		while (PeekMessage(&windowMessage, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&windowMessage);
			DispatchMessage(&windowMessage);
			
			if (windowMessage.message == WM_QUIT)
			{
				shouldRun = false;
			}
		}
		const auto dt = engine.BeginFrame();
		game.Update(dt);
		engine.RenderFrame();
		engine.EndFrame();
		CommonUtilities::Input::GetInstance()->update();
	}

#ifdef USE_CONSOLE_COMMAND
	CloseConsole();
#endif

	DL_Debug::CDebug::Destroy();

	return 0;
}