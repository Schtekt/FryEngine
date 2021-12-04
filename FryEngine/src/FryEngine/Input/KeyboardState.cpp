#include "KeyboardState.h"
#include <Windows.h>
KeyboardState::KeyboardState()
{
}

KeyboardState::~KeyboardState()
{

}

void KeyboardState::SetKeyState(int virtualKeyCode, bool isDown)
{
	m_pKeyStates[virtualKeyCode] = isDown;
}

KeyState KeyboardState::GetKeyState(int virtualKeyCode) const
{
	return (KeyState)m_pKeyStates[virtualKeyCode];
}
