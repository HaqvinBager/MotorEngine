#pragma once
#include "WinInclude.h"
#include <string>

class CWindowHandler
{
public:
	struct SWindowData
	{
		UINT myX;
		UINT myY;
		UINT myWidth;
		UINT myHeight;
	};

	static LRESULT CALLBACK WinProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	CWindowHandler();
	~CWindowHandler();

	bool Init(CWindowHandler::SWindowData someWindowData);
	const HWND GetWindowHandle() const;

	UINT GetWidth() const;
	UINT GetHeight() const;

	void SetWindowTitle(std::string aString);
	//void HandleInput(float dt);

private:
	CWindowHandler::SWindowData myWindowData;
	HWND myWindowHandle;

	//CKeyboard myKeyboard;
	//CMouse myMouse;
};

