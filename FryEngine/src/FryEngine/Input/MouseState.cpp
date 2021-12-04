#include "MouseState.h"

KeyState MouseState::IsLeftDown()
{
	return (KeyState)m_leftBtnDown;
}

KeyState MouseState::IsRightDown()
{
	return (KeyState)m_rightBtnDown;
}

int MouseState::GetPosX()
{
	return m_posX;
}

int MouseState::GetPosY()
{
	return m_posY;
}

void MouseState::SetLeftButton(bool isDown)
{
	m_leftBtnDown = isDown;
}

void MouseState::SetRightButton(bool isDown)
{
	m_rightBtnDown = isDown;
}

void MouseState::SetMousePos(int x, int y)
{
	m_posX = x;
	m_posY = y;
}
