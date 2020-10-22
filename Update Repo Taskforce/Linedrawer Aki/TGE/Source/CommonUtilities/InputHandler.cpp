#include "InputHandler.hpp"

CommonUtilities::InputHandler::InputHandler()
{
	myMouseX		= 0;
	myMouseY		= 0;
	myMouseXLast	= 0;
	myMouseYLast	= 0;
	myMouseWheel	= 0;
	myMouseScreenX	= 0;
	myMouseScreenY	= 0;

}

bool CommonUtilities::InputHandler::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message) 
	{
		case WM_KEYDOWN:
		{
			myKeyDown[wParam] = true;
			return true;
		}
		break;

		case WM_KEYUP:
		{
			myKeyDown[wParam] = false;
			return true;
		}
		break;

		case WM_MOUSEMOVE:
		{
			myMouseX = GET_X_LPARAM(lParam);
			myMouseY = GET_Y_LPARAM(lParam);
			return true;
		}
		break;

		case WM_MOUSEWHEEL:
		{
			myMouseWheel += GET_WHEEL_DELTA_WPARAM(wParam );
			return true;
		}
		break;

		case WM_LBUTTONDOWN:
		{
			myMouseButton[(int)EMouseButton::Left] = true;
			return true;
		}
		break;

		case WM_LBUTTONUP:
		{
			myMouseButton[(int)EMouseButton::Left] = false;
			return true;
		}
		break;

		case WM_RBUTTONDOWN:
		{
			myMouseButton[(int)EMouseButton::Right] = true;
			return true;
		}
		break;

		case WM_RBUTTONUP:
		{
			myMouseButton[(int)EMouseButton::Right] = false;
			return true;
		}break;

		case WM_MBUTTONDOWN:
		{
			myMouseButton[(int)EMouseButton::Middle] = true;
			return true;
		}
		break;

		case WM_MBUTTONUP:
		{
			myMouseButton[(int)EMouseButton::Middle] = false;
			return true;
		}
		break;

		case WM_XBUTTONDOWN:
		{
			if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) 
			{
				myMouseButton[(int)EMouseButton::Mouse4] = true;
			}
			else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2) 
			{
				myMouseButton[(int)EMouseButton::Mouse5] = true;
			}
			return true;
		}
		break;

		case WM_XBUTTONUP:
		{
			if (GET_XBUTTON_WPARAM(wParam ) == XBUTTON1) 
			{
				myMouseButton[(int)EMouseButton::Mouse4] = false;
			}
			else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2) 
			{
				myMouseButton[(int)EMouseButton::Mouse5] = false;
			}
			return true;
		}break;

		default:
		{
			return false;
		}
		break;
	}
}

void CommonUtilities::InputHandler::Update() {
	myKeyDownLast = myKeyDown;

	myMouseXLast		= myMouseX;
	myMouseYLast		= myMouseY;
	myMouseWheel		= 0;
	myMouseButtonLast	= myMouseButton;

	POINT point;
	if (GetCursorPos(&point)) {
		myMouseScreenX = point.x;
		myMouseScreenY = point.y;
	}
}

bool CommonUtilities::InputHandler::IsKeyDown(WPARAM wParam) {
	return myKeyDown[wParam];
}

bool CommonUtilities::InputHandler::IsKeyPressed(WPARAM wParam) {
	return myKeyDown[wParam] && (!myKeyDownLast[wParam]);
}

bool CommonUtilities::InputHandler::IsKeyReleased(WPARAM wParam) {
	return (!myKeyDown[wParam]) && myKeyDownLast[wParam];
}

int CommonUtilities::InputHandler::MouseWindowX() {
	return myMouseX;
}

int CommonUtilities::InputHandler::MouseWindowY() {
	return myMouseY;
}

int CommonUtilities::InputHandler::MouseScreenX() {
	return myMouseScreenX;
}

int CommonUtilities::InputHandler::MouseScreenY() {
	return myMouseScreenY;
}

int CommonUtilities::InputHandler::MouseDeltaX() {
	return (myMouseX - myMouseXLast);
}

int CommonUtilities::InputHandler::MouseDeltaY() {
	return (myMouseY - myMouseYLast);
}

int CommonUtilities::InputHandler::MouseWheel() {
	return myMouseWheel;
}

void CommonUtilities::InputHandler::SetMousePosition(int x ,int y) {
	SetCursorPos( x, y );
}

bool CommonUtilities::InputHandler::IsMouseDown(EMouseButton aMouseButton) {
	return myMouseButton[(int)aMouseButton];
}

bool CommonUtilities::InputHandler::IsMousePressed(EMouseButton aMouseButton) {
	return myMouseButton[(int)aMouseButton] && (!myMouseButtonLast[(int)aMouseButton]);
}

bool CommonUtilities::InputHandler::IsMouseReleased(EMouseButton aMouseButton) {
	return (!myMouseButton[(int)aMouseButton]) && myMouseButtonLast[(int)aMouseButton];
}