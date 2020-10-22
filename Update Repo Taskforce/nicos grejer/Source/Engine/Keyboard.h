#pragma once
#include <queue>
#include <bitset>
#include <optional>

class CKeyboard {
	friend class CWindowHandler;
public:
	class CEvent {
	public:
		enum class EType {
			Press,
			Release,
			Invalid
		};
	private:
		EType myType;
		unsigned char myCode;
	public:
		CEvent() noexcept : myType(EType::Invalid), myCode(0u) {}
		CEvent(EType type, unsigned char code) noexcept :
			myType(type),
			myCode(code) {}

		bool IsPress() const noexcept {
			return myType == EType::Press;
		}
		bool IsRelease() const noexcept {
			return myType == EType::Release;
		}
		bool IsValid() const noexcept {
			return myType != EType::Invalid;
		}
		unsigned char GetCode() const noexcept {
			return myCode;
		}
	};

public:
	CKeyboard() = default;
	CKeyboard(const CKeyboard&) = delete;
	CKeyboard& operator=(const CKeyboard&) = delete;
	bool KeyIsPressed(unsigned char keycode) const noexcept;
	std::optional<CEvent> ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void FlushKey() noexcept;
	char ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void FlushChar() noexcept;
	void Flush() noexcept;
	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool AutorepeatIsEnabled() const noexcept;
private:
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;
private:
	static constexpr unsigned int ourNumberOfKeys    = 256u;
	static constexpr unsigned int ourBufferSize		 = 16u;
	bool myAutorepeatEnabled = false;
	std::bitset<ourNumberOfKeys> myKeyStates;
	std::queue<CEvent> myKeyBuffer;
	std::queue<char> myCharBuffer;
};
