#include "stdafx.h"
#include "WindowHandler.h"

LRESULT CALLBACK CWindowHandler::WinProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	
	static CWindowHandler* windowhandler = nullptr;

	if (uMsg == WM_DESTROY || uMsg == WM_CLOSE) {
		
		PostQuitMessage(0);
		return 0;
	} else if (uMsg == WM_CREATE) {
		CREATESTRUCT* createstruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		windowhandler = reinterpret_cast<CWindowHandler*>(createstruct->lpCreateParams);
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

CWindowHandler::CWindowHandler() {
	myWindowHandle = nullptr;
	myHeight = 0;
	myWidth = 0;
}

CWindowHandler::~CWindowHandler() {
}

bool CWindowHandler::Init(SWindowData someWindowData) {
	WNDCLASS windowclass = {};
	windowclass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowclass.lpfnWndProc = CWindowHandler::WinProc;
	windowclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowclass.lpszClassName = L"TillämpadGrafikprogrammering";
	RegisterClass(&windowclass);

	myWindowHandle = CreateWindow(L"TillämpadGrafikprogrammering", L"My nice window :)", WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE, someWindowData.myX, someWindowData.myY, someWindowData.myWidth, someWindowData.myHeight, nullptr, nullptr, nullptr, this);

	myWidth = someWindowData.myWidth;
	myHeight = someWindowData.myHeight;

	return true;
}

UINT CWindowHandler::GetWidth() const {
	LPRECT rect = new RECT{ 0, 0, 0, 0 };
	if (GetClientRect(myWindowHandle, rect) != 0) {
		return rect->right;
	}

	return myWidth;
}

UINT CWindowHandler::GetHeight() const {
	LPRECT rect = new RECT{ 0, 0, 0, 0 };
	if (GetClientRect(myWindowHandle, rect) != 0) {
		return rect->bottom;
	}

	return myHeight;
}

HWND CWindowHandler::GetWindowHandle() const {
	return myWindowHandle;
}