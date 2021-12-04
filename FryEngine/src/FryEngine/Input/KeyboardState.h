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
	void SetKeyState(int virtualKeyCode, bool isDown);
	KeyState GetKeyState(int virtualKeyCode) const;
private:
	bool m_pKeyStates[256] = { 0 };
};