#pragma once
#include "KeyboardState.h"

class MouseState
{
public:
	KeyState IsLeftDown();
	KeyState IsRightDown();
	int  GetPosX();
	int  GetPosY();

	void SetLeftButton(bool isDown);
	void SetRightButton(bool isDown);

	void SetMousePos(int x, int y);
private:
	bool m_leftBtnDown = false;
	bool m_rightBtnDown = false;

	int m_posX = 0;
	int m_posY = 0;
};