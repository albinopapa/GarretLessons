#pragma once

#include "Direct2D.h"
#include "Circle.h"
#include "Keyboard.h"

class Game
{
public:
	Game(HWND WinHandle);
	~Game();

	void Go();
	Keyboard *GetKeyboard();

private:
	void UpdateFrame();
	void ComposeFrame();
	void MoveRightAndDown();
	void MoveLeftAndUp();

private:
	Direct2D d2d;
	Keyboard kbd;
	Circle turtle;
};

