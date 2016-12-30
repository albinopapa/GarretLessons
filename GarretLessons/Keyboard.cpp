#include "Keyboard.h"



Keyboard::Keyboard()
{
	for ( auto &key : keys )
	{
		key = false;
	}
}


Keyboard::~Keyboard()
{}

bool Keyboard::IsKeyPressed( int Code )
{
	return keys[Code];
}

void Keyboard::KeyPress( int Code )
{
	keys[ Code ] = true;
}

void Keyboard::KeyRelease( int Code )
{
	keys[ Code ] = false;
}
