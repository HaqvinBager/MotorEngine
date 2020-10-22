#include "pch.h"
#include "WindowHandler.hpp"

// CommonUtilities
#include "InputHandler.hpp"

#ifdef _DEBUG
#pragma comment(lib, "CommonUtilities_Debug.lib")
#endif // DEBUG
#ifdef NDEBUG
#pragma comment(lib, "CommonUtilities_Release.lib")

#endif // NDEBUG
CommonUtilities::InputHandler* WindowHandler::myInputHandler = new CommonUtilities::InputHandler();

WindowHandler::WindowHandler()
	: myWindowHandle(nullptr)
	, myWindowData({})
	//, myInputHandler(new CommonUtilities::InputHandler())
{}
	
WindowHandler::~WindowHandler()
{
	SAFE_DELETE(myInputHandler)
}

HWND& WindowHandler::GetWindowhandle()
{
	return myWindowHandle;
}

LRESULT CALLBACK WindowHandler::WinProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	static WindowHandler* windowHandler = nullptr;
	// Call input handler update events
	myInputHandler->UpdateEvents(uMsg, wParam, lParam);

	if(uMsg == WM_DESTROY || uMsg == WM_CLOSE)
	{
		PostQuitMessage(0);
		return 0;
	}
	else if(uMsg == WM_CREATE)
	{
		CREATESTRUCT* createstruct	= reinterpret_cast<CREATESTRUCT*>(lParam);
		windowHandler				= reinterpret_cast<WindowHandler*>(createstruct->lpCreateParams);
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool WindowHandler::Init(SWindowData someWindowData)
{
	WNDCLASS windowclass = {};
	windowclass.style			= CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowclass.lpfnWndProc		= WindowHandler::WinProc;
	windowclass.hCursor			= LoadCursor(nullptr, IDC_ARROW);
	windowclass.lpszClassName	= L"TillämpadGrafikProgrammering";
	//windowclass.hIcon			= LoadIcon(NULL, L"ghosticon.ico");
	RegisterClass(&windowclass);

	myWindowHandle = CreateWindow(windowclass.lpszClassName, L"Game, The"
								  , WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE
								  , someWindowData.myX, someWindowData.myY, someWindowData.myWidth, someWindowData.myHeight
								  , nullptr, nullptr, nullptr, this);
	myWindowData = someWindowData;

	return true;
}

const int/*change to UINT*/ WindowHandler::GetWidth()
{
	LPRECT rect = new RECT{ 0,0,0,0 };
	if (GetClientRect(myWindowHandle, rect) != 0) {
		return rect->right;
	}
	return myWindowData.myWidth;
}

const int WindowHandler::GetHeight()
{
	LPRECT rect = new RECT{ 0,0,0,0 };
	if (GetClientRect(myWindowHandle, rect) != 0) {
		return rect->bottom;
	}
	return myWindowData.myHeight;
}

CommonUtilities::InputHandler& WindowHandler::GetInputHandler()
{
	return *myInputHandler;
}
