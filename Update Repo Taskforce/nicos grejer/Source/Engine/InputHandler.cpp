#include "stdafx.h"
#include "InputHandler.h"
#include <iostream>
#include <cassert>
#pragma comment(lib, "Xinput9_1_0.lib")

using std::bitset;

CommonUtilities::InputHandler* CommonUtilities::InputHandler::main_handler = nullptr;

bool CommonUtilities::InputHandler::update_events(UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_KEYDOWN:
	{
		_key_down[wParam] = true;
		return true;
	} break;
	case WM_KEYUP:
	{
		_key_down[wParam] = false;
		return true;
	} break;
	case WM_MOUSEMOVE:
	{
		_mouse_x = GET_X_LPARAM(lParam);
		_mouse_y = GET_Y_LPARAM(lParam);
		return true;
	} break;
	case WM_MOUSEWHEEL:
	{
		_mouse_wheel += GET_WHEEL_DELTA_WPARAM(wParam);
		return true;
	} break;
	case WM_LBUTTONDOWN:
	{
		_mouse_down[static_cast<int>(MouseButton::Left)] = true;
		return true;
	} break;
	case WM_LBUTTONUP:
	{
		_mouse_down[static_cast<int>(MouseButton::Left)] = false;
		return true;
	} break;
	case WM_RBUTTONDOWN:
	{
		_mouse_down[static_cast<int>(MouseButton::Right)] = true;
		return true;
	} break;
	case WM_RBUTTONUP:
	{
		_mouse_down[static_cast<int>(MouseButton::Right)] = false;
		return true;
	} break;
	case WM_MBUTTONDOWN:
	{
		_mouse_down[static_cast<int>(MouseButton::Middle)] = true;
		return true;
	} break;
	case WM_MBUTTONUP:
	{
		_mouse_down[static_cast<int>(MouseButton::Middle)] = false;
		return true;
	} break;
	case WM_XBUTTONDOWN:
	{
		if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) {
			_mouse_down[static_cast<int>(MouseButton::Mouse4)] = true;
		} else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2) {
			_mouse_down[static_cast<int>(MouseButton::Mouse5)] = true;
		}
		return true;
	} break;
	case WM_XBUTTONUP:
	{
		if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) {
			_mouse_down[static_cast<int>(MouseButton::Mouse4)] = false;
		} else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2) {
			_mouse_down[static_cast<int>(MouseButton::Mouse5)] = false;
		}
		return true;
	} break;
	default:
	{
		return false;
	} break;
	}
}

void CommonUtilities::InputHandler::update() {

	_key_down_last = _key_down;
	_mouse_down_last = _mouse_down;
	_mouse_wheel = 0;
	_mouse_x_last = _mouse_x;
	_mouse_y_last = _mouse_y;

	POINT point;

	if (GetCursorPos(&point)) {
		_mouse_screen_x = point.x;
		_mouse_screen_y = point.y;
	}

	_controllers_controlls[static_cast<int>(GamePadIndex::one)]._x_button_last = _controllers_XINPUT[static_cast<int>(GamePadIndex::one)].Gamepad.wButtons;
	_controllers_controlls[static_cast<int>(GamePadIndex::two)]._x_button_last = _controllers_XINPUT[static_cast<int>(GamePadIndex::two)].Gamepad.wButtons;
	//_controllers_controlls[static_cast<int>(GamePadIndex::three)]._x_button_last = _controllers_XINPUT[static_cast<int>(GamePadIndex::two)].Gamepad.wButtons;
	//_controllers_controlls[static_cast<int>(GamePadIndex::four)]._x_button_last = _controllers_XINPUT[static_cast<int>(GamePadIndex::two)].Gamepad.wButtons;

	x_refresh_controll(GamePadIndex::one);
	x_refresh_controll(GamePadIndex::two);
	//x_refresh_controll(GamePadIndex::three);
	//x_refresh_controll(GamePadIndex::four);
}

#pragma region Key Board & Mouse 

bool CommonUtilities::InputHandler::is_key_pressed(WPARAM wParam) const {
	return _key_down[wParam] && !_key_down_last[wParam];
}
bool CommonUtilities::InputHandler::is_key_down(WPARAM wParam) const {
	return _key_down[wParam];
}
bool CommonUtilities::InputHandler::is_key_released(WPARAM wParam) const {
	return (!_key_down[wParam]) && _key_down_last[wParam];
}

float CommonUtilities::InputHandler::scaled_mouse_x() const {
	RECT rect;
	if (GetWindowRect(*_window_handle, &rect)) {
		return static_cast<float>(_mouse_x)* _window_scale_x / static_cast<float>(rect.right - rect.left - 16);
	} else {
		return static_cast<float>(_mouse_x);
	}
}
float CommonUtilities::InputHandler::scaled_mouse_y() const {
	RECT rect;
	if (GetWindowRect(*_window_handle, &rect)) {
		return static_cast<float>(_mouse_y)* _window_scale_y / static_cast<float>(rect.bottom - rect.top - 39);
	} else {
		return static_cast<float>(_mouse_y);
	}
}
float CommonUtilities::InputHandler::scaled_mouse_delta_x() const {
	RECT rect;
	if (GetWindowRect(*_window_handle, &rect)) {
		return static_cast<float>(_mouse_x - _mouse_x_last)* _window_scale_x / static_cast<float>(rect.right - rect.left - 16);
	} else {
		return static_cast<float>(_mouse_x - _mouse_x_last);
	}
}
float CommonUtilities::InputHandler::scaled_mouse_delta_y() const {
	RECT rect;
	if (GetWindowRect(*_window_handle, &rect)) {
		return static_cast<float>(_mouse_y - _mouse_y_last)* _window_scale_y / static_cast<float>(rect.bottom - rect.top - 39);
	} else {
		return static_cast<float>(_mouse_y - _mouse_y_last);
	}
}
int CommonUtilities::InputHandler::mouse_x() const {
	return _mouse_x;
}
int CommonUtilities::InputHandler::mouse_y() const {
	return _mouse_y;
}
int CommonUtilities::InputHandler::mouse_delta_x() const {
	return _mouse_x - _mouse_x_last;
}
int CommonUtilities::InputHandler::mouse_delta_y() const {
	return _mouse_y - _mouse_y_last;
}
int CommonUtilities::InputHandler::mouse_screen_x() const {
	return _mouse_screen_x;
}
int CommonUtilities::InputHandler::mouse_screen_y() const {
	return _mouse_screen_y;
}
int CommonUtilities::InputHandler::mouse_wheel() const {
	return _mouse_wheel;
}
void CommonUtilities::InputHandler::set_mouse_screen_position(int x, int y) {
	SetCursorPos(x, y);
}

bool CommonUtilities::InputHandler::is_mouse_pressed(MouseButton mouse_button) const {
	return _mouse_down[static_cast<int>(mouse_button)] && !_mouse_down_last[static_cast<int>(mouse_button)];
}
bool CommonUtilities::InputHandler::is_mouse_down(MouseButton mouse_button) const {
	return _mouse_down[static_cast<int>(mouse_button)];
}
bool CommonUtilities::InputHandler::is_mouse_released(MouseButton mouse_button) const {
	return (!_mouse_down[static_cast<int>(mouse_button)]) && _mouse_down_last[static_cast<int>(mouse_button)];
}

#pragma endregion

#pragma region XInput GamePad

XINPUT_GAMEPAD* CommonUtilities::InputHandler::x_state_controll(const GamePadIndex index) {
	return &_controllers_XINPUT[static_cast<int>(index)].Gamepad;
}

