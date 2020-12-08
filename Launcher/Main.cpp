#include "WinInclude.h"
#include "Engine.h"
#include "Input.h"
#include "InputMapper.h"
#include "Game.h"
#include <DbgHelp.h>
#include <strsafe.h>
#include "EngineException.h"
#include "DL_Debug.h"

#ifdef _DEBUG
struct AllocationMetrics
{
	uint32_t myTotalAllocated = 0;
	uint32_t myTotalFreed = 0;

	uint32_t CurrentUsage() { return myTotalAllocated - myTotalFreed; }
};

static AllocationMetrics gOurAllocationMetrics;

#include <iostream>
void* operator new(size_t size)
{
	/*std::cout << "Allocating " << size << " bytes " << std::endl;*/
	gOurAllocationMetrics.myTotalAllocated += static_cast<uint32_t>(size);
	return malloc(size);
}

void operator delete(void* memory, size_t size)
{
	/*std::cout << "Freeing " << size << " bytes" << std::endl;*/
	gOurAllocationMetrics.myTotalFreed += static_cast<uint32_t>(size);
	free(memory);
}


#endif
static void PrintMemoryUsage()
{
#ifdef _DEBUG
	std::cout << "Our total allocated memory: " << gOurAllocationMetrics.myTotalAllocated / static_cast<UINT32>(1048576) << " mb" << std::endl;
	std::cout << "Our current memory usage:   " << gOurAllocationMetrics.CurrentUsage() / static_cast<uint32_t>(1048576) << " mb" << std::endl;
	std::cout << "Our total freed memory:     " << gOurAllocationMetrics.myTotalFreed / static_cast<UINT32>(1048576) << " mb" << std::endl;
#endif
}

#ifdef _DEBUG
#pragma comment(lib, "Game_Debug.lib")
#pragma comment(lib, "dbghelp.lib")
#define USE_DEBUG_LOG
#define USE_FILTER_LOG
#endif // _DEBUG
#ifdef NDEBUG
#pragma comment(lib, "Game_Release.lib")
#pragma comment(lib, "dbghelp.lib")
#endif // NDEBUG

#ifdef _DEBUG
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
	fclose(stdin); 
	fclose(stdout);
	fclose(stderr);
#pragma warning( pop )
}

int CreateMiniDump(EXCEPTION_POINTERS* someExceptionPointers, std::wstring &aSubPath)
{
	BOOL bMiniDumpSuccessful;
	WCHAR szPath[MAX_PATH] = L"Crashes";
	WCHAR szSubPath[MAX_PATH];
	WCHAR szFileName[MAX_PATH];
	//const WCHAR* szAppName = L"Crashes";
	const WCHAR* szName = L"MiniDump";
	//DWORD dwBufferSize = MAX_PATH;
	HANDLE hDumpFile;
	SYSTEMTIME stLocalTime;
	MINIDUMP_EXCEPTION_INFORMATION ExpParam;

	GetLocalTime(&stLocalTime);
	//GetTempPath(dwBufferSize, szPath);

	StringCchPrintf(szFileName, MAX_PATH, L"%s", szPath);
	CreateDirectory(szFileName, NULL);

	StringCchPrintf(szSubPath, MAX_PATH, L"%s\\%s_%02d%02d_%02d%02d_ThreadID%ld",
		szPath, L"Crash", stLocalTime.wMonth, stLocalTime.wDay, stLocalTime.wHour, stLocalTime.wMinute, GetCurrentThreadId());
	CreateDirectory(szSubPath, NULL);

	StringCchPrintf(szFileName, MAX_PATH, L"%s\\%s.dmp", szSubPath, szName);

	hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

	ExpParam.ThreadId = GetCurrentThreadId();
	ExpParam.ExceptionPointers = someExceptionPointers;
	ExpParam.ClientPointers = TRUE;

	bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
		hDumpFile, MiniDumpWithDataSegs, &ExpParam, NULL, NULL);

	OutputDebugStringW(szPath);

	aSubPath = szSubPath;
	return EXCEPTION_EXECUTE_HANDLER;
}

LONG WINAPI ExceptionFilterFunction(_EXCEPTION_POINTERS* aExceptionP, std::wstring &aSubPath)
{
	CreateMiniDump(aExceptionP, aSubPath);
	return EXCEPTION_EXECUTE_HANDLER;
}

void RunGame(LPWSTR lpCmdLine) 
{
	DL_Debug::CDebug::Create();
	DL_Debug::CDebug::GetInstance()->ReadCommandLineArguments(lpCmdLine);
	DL_Debug::CDebug::GetInstance()->WriteLog("resource", "resource message");
	DL_Debug::CDebug::GetInstance()->WriteLog("ingame", "ingame message");
	DL_Debug::CDebug::GetInstance()->WriteLog("engine", "engine message");

	// THIS IS KEPT FOR SAFETY ONLY. To change resolution, change in Json/WindowSettings.json
	CWindowHandler::SWindowData windowData;
	windowData.myX = 100;
	windowData.myY = 100;
	windowData.myWidth = 1280;
	windowData.myHeight = 720;

	static CEngine engine;

	bool shouldRun = engine.Init(windowData);
	if (!shouldRun)
		return;

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

		if (!shouldRun)
			break;

		//PrintMemoryUsage();
		engine.BeginFrame();
		shouldRun = game.Update();
		engine.RenderFrame();
		engine.EndFrame();
		CInputMapper::GetInstance()->Update();
	}

	DL_Debug::CDebug::Destroy();
}

	std::wstring subPath = L"";

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	hInstance;
	hPrevInstance;
	lpCmdLine;
	nShowCmd;


#ifdef USE_CONSOLE_COMMAND
	InitConsole();
#endif

	//__try {
		RunGame(lpCmdLine);
	//}
	//__except (ExceptionFilterFunction(GetExceptionInformation(), subPath)) 
	//{
	//	CEngine::GetInstance()->CrashWithScreenShot(subPath);
	//	ENGINE_ERROR_BOOL_MESSAGE(false, "Program crashed! A minidump was created at Bin/Crashes, please tell a programmer.");
	//}

#ifdef USE_CONSOLE_COMMAND
	CloseConsole();
#endif

	return 0;
}