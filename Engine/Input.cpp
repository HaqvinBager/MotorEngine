#include "stdafx.h"
#include "Input.h"

Input* Input::GetInstance()
{
	static Input* input = new Input();
	return input;
}

Input::Input() {
	_mouse_x = 0;
	_mouse_y = 0;
	_mouse_x_last = 0;
	_mouse_y_last = 0;
	_mouse_wheel = 0;
	_mouse_screen_x = 0;
	_mouse_screen_y = 0;
}

bool Input::update_events(UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {
	case WM_KEYDOWN:
		_key_down[wParam] = true;
		return true;
	case WM_KEYUP:
		_key_down[wParam] = false;
		return true;
	case WM_MOUSEMOVE:
		_mouse_x = GET_X_LPARAM(lParam);//returns x coordiante
		_mouse_y = GET_Y_LPARAM(lParam);//returns y coordinate
		return true;
	case WM_MOUSEWHEEL:
		_mouse_wheel += GET_WHEEL_DELTA_WPARAM(wParam);//returns difference in mouse wheel position
		return true;
	case WM_LBUTTONDOWN:
		_mouse_button[(int)MouseButton::Left] = true;
		return true;
	case WM_LBUTTONUP:
		_mouse_button[(int)MouseButton::Left] = false;
		return true;
	case WM_RBUTTONDOWN:
		_mouse_button[(int)MouseButton::Right] = true;
		return true;
	case WM_RBUTTONUP:
		_mouse_button[(int)MouseButton::Right] = false;
		return true;
	case WM_MBUTTONDOWN:
		_mouse_button[(int)MouseButton::Middle] = true;
		return true;
	case WM_MBUTTONUP:
		_mouse_button[(int)MouseButton::Middle] = false;
		return true;
	case WM_XBUTTONDOWN:
		if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) {
			_mouse_button[(int)MouseButton::Mouse4] = true;
		} else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2) {
			_mouse_button[(int)MouseButton::Mouse5] = true;
		}
		return true;
	case WM_XBUTTONUP:
		if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) {
			_mouse_button[(int)MouseButton::Mouse4] = false;
		} else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2) {
			_mouse_button[(int)MouseButton::Mouse5] = false;
		}
	}

	return false;
}

void Input::update() {
	_key_down_last = _key_down;

	_mouse_x_last = _mouse_x;
	_mouse_y_last = _mouse_y;
	_mouse_wheel = 0;
	_mouse_button_last = _mouse_button;

	POINT point;
	if (GetCursorPos(&point)) {
		_mouse_screen_x = point.x;
		_mouse_screen_y = point.y;
	}
}

bool Input::move_left() {
	return IsKeyPressed('A') == true || IsKeyPressed(VK_LEFT) == true;
}

bool Input::move_right() {
	return IsKeyPressed('D') == true || IsKeyPressed(VK_RIGHT) == true;
}

bool Input::move_up() {
	return IsKeyPressed('W') == true || IsKeyPressed(VK_UP) == true;
}

bool Input::MoveDown() {
	return IsKeyPressed('S') == true || IsKeyPressed(VK_DOWN) == true;
}

bool Input::IsKeyDown(WPARAM wParam) {
	return _key_down[wParam];
}

bool Input::IsKeyPressed(WPARAM wParam) {
	return _key_down[wParam] && (!_key_down_last[wParam]);
}

bool Input::IsKeyReleased(WPARAM wParam) {
	return (!_key_down[wParam]) && _key_down_last[wParam];
}

int Input::MouseX() {
	return _mouse_x;
}

int Input::MouseY() {
	return _mouse_y;
}

int Input::MouseScreenX() {
	return _mouse_screen_x;
}

int Input::MouseScreenY() {
	return _mouse_screen_y;
}

int Input::MouseDeltaX() {
	return (_mouse_x - _mouse_x_last);
}

int Input::MouseDeltaY() {
	return (_mouse_y - _mouse_y_last);
}

int Input::MouseWheel() {
	return _mouse_wheel;
}

void Input::SetMouseScreenPosition(int x, int y) {
	SetCursorPos(x, y);
}

bool Input::IsMouseDown(MouseButton mouse_button) {
	return _mouse_button[(int)mouse_button];
}

bool Input::IsMousePressed(MouseButton mouse_button) {
	return _mouse_button[(int)mouse_button] && (!_mouse_button_last[(int)mouse_button]);
}

bool Input::IsMouseReleased(MouseButton mouse_button) {
	return (!_mouse_button[(int)mouse_button]) && _mouse_button_last[(int)mouse_button];
}