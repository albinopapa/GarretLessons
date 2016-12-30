#include <Windows.h>
#include "Game.h"

Keyboard *kbd;

LRESULT CALLBACK MessageCallback( HWND WinHandle, UINT MessageID, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = 0;

	switch ( MessageID )
	{
		case WM_KEYDOWN:
		{
			kbd->KeyPress( static_cast< int >( wParam ) );
			break;
		}
		case WM_KEYUP:
		{
			kbd->KeyRelease( static_cast< int >( wParam ) );
			break;
		}
		case WM_DESTROY: 
			PostQuitMessage( 0 );
			break;
	}

	lResult = DefWindowProc( WinHandle, MessageID, wParam, lParam );
	return lResult;
}

class WndClass
{
public:
	WndClass( LPCWSTR ClassName, HINSTANCE Instance, WNDPROC CallbackFunc )
		:
		m_classname( ClassName ),
		m_instance( Instance )
	{
		WNDCLASSEX wc{};
		wc.cbSize = sizeof( wc );
		wc.hInstance = m_instance;
		wc.lpfnWndProc = CallbackFunc;
		wc.lpszClassName = m_classname;

		RegisterClassEx( &wc );
	}
	~WndClass()
	{
		UnregisterClass( m_classname, m_instance );
	}

private:
	LPCWSTR m_classname;
	HINSTANCE m_instance;
};
class ComManager
{
public:
	ComManager();
	~ComManager();
};

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
	ComManager comManager;

	auto instance = GetModuleHandle( nullptr );
	const auto classname = L"MyClass";
	WndClass wc( classname, instance, MessageCallback );

	HWND winHandle = [&classname, instance]{
		const auto winTitle = L"Garret's programming lessons.";

		const auto windowStyle = WS_OVERLAPPEDWINDOW;
		const auto exWindowStyle = WS_EX_OVERLAPPEDWINDOW;

		const int x = 0;
		const int y = 0;

		const RECT wr = [ &windowStyle, &exWindowStyle ]( int X, int Y, int Width, int Height )
		{
			RECT wr{ X, Y, X + Width, Y + Height };
			AdjustWindowRectEx( &wr, windowStyle, FALSE, exWindowStyle );

			return wr;
		}( x, y, 800, 600 );
		const int windowWidth = []( const RECT &R ) { return R.right - R.left; }( wr );
		const int windowHeight = []( const RECT &R ) { return R.bottom - R.top; }( wr );

		return CreateWindowEx(
			exWindowStyle, classname, winTitle, windowStyle,
			x, y, windowWidth, windowHeight, nullptr, nullptr, instance,
			nullptr
		);
	}( );

	if ( IsWindow( winHandle ) == FALSE ) return -1;
	ShowWindow( winHandle, SW_SHOWDEFAULT );
	
	[winHandle]()
	{
		MSG msg{};
		try
		{
			Game tree( winHandle );
			kbd = tree.GetKeyboard();

			while ( true )
			{
				while ( PeekMessage( &msg, nullptr, 0u, 0u, PM_REMOVE ) )
				{
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}

				if ( msg.message == WM_QUIT ) return;
				try
				{
					tree.Go();
				}
				catch ( Direct2D::Exception e )
				{
					const auto errMsg = e.GetFullMessage();
					MessageBox( nullptr, errMsg.c_str(), L"Error", MB_ABORTRETRYIGNORE );
					return;
				}

			}
		}
		catch ( Direct2D::Exception e )
		{
			const auto errMsg = e.GetFullMessage();
			MessageBox( nullptr, errMsg.c_str(), L"Error", MB_ABORTRETRYIGNORE );
			return;
		}
		
	}();

	return 0;
}

ComManager::ComManager()
{
	CoInitialize( nullptr );
}

ComManager::~ComManager()
{
	CoUninitialize();
}
