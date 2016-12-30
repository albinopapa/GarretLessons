#include "Circle.h"



Circle::Circle()
	:
	x( 400.f ),
	y( 300.f ),
	radius( 15.f ),
	speed( 2.f ),
	color( 1.f, 0.f, 0.f, 1.f )
{}


Circle::~Circle()
{}

float Circle::GetX() const
{
	return x;
}

float Circle::GetY() const
{
	return y;
}

float Circle::GetRadius() const
{
	return radius;
}

void Circle::MoveLeft()
{
	x -= speed;
}

void Circle::MoveRight()
{
	x += speed;
}

void Circle::MoveUp()
{
	y -= speed;
}

void Circle::MoveDown()
{
	y += speed;
}

void Circle::Draw( Direct2D & D2D )
{
	D2D.DrawCircle( x, y, radius, color );// This line
}
