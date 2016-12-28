#include "Direct2D.h"
#include "DXErr.h"
#pragma comment(lib, "d3d11.lib")

using Microsoft::WRL::ComPtr;
Direct2D::Direct2D( HWND WinHandle )
	:
	m_winHandle( WinHandle )
{
	const auto rc = [ this ] 
	{
		RECT rc{};
		GetClientRect( m_winHandle, &rc );
		return rc;
	}( );
	const auto winSize = D2D1_SIZE_U{ 
		static_cast<uint32_t>( rc.right - rc.left ),
		static_cast<uint32_t>( rc.bottom - rc.top ) };

	DXGI_SWAP_CHAIN_DESC scd{};
	scd.BufferDesc = {
		winSize.width,
		winSize.height,
		{ 60, 1 },
		DXGI_FORMAT_B8G8R8A8_UNORM,
		DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
		DXGI_MODE_SCALING_UNSPECIFIED
	};
	scd.SampleDesc = { 1, 0 };
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = m_winHandle;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scd,
		&m_pSwapchain,
		&m_pDevice3D,
		nullptr,
		&m_pContext3D
	);

	ComPtr<IDXGIDevice> pDeviceDxgi;
	if ( SUCCEEDED( hr ) )
	{
		hr = m_pDevice3D.As( &pDeviceDxgi );
	}
	if ( SUCCEEDED( hr ) )
	{
		hr = D2D1CreateDevice( pDeviceDxgi.Get(), nullptr, &m_pDevice );
	}
	if ( SUCCEEDED( hr ) )
	{
		hr = m_pDevice->CreateDeviceContext( D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pContext );
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	D3D11_TEXTURE2D_DESC td{};
	if ( SUCCEEDED( hr ) )
	{
		m_pSwapchain->GetBuffer( 0, IID_PPV_ARGS( &pTexture ) );
	}
	auto pSurface = Microsoft::WRL::ComPtr<IDXGISurface>();
	if ( SUCCEEDED( hr ) )
	{
		hr = pTexture.As( &pSurface );
	}
	if ( SUCCEEDED( hr ) )
	{
		hr = m_pContext->CreateBitmapFromDxgiSurface( pSurface.Get(), nullptr, &m_pRenderTarget );
	}
	if ( SUCCEEDED( hr ) )
	{
		m_pContext->SetTarget( m_pRenderTarget.Get() );
		const auto color = D2D1::ColorF( D2D1::ColorF::White );
		hr = m_pContext->CreateSolidColorBrush( color, &m_pSolidBrush );
	}
	if ( FAILED( hr ) )
	{
		throw( hr );
	}
}


Direct2D::~Direct2D()
{}

void Direct2D::BeginFrame( float R, float G, float B )
{
	m_pContext->BeginDraw();
	m_pContext->Clear( { R, G, B, 1.f } );
}

void Direct2D::EndFrame()
{
	auto hr = m_pContext->EndDraw();
	if ( FAILED( hr ) )
		throw( Direct2D::Exception( hr, L"Failed at EndFrame()" ));
	m_pSwapchain->Present( 1, 0 );
	/*if ( hr == D2DERR_RECREATE_TARGET )
	{
		m_pRenderTarget.Reset();
		RECT wr{};
		GetWindowRect( m_winHandle, &wr );
		const D2D1_SIZE_U winSize{ wr.right - wr.left, wr.bottom - wr.top };

		D2D1_BITMAP_PROPERTIES props{};
		props.dpiX = 96.f;
		props.dpiY = 96.f;
		props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
		props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
		hr = m_pContext->CreateBitmap( winSize, props, &m_pRenderTarget );

		if ( FAILED( hr ) ) throw( hr );
		m_pContext->SetTarget( m_pRenderTarget.Get() );
	}*/
}

void Direct2D::DrawBox( float X, float Y, float Width, float Height, const D2D1_COLOR_F & Color, BOOL Outline )
{
	m_pSolidBrush->SetColor( Color );
	const auto rect = D2D1::RectF( X, Y, X + Width, Y + Height );

	auto drawOutline = [ ](ID2D1DeviceContext *pContext, const D2D1_RECT_F &Rect, ID2D1SolidColorBrush *pBrush)
	{
		pContext->DrawRectangle( Rect, pBrush );
	};
	auto drawNoOutline = [ ]( ID2D1DeviceContext *pContext, const D2D1_RECT_F &Rect, ID2D1SolidColorBrush *pBrush )
	{
		pContext->FillRectangle( Rect, pBrush );
	};
	
	using DrawFunc = void( *)( ID2D1DeviceContext *pContext, const D2D1_RECT_F &Rect, ID2D1SolidColorBrush *pBrush );

	auto drawfunc = ( Outline == TRUE ) ? ( DrawFunc )drawOutline : ( DrawFunc )drawNoOutline;
	
	drawfunc( m_pContext.Get(), rect, m_pSolidBrush.Get() );
	
}

void Direct2D::DrawCircle( float X, float Y, float Radius, const D2D1_COLOR_F & Color, BOOL Outline )
{
	m_pSolidBrush->SetColor( Color );

	D2D1_ELLIPSE ellipse{};
	ellipse.point = { X, Y };
	ellipse.radiusX = ellipse.radiusY = Radius;

	if ( Outline == TRUE )
	{
		m_pContext->DrawEllipse( ellipse, m_pSolidBrush.Get() );
	}
	else
	{
		m_pContext->FillEllipse( ellipse, m_pSolidBrush.Get() );
	}
}

Direct2D::Exception::Exception( HRESULT hr, std::wstring&& note, std::wstring&& file, unsigned int line )
	:
	::Exception( std::move( file ), line, std::move( note ) ),
	hr( hr )
{}

std::wstring Direct2D::Exception::GetErrorName() const
{
	return DXGetErrorString( hr );
}

std::wstring Direct2D::Exception::GetErrorDescription() const
{
	constexpr auto maxBufferLength = 320u;
	wchar_t buffer[ maxBufferLength ]{};
	DXGetErrorDescription( hr, buffer, maxBufferLength );
	return std::wstring( buffer );
}

std::wstring Direct2D::Exception::GetFullMessage() const
{
	const auto empty = L"";
	const auto errorName = GetErrorName();
	const auto errorDesc = GetErrorDescription();
	const auto& note = GetNote();
	const auto location = GetLocation();
	return
		( !errorName.empty() ? std::wstring( L"Error: " ) + errorName + L"\n"
				: empty )
		+ ( !errorDesc.empty() ? std::wstring( L"Description: " ) + errorDesc + L"\n"
			: empty )
		+ ( !note.empty() ? std::wstring( L"Note: " ) + note + L"\n"
			: empty )
		+ ( !location.empty() ? std::wstring( L"Location: " ) + location
			: empty );
}

std::wstring Direct2D::Exception::GetExceptionType() const
{
	return std::wstring(L"Direct2D Exception");
}
