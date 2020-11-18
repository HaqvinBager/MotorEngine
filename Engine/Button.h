#pragma once
#include <functional>

class CCanvas;
class CSpriteInstace;

enum class EButtonState {
	Idle,
	Hover,
	Click
};

class CButton {
	friend CCanvas;
public:
	void OnHover();
	template<typename...Params>
	void OnClick(Params&&...p);
	void OnLeave();

//private:
	CButton(std::function<void()> aFunction);
	~CButton();

	bool myEnabled;
	EButtonState myState;
	std::function<void()> myFunction;
	std::array<CSpriteInstace*, 3> mySprites;
};

template<typename ...Params>
inline void CButton::OnClick(Params&& ...p)
{
	myFunction(std::forward<Params>(p)...);
}
