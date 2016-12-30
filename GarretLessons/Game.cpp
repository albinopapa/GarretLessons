#include "Game.h"



Game::Game( HWND WinHandle )
	:
	d2d( WinHandle )
{}


Game::~Game()
{}

void Game::Go()
{
	UpdateFrame();
	d2d.BeginFrame( 0.f, 0.f, 0.f );
	ComposeFrame();
	d2d.EndFrame();
}

void Game::UpdateFrame()
{
	if ( kbd.IsKeyPressed( VK_UP ) )
	{
		turtle.MoveUp();
	}
	else if ( kbd.IsKeyPressed( VK_DOWN ) )
	{
		turtle.MoveDown();
	}
	
	if ( kbd.IsKeyPressed( VK_LEFT ) )
	{
		turtle.MoveLeft();
	}
	else if ( kbd.IsKeyPressed( VK_RIGHT ) )
	{
		turtle.MoveRight();
	}
}

void Game::ComposeFrame()
{	
	turtle.Draw( d2d );
}

void Game::MoveRightAndDown()
{	
	if ( turtle.GetX() < 800 - turtle.GetRadius() )
	{
		turtle.MoveRight();
	}
	else
	{		
		if ( turtle.GetY() <= 600 - turtle.GetRadius() )
		{
			turtle.MoveDown();
		}
	}	
}

void Game::MoveLeftAndUp()
{
	if ( turtle.GetX() > 0 + turtle.GetRadius() )
	{
		turtle.MoveLeft();
	}
	else
	{		
		if ( turtle.GetY() >= 0 + turtle.GetRadius() )
		{
			turtle.MoveUp();
		}
	}
}

Keyboard * Game::GetKeyboard()
{
	return &kbd;
}