bool CommonUtilities::InputHandler::x_check_connection_controll(const GamePadIndex index) {
	assert(static_cast<int>(index) < XUSER_MAX_COUNT);

	int controller_id = -1;

	for (DWORD i = 0; i < XUSER_MAX_COUNT && controller_id == -1; ++i) {
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		if (XInputGetState(i, &state) == ERROR_SUCCESS) {
			controller_id = i;
		}
	}

	_controllers_controlls[static_cast<int>(index)]._x_controller_id = controller_id;

	return _controllers_controlls[static_cast<int>(index)]._x_controller_id == static_cast<int>(index);
}

bool CommonUtilities::InputHandler::x_refresh_controll(const GamePadIndex index) {
	assert(static_cast<int>(index) < XUSER_MAX_COUNT);

	++_controllers_controlls[static_cast<int>(index)]._x_check_interval;

	_controllers_XINPUT[static_cast<int>(index)].Gamepad.wButtons;

	if (_controllers_controlls[static_cast<int>(index)]._x_controller_id == -1 && _controllers_controlls[static_cast<int>(index)]._x_check_interval >= X_CHECK_INTERVAL) {
		x_check_connection_controll(index);
		_controllers_controlls[static_cast<int>(index)]._x_check_interval = 0;
	}

	if (_controllers_controlls[static_cast<int>(index)]._x_controller_id != -1) {

		ZeroMemory(&_controllers_XINPUT[static_cast<int>(index)], sizeof(XINPUT_STATE));
		if (XInputGetState(_controllers_controlls[static_cast<int>(index)]._x_controller_id, &_controllers_XINPUT[static_cast<int>(index)]) != ERROR_SUCCESS) {
			_controllers_controlls[static_cast<int>(index)]._x_controller_id = -1;
			return false;
		}

		float left_x = fmaxf(-1, (float)_controllers_XINPUT[static_cast<int>(index)].Gamepad.sThumbLX / 32767);
		float left_y = fmaxf(-1, (float)_controllers_XINPUT[static_cast<int>(index)].Gamepad.sThumbLY / 32767);

		float left_distance = sqrtf((left_x * left_x) + (left_y * left_y));

		if (left_distance < _controllers_controlls[static_cast<int>(index)]._x_deadzone) {
			left_distance = 0.0f;
		} else {
			left_distance -= _controllers_controlls[static_cast<int>(index)]._x_deadzone;
			left_distance /= (1.0f - _controllers_controlls[static_cast<int>(index)]._x_deadzone);
		}

		if (left_distance > 1.0f) {
			left_x /= left_distance;
			left_y /= left_distance;
		}

		_controllers_controlls[static_cast<int>(index)]._x_left_stick_x = left_x;
		_controllers_controlls[static_cast<int>(index)]._x_left_stick_y = left_y;

		float right_x = fmaxf(-1, (float)_controllers_XINPUT[static_cast<int>(index)].Gamepad.sThumbRX / 32767);
		float right_y = fmaxf(-1, (float)_controllers_XINPUT[static_cast<int>(index)].Gamepad.sThumbRY / 32767);
		float right_distance = sqrtf((right_x * right_x) + (right_y * right_y));

		if (right_distance < _controllers_controlls[static_cast<int>(index)]._x_deadzone) {
			right_distance = 0.0f;
		} else {
			right_distance -= _controllers_controlls[static_cast<int>(index)]._x_deadzone;
			right_distance /= (1.0f - _controllers_controlls[static_cast<int>(index)]._x_deadzone);
		}

		if (right_distance > 1.0f) {
			right_x /= right_distance;
			right_y /= right_distance;
		}

		_controllers_controlls[static_cast<int>(index)]._x_right_stick_x = right_x;
		_controllers_controlls[static_cast<int>(index)]._x_right_stick_y = right_y;

		_controllers_controlls[static_cast<int>(index)]._x_left_trigger = (float)_controllers_XINPUT[static_cast<int>(index)].Gamepad.bLeftTrigger / 255;
		_controllers_controlls[static_cast<int>(index)]._x_right_trigger = (float)_controllers_XINPUT[static_cast<int>(index)].Gamepad.bRightTrigger / 255;
	}
	return true;
}

