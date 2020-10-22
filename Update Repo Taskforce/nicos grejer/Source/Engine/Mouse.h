#pragma once
#include <queue>
#include <optional>

class CMouse {
	friend class Window;
public:
	struct SRawDelta {
		int myX, myY;
	};
	class CMouseEvent {
	public:
		enum class EType {
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};
	private:
		EType myType;
		bool myLeftIsPressed;
		bool myRightIsPressed;
		int myX;
		int myY;
	public:
		CMouseEvent() noexcept :
			myType(EType::Invalid), 
			myLeftIsPressed(false),
		    myRightIsPressed(false),
		    myX(0),
		    myY(0) {}
		CMouseEvent(EType aType, const CMouse& aParent) noexcept :
			myType(aType),
			myLeftIsPressed(aParent.myLeftIsPressed),
			myRightIsPressed(aParent.myRightIsPressed),
			myX(aParent.myX),
			myY(aParent.myY) {}

		bool IsValid() const noexcept {
			return myType != EType::Invalid;
		}
		EType GetType() const noexcept {
			return myType;
		}
		std::pair<int, int> GetPosition() const noexcept {
			return std::pair<int, int>(myX, myY);
		}
		int GetPositionX() const noexcept {
			return myX;
		}
		int GetPositionY() const noexcept {
			return myY;
		}
		bool LeftIsPressed() const noexcept {
			return myLeftIsPressed;
		}
		bool RightIsPressed() const noexcept {
			return myRightIsPressed;
		}
	};

public:
	CMouse() = default;
	CMouse(const CMouse&) = delete;
	CMouse& operator=(const CMouse&) = delete;
	std::pair<int, int> GetPosition() const noexcept;
	std::optional<SRawDelta> ReadRawDelta() noexcept;
	int GetPositionX() const noexcept;
	int GetPositionY() const noexcept;
	bool IsInWindow() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	CMouse::CMouseEvent Read() noexcept;
	bool IsEmpty() const noexcept {
		return myBuffer.empty();
	}
	void Flush() noexcept;
	void EnableRaw() noexcept;
	void DisableRaw() noexcept;
	bool RawEnabled() noexcept;
private:
	void OnMouseMove(int x, int y) noexcept;
	void OnMouseEnter() noexcept;
	void OnMouseLeave() noexcept;
	void OnRawDelta(int dx, int dy) noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer() noexcept;
	void TrimRawInputBuffer() noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;
private:
	static constexpr unsigned int ourBufferSize = 16u;
	int myX;
	int myY;
	bool myLeftIsPressed = false;
	bool myRightIsPressed = false;
	bool myIsInWindow = false;
	int myWheelDeltaCarry = 0;
	bool myRawEnabled;
	std::queue<CMouseEvent> myBuffer;
	std::queue<SRawDelta> myRawDeltaBuffer;
};

