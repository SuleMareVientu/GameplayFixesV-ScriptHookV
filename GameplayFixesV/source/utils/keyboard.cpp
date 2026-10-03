#include "utils\keyboard.h"

class KeyboardManager
{
public:
	static constexpr int KeyCount = 256;
	static constexpr int NowPeriodMs = 100;
	static constexpr int MaxDownMs = 5000;

	struct KeyState {
		ULONGLONG time = 0;
		BOOL isWithAlt = FALSE;
		BOOL wasDownBefore = FALSE;
		BOOL isUpNow = FALSE;
	};

	static KeyboardManager& Instance()
	{
		static KeyboardManager instance;
		return instance;
	}

	void OnMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow)
	{
		if (IsValidKey(key))
		{
			m_keyStates[key].time = GetTickCount64();
			m_keyStates[key].isWithAlt = isWithAlt;
			m_keyStates[key].wasDownBefore = wasDownBefore;
			m_keyStates[key].isUpNow = isUpNow;
		}
	}

	bool IsDown(DWORD key) const
	{
		if (!IsValidKey(key) || m_keyStates[key].time == 0)
			return false;

		return (GetTickCount64() < m_keyStates[key].time + MaxDownMs) && !m_keyStates[key].isUpNow;
	}

	bool IsJustUp(DWORD key, bool exclusive = true)
	{
		if (!IsValidKey(key) || m_keyStates[key].time == 0)
			return false;

		const bool justUp = (GetTickCount64() < m_keyStates[key].time + NowPeriodMs) && m_keyStates[key].isUpNow;
		if (justUp && exclusive)
			Reset(key);

		return justUp;
	}

	void Reset(DWORD key)
	{
		if (IsValidKey(key))
			m_keyStates[key] = KeyState{};
	}

private:
	KeyboardManager() = default;
	static constexpr bool IsValidKey(DWORD key) { return key >= 1 && key < KeyCount; }

	KeyState m_keyStates[KeyCount]{};
};

void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow)
{
	KeyboardManager::Instance().OnMessage(key, repeats, scanCode, isExtended, isWithAlt, wasDownBefore, isUpNow);
}

bool IsKeyDown(DWORD key)
{
	return KeyboardManager::Instance().IsDown(key);
}

bool IsKeyJustUp(DWORD key, bool exclusive)
{
	return KeyboardManager::Instance().IsJustUp(key, exclusive);
}

void ResetKeyState(DWORD key)
{
	KeyboardManager::Instance().Reset(key);
}