#pragma once

class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	bool IsKeyPressed( int Code );
	void KeyPress( int Code );
	void KeyRelease( int Code );
private:
	bool keys[ 256 ];

};

