#pragma once
//#include <Windows.h>
#include "WinInclude.h"

class CWindowHandler {
public:
	
	struct SWindowData {
	public:
		int myX;
		int myY;
		int myWidth;
		int myHeight;
	};

	CWindowHandler();
	~CWindowHandler();
	static LRESULT CALLBACK WinProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	bool Init(SWindowData someWindowData);

	UINT GetWidth() const;
	UINT GetHeight() const;
	HWND GetWindowHandle() const;

private:
	HWND myWindowHandle;
	UINT myWidth;
	UINT myHeight;
};