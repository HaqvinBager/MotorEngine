#include "Mouse.h"
#include "WinInclude.h"

std::pair<int, int> CMouse::GetPosition() const noexcept {
    return {myX, myY};
}

std::optional<CMouse::SRawDelta> CMouse::ReadRawDelta() noexcept {
    if (myRawDeltaBuffer.empty()) {
        return std::nullopt;
    }
    const SRawDelta d = myRawDeltaBuffer.front();
    myRawDeltaBuffer.pop();
    return d;
}

int CMouse::GetPositionX() const noexcept {
    return myX;
}

int CMouse::GetPositionY() const noexcept {
    return myY;
}

bool CMouse::IsInWindow() const noexcept {
    return myIsInWindow;
}

bool CMouse::LeftIsPressed() const noexcept {
    return myLeftIsPressed;
}

bool CMouse::RightIsPressed() const noexcept {
    return myRightIsPressed;
}

CMouse::CMouseEvent CMouse::Read() noexcept {
    if (myBuffer.size() > 0u) {
        CMouse::CMouseEvent e = myBuffer.front();
        myBuffer.pop();
        return e;
    } else {
        return CMouse::CMouseEvent();
    }
}

void CMouse::Flush() noexcept {
    myBuffer = std::queue<CMouseEvent>();
}

void CMouse::EnableRaw() noexcept {
    myRawEnabled = true;
}

void CMouse::DisableRaw() noexcept {
    myRawEnabled = false;
}

bool CMouse::RawEnabled() noexcept {
    return myRawEnabled;
}

void CMouse::OnMouseMove(int new_x, int new_y) noexcept {
    myX = new_x;
    myY = new_y;

    myBuffer.push(CMouse::CMouseEvent(CMouse::CMouseEvent::EType::Move, *this));
    TrimBuffer();
}

void CMouse::OnMouseEnter() noexcept {
    myIsInWindow = true;
    myBuffer.push(CMouse::CMouseEvent(CMouse::CMouseEvent::EType::Enter, *this));
    TrimBuffer();
}

void CMouse::OnMouseLeave() noexcept {
    myIsInWindow = false;
    myBuffer.push(CMouse::CMouseEvent(CMouse::CMouseEvent::EType::Leave, *this));
    TrimBuffer();
}

void CMouse::OnRawDelta(int dx, int dy) noexcept {
    myRawDeltaBuffer.push({ dx, dy });
    TrimRawInputBuffer();
}

void CMouse::OnLeftPressed(int /*x*/, int /*y*/) noexcept {
    myLeftIsPressed = true;
    myBuffer.push(CMouse::CMouseEvent(CMouse::CMouseEvent::EType::LPress, *this));
    TrimBuffer();
}

void CMouse::OnLeftReleased(int /*x*/, int /*y*/) noexcept {
    myLeftIsPressed = false;
    myBuffer.push(CMouse::CMouseEvent(CMouse::CMouseEvent::EType::LRelease, *this));
    TrimBuffer();
}

void CMouse::OnRightPressed(int /*x*/, int /*y*/) noexcept {
    myRightIsPressed = true;
    myBuffer.push(CMouse::CMouseEvent(CMouse::CMouseEvent::EType::RPress, *this));
    TrimBuffer();
}

void CMouse::OnRightReleased(int /*x*/, int /*y*/) noexcept {
    myRightIsPressed = false;
    myBuffer.push(CMouse::CMouseEvent(CMouse::CMouseEvent::EType::RRelease, *this));
    TrimBuffer();
}

void CMouse::OnWheelUp(int /*x*/, int /*y*/) noexcept {
    myBuffer.push(CMouse::CMouseEvent(CMouse::CMouseEvent::EType::WheelUp, *this));
    TrimBuffer();
}

void CMouse::OnWheelDown(int /*x*/, int /*y*/) noexcept {
    myBuffer.push(CMouse::CMouseEvent(CMouse::CMouseEvent::EType::WheelDown, *this));
    TrimBuffer();
}

void CMouse::TrimBuffer() noexcept {
    while (myBuffer.size() > ourBufferSize) {
        myBuffer.pop();
    }
}

void CMouse::TrimRawInputBuffer() noexcept {
    while (myRawDeltaBuffer.size() > ourBufferSize) {
        myRawDeltaBuffer.pop();
    }
}

void CMouse::OnWheelDelta(int x, int y, int delta) noexcept {
    myWheelDeltaCarry += delta;

    while (myWheelDeltaCarry >= WHEEL_DELTA) {
        myWheelDeltaCarry -= WHEEL_DELTA;
        OnWheelUp(x, y);
    }
    while (myWheelDeltaCarry <= -WHEEL_DELTA) {
        myWheelDeltaCarry += WHEEL_DELTA;
        OnWheelDown(x, y);
    }
}
