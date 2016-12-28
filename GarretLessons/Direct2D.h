#pragma once

#include <d2d1_1.h>
#include <wrl/client.h>
#include <string>

#include "Exception.h"

#pragma comment(lib, "d2d1.lib")



class Direct2D
{
public:
	class Exception :public ::Exception
	{
	public:
		Exception( HRESULT hr, std::wstring&& note, std::wstring&& file = _CRT_WIDE( __FILE__ ), unsigned int line = __LINE__ );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};

	Direct2D(HWND WinHandle);
	~Direct2D();

	void BeginFrame( float R, float G, float B );
	void EndFrame();

	void DrawBox( float X, float Y, float Width, float Height, const D2D1_COLOR_F &Color, BOOL Outline = FALSE );
	void DrawCircle( float X, float Y, float Radius, const D2D1_COLOR_F &Color, BOOL Outline = false );


private:
	Microsoft::WRL::ComPtr<ID2D1Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_pContext;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_pRenderTarget;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pSolidBrush;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapchain;
	HWND m_winHandle;
};

