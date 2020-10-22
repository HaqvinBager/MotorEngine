#pragma once
#include "windows.h"

namespace CommonUtilities
{
	class InputHandler;
}

class WindowHandler {
public:
	struct SWindowData {
		int myX;
		int myY;
		int myWidth;
		int myHeight;
	};

	WindowHandler();
	~WindowHandler();

	HWND& GetWindowhandle();

	static LRESULT CALLBACK WinProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	bool Init(SWindowData someWindowData);

	const int GetWidth();
	const int GetHeight();

	CommonUtilities::InputHandler& GetInputHandler();

private:
	HWND		myWindowHandle;
	SWindowData myWindowData;

	// Temp
	static CommonUtilities::InputHandler* myInputHandler;
	// !Temp
};

