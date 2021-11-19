#pragma once

enum KeyState
{
	Up = false,
	Down = true
};

class KeyboardState
{
public:
	KeyboardState();
	~KeyboardState();
	void UpdateKeyboard(long long dt);
	void UpdateKey(int virtualKeyCode, long long dt);
	KeyState GetKeyState(int virtualKeyCode) const;
	double GetTimeHeldDown(int virtualKeyCode);
	bool HasKeyRecentlyMovedUp(int virtualKeyCode);
private:
	bool m_pKeyStates[256] = { 0 };
	double m_pKeyDownTime[256] = { 0.0 };
};