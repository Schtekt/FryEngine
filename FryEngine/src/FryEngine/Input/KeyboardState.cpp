#include "KeyboardState.h"
#include <Windows.h>
KeyboardState::KeyboardState()
{
}

KeyboardState::~KeyboardState()
{

}

void KeyboardState::UpdateKeyboard(long long dt)
{
	for (unsigned int i = 0; i < 256; i++)
	{
		m_pKeyDownTime[i] *= m_pKeyStates[i];
		m_pKeyStates[i] = GetAsyncKeyState(i);
		m_pKeyDownTime[i] += m_pKeyStates[i] * dt;
	}
}

void KeyboardState::UpdateKey(int virtualKeyCode, long long dt)
{
	m_pKeyDownTime[virtualKeyCode] *= m_pKeyStates[virtualKeyCode];
	m_pKeyStates[virtualKeyCode] = GetAsyncKeyState(virtualKeyCode);
	m_pKeyDownTime[virtualKeyCode] += m_pKeyStates[virtualKeyCode] * dt;
}

KeyState KeyboardState::GetKeyState(int virtualKeyCode) const
{
	return (KeyState)m_pKeyStates[virtualKeyCode];
}

double KeyboardState::GetTimeHeldDown(int virtualKeyCode)
{
	return m_pKeyDownTime[virtualKeyCode];
}

bool KeyboardState::HasKeyRecentlyMovedUp(int virtualKeyCode)
{
	return !m_pKeyStates[virtualKeyCode] && (m_pKeyDownTime[virtualKeyCode] > 0.00000001);
}