bool CommonUtilities::InputHandler::x_is_connected(const GamePadIndex index) const {
	assert(static_cast<int>(index) < XUSER_MAX_COUNT);

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	if (XInputGetState(static_cast<int>(index), &state) != ERROR_SUCCESS) {
		return false;
	} 
		return true;
}

bool CommonUtilities::InputHandler::x_is_pressed(const GamePadIndex index, const XButton button) const {
	assert(static_cast<int>(index) < XUSER_MAX_COUNT);
	return (_controllers_XINPUT[static_cast<int>(index)].Gamepad.wButtons& static_cast<WORD>(button)) != 0 && (_controllers_controlls[static_cast<int>(index)]._x_button_last& static_cast<WORD>(button)) == 0;
}

bool CommonUtilities::InputHandler::x_is_down(const GamePadIndex index, const XButton button) const {
	assert(static_cast<int>(index) < XUSER_MAX_COUNT);
	return (_controllers_XINPUT[static_cast<int>(index)].Gamepad.wButtons& static_cast<WORD>(button)) != 0;
}

bool CommonUtilities::InputHandler::x_is_released(const GamePadIndex index, const XButton button) const {
	assert(static_cast<int>(index) < XUSER_MAX_COUNT);
	return  (_controllers_XINPUT[static_cast<int>(index)].Gamepad.wButtons& static_cast<WORD>(button)) == 0 && (_controllers_controlls[static_cast<int>(index)]._x_button_last& static_cast<WORD>(button)) != 0;
}

bool CommonUtilities::InputHandler::x_is_left_trigger_pressed(const GamePadIndex index) const {
	assert(static_cast<int>(index) < XUSER_MAX_COUNT);
	return _controllers_controlls[static_cast<int>(index)]._x_left_trigger > X_TRIGGER_THRESHOLD;
}

float CommonUtilities::InputHandler::x_left_trigger(const GamePadIndex index) const {
	assert(static_cast<int>(index) < XUSER_MAX_COUNT);
	return _controllers_controlls[static_cast<int>(index)]._x_left_trigger;
}

float CommonUtilities::InputHandler::x_right_trigger(const GamePadIndex index) const {
	assert(static_cast<int>(index) < XUSER_MAX_COUNT);
	return  _controllers_controlls[static_cast<int>(index)]._x_right_trigger;
}

float CommonUtilities::InputHandler::x_left_stick_x(const GamePadIndex index) const {
	assert(static_cast<int>(index) < XUSER_MAX_COUNT);
	return _controllers_controlls[static_cast<int>(index)]._x_left_stick_x;
}

float CommonUtilities::InputHandler::x_left_stick_y(const GamePadIndex index) const {
	assert(static_cast<int>(index) < XUSER_MAX_COUNT);
	return   _controllers_controlls[static_cast<int>(index)]._x_left_stick_y;
}

float CommonUtilities::InputHandler::x_right_stick_x(const GamePadIndex index) const {
	assert(static_cast<int>(index) < XUSER_MAX_COUNT);
	return   _controllers_controlls[static_cast<int>(index)]._x_right_stick_x;
}

float CommonUtilities::InputHandler::x_right_stick_y(const GamePadIndex index) const {
	assert(static_cast<int>(index) < XUSER_MAX_COUNT);
	return   _controllers_controlls[static_cast<int>(index)]._x_right_stick_y;
}

#pragma endregion

void CommonUtilities::InputHandler::set_window_scale(float scale_x, float scale_y) {
	_window_scale_x = scale_x;
	_window_scale_y = scale_y;
}
void CommonUtilities::InputHandler::set_window(HWND* window_handle) {
	_window_handle = window_handle;
}

void CommonUtilities::InputHandler::set_as_main() {
	main_handler = this;

	// this is weird
	for (auto i = 0u; i < _controllers_XINPUT.size(); ++i) {
		_controllers_controlls[i]._x_controller_id = i;
	}


}

CommonUtilities::InputHandler* CommonUtilities::InputHandler::main() {
	return main_handler;
}
