#include "FGraphics.h"

#include <d2d1.h>
#include <dwrite.h>

//Graphics::Graphics(HWND Window)
//{
//	Direct2DFactory = 0;
//
//	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Direct2DFactory);
//
//	Direct2DFactory->CreateHwndRenderTarget(
//		D2D1::RenderTargetProperties(),
//		D2D1::HwndRenderTargetProperties(Window, D2D1::SizeU(GlobalRectangle.right, GlobalRectangle.bottom)), &RenderTarget);
//
//	//RenderTarget->CreateSolidColorBrush(
//	//	D2D1::ColorF(D2D1::ColorF::Blue),
//	//	&pBlackBrush);
//
//	//DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
//	//	__uuidof(WriteFactory),
//	//	reinterpret_cast<IUnknown **>(&WriteFactory)
//	//);
//
//	//WriteFactory->CreateTextFormat(
//	//	L"Verdana",
//	//	NULL,
//	//	DWRITE_FONT_WEIGHT_NORMAL,
//	//	DWRITE_FONT_STYLE_NORMAL,
//	//	DWRITE_FONT_STRETCH_NORMAL,
//	//	50,
//	//	L"", //locale
//	//	&TextFormat
//	//);
//
//	//RenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &pBlackBrush);
//
//	//int RectangleLeft = 100;
//	//int RectangleRight = 100 + 200;
//	//int RectangleTop = 100 + 200;
//	//int RectangleBottom = 100;
//
//	//RenderTarget->FillRectangle(
//	//	D2D1::RectF(RectangleLeft, RectangleTop, RectangleRight, RectangleBottom), pBlackBrush);
//}

FGraphics::FGraphics()
{
	Direct2DFactory = NULL;
	RenderTarget = NULL;
}

FGraphics::~FGraphics()
{
	if (Direct2DFactory)
	{
		Direct2DFactory->Release();
	}

	if (RenderTarget)
	{
		RenderTarget->Release();
	}
}

bool FGraphics::Init(HWND WindowHandle)
{
	HRESULT HResult = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Direct2DFactory);
	if (HResult != S_OK)
	{
		return false;
	}

	RECT WindowRect;
	GetClientRect(WindowHandle, &WindowRect);

	HResult = Direct2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			WindowHandle, D2D1::SizeU(WindowRect.right, WindowRect.bottom)),
		&RenderTarget);
	if (HResult != S_OK)
	{
		return false;
	}

	return true;
}

void FGraphics::ClearScreen(float R, float G, float B)
{
	RenderTarget->Clear(D2D1::ColorF(R, G, B));
}

void FGraphics::DrawCircle(float x, float y, float radius, float r, float g, float b, float a)
{
	ID2D1SolidColorBrush *Brush;
	RenderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &Brush);

	RenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), Brush);

	Brush->Release();
}

void FGraphics::DrawRectangle(int Left, int Right, int Top, int Bottom)
{
	ID2D1SolidColorBrush *Brush;
	RenderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0), &Brush);
		
	RenderTarget->FillRectangle(D2D1::Rect(Left, Top, Right, Bottom), Brush);

	Brush->Release();
}

void FGraphics::RenderAndUpdateClient()
{
	//if (RenderTarget)
	//{
	//	RenderTarget->BeginDraw();
	//	RenderTarget->Clear(0);

	//	static const WCHAR sc_helloWorld[] = L"Hello, World!";

	//	//if (MouseEnter(Window, &SexyButton))
	//	//{
	//	//	DrawMenuRectangle(SexyButton.Width, SexyButton.Height, SexyButton.X, SexyButton.Y, D2D1::ColorF::Red);
	//	//}
	//	//else
	//	//{
	//	//	DrawMenuRectangle(SexyButton.Width, SexyButton.Height, SexyButton.X, SexyButton.Y, D2D1::ColorF::Yellow);
	//	//}

	//	RenderTarget->DrawText(
	//		sc_helloWorld,
	//		ARRAYSIZE(sc_helloWorld) - 1,
	//		TextFormat,
	//		D2D1::RectF(0, 0, 200, 200),
	//		pBlackBrush
	//	);

	//	RenderTarget->EndDraw();
	//}
}