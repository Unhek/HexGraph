#pragma once

#include <d2d1.h>
#include <dwrite.h>

class FGraphics
{

	ID2D1Factory *Direct2DFactory;
	ID2D1HwndRenderTarget *RenderTarget;
	IDWriteFactory *WriteFactory;
	IDWriteTextFormat *TextFormat;
	ID2D1SolidColorBrush *Brush;
	ID2D1SolidColorBrush *pBlackBrush;

	RECT GlobalRectangle;

public:
	FGraphics();
	~FGraphics();

	bool Init(HWND Window);

	void BeginDraw() { RenderTarget->BeginDraw(); }
	void EndDraw() { RenderTarget->EndDraw(); }

	void ClearScreen(float R, float G, float B);
	void DrawCircle(float x, float y, float radius, float r, float g, float b, float a);
	void DrawRectangle(int Left, int Right, int Top, int Bottom);


	void RenderAndUpdateClient();
};

