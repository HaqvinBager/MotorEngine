#include "Keyboard.h"

bool CKeyboard::KeyIsPressed(unsigned char keycode) const noexcept {
    return myKeyStates[keycode];
}

std::optional<CKeyboard::CEvent> CKeyboard::ReadKey() noexcept {
    if (myKeyBuffer.size() > 0u) {
        CKeyboard::CEvent e = myKeyBuffer.front();
        myKeyBuffer.pop();
        return e;
    } else {
        return CKeyboard::CEvent();
    }
}

bool CKeyboard::KeyIsEmpty() const noexcept {
    return myKeyBuffer.empty();
}

void CKeyboard::FlushKey() noexcept {
    myKeyBuffer = std::queue<CEvent>();
}

char CKeyboard::ReadChar() noexcept {
    if (myCharBuffer.size() > 0u) {
        unsigned char charcode = myCharBuffer.front();
        myCharBuffer.pop();
        return charcode;
    } else {
        return 0;
    }
}

bool CKeyboard::CharIsEmpty() const noexcept {
    return myCharBuffer.empty();
}

void CKeyboard::FlushChar() noexcept {
    myCharBuffer = std::queue<char>();
}

void CKeyboard::Flush() noexcept {
    FlushKey();
    FlushChar();
}

void CKeyboard::EnableAutorepeat() noexcept {
    myAutorepeatEnabled = true;
}

void CKeyboard::DisableAutorepeat() noexcept {
    myAutorepeatEnabled = false;
}

bool CKeyboard::AutorepeatIsEnabled() const noexcept {
    return myAutorepeatEnabled;
}

void CKeyboard::OnKeyPressed(unsigned char keycode) noexcept {
    myKeyStates[keycode] = true;
    myKeyBuffer.push(CKeyboard::CEvent(CKeyboard::CEvent::EType::Press, keycode));
    TrimBuffer(myKeyBuffer);
}

void CKeyboard::OnKeyReleased(unsigned char keycode) noexcept {
    myKeyStates[keycode] = false;
    myKeyBuffer.push(CKeyboard::CEvent(CKeyboard::CEvent::EType::Release, keycode));
    TrimBuffer(myKeyBuffer);
}

void CKeyboard::OnChar(char character) noexcept {
    myCharBuffer.push(character);
    TrimBuffer(myCharBuffer);
}

void CKeyboard::ClearState() noexcept {
    myKeyStates.reset();
}

template<typename T>
void CKeyboard::TrimBuffer(std::queue<T>& buffer) noexcept {
    while (buffer.size() > ourBufferSize) {
        buffer.pop();
    }
}
