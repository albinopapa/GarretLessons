#pragma once

#include "Direct2D.h"

class Circle
{
public:
	Circle();
	~Circle();

	float GetX()const;
	float GetY()const;
	float GetRadius()const;

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

	void Draw( Direct2D &D2D );
	
private:
	float x, y, radius, speed;
	D2D1::ColorF color;
};

