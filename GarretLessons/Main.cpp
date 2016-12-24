#include <Windows.h>

LRESULT CALLBACK MessageCallback( HWND WinHandle, UINT MessageID, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = 0;

	switch ( MessageID )
	{
		case WM_DESTROY:
			PostQuitMessage( 0 );
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
int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
	auto instance = GetModuleHandle( nullptr );
	const auto classname = L"MyClass";
	const auto winTitle = L"G is a goof-ball.";

	WndClass wc( classname, instance, MessageCallback );

	const auto windowStyle = WS_OVERLAPPEDWINDOW;
	const auto exWindowStyle = WS_EX_OVERLAPPEDWINDOW;

	const int x = 0;
	const int y = 0;

	const RECT wr = []( int X, int Y, int Width, int Height )
	{
		RECT wr{X, Y, X + Width, Y + Height};
		AdjustWindowRectEx( &wr, windowStyle, FALSE, exWindowStyle );

		return wr;
	}( x, y, 800, 600 );
	const int windowWidth = []( const RECT &R ) { return R.right - R.left; }( wr );
	const int windowHeight = []( const RECT &R ) { return R.bottom - R.top; }( wr );

	HWND winHandle = CreateWindowEx(
		exWindowStyle, classname, winTitle, windowStyle,
		x, y, windowWidth, windowHeight, nullptr, nullptr, instance,
		nullptr
	);

	if ( IsWindow( winHandle ) == FALSE ) return -1;

	ShowWindow( winHandle, SW_SHOWDEFAULT );

	MSG msg{};
	while ( true )
	{
		while ( PeekMessage( &msg, nullptr, 0u, 0u, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		if ( msg.message == WM_QUIT )
		{
			break;
		}
	}

	return 0;
}
