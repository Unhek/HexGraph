#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>

#include <stdio.h>

#include "Utility.h"


#include "Main.h"
#include "Button.h"


HINSTANCE__ *GlobalInstance;
static HWND Window;

// Globals, tmporary
ID2D1Factory *Direct2DFactory = 0;
ID2D1HwndRenderTarget *RenderTarget = 0;
IDWriteFactory *WriteFactory = 0;
IDWriteTextFormat *TextFormat = 0;

RECT GlobalRectangle;

ID2D1SolidColorBrush* pBlackBrush = 0;
bool GlobalRunning;

void DrawMenuRectangle(int Width, int Height, int PosX, int PosY, D2D1::ColorF Color)
{
	ID2D1SolidColorBrush *Brush;
	
	RenderTarget->CreateSolidColorBrush(D2D1::ColorF(Color), &Brush);

	int RectangleLeft = PosX;
	int RectangleRight = PosX + Width;
	int RectangleTop = PosY + Height;
	int RectangleBottom = PosY;

	RenderTarget->FillRectangle(
		D2D1::RectF(RectangleLeft, RectangleTop, RectangleRight, RectangleBottom), Brush);
}

bool MouseEnter(HWND Window, Button *Button)
{
	bool Result = false;

	POINT Mouse;

	GetCursorPos(&Mouse);
	ScreenToClient(Window, &Mouse);

	if (Mouse.x > Button->X && Mouse.x < Button->X + Button->Width &&
		Mouse.y > Button->Y && Mouse.y < Button->Y + Button->Height)
	{
		Result = true;
	}
	else
	{
		Result = false;
	}

	return Result;
}


void RenderAndUpdateClient()
{
	Button SexyButton(100, 50, 30, 30);

	if (RenderTarget)
	{
		RenderTarget->BeginDraw();
		RenderTarget->Clear(0);

		static const WCHAR sc_helloWorld[] = L"Hello, World!";

		if (MouseEnter(Window, &SexyButton))
		{
			DrawMenuRectangle(SexyButton.Width, SexyButton.Height, SexyButton.X, SexyButton.Y, D2D1::ColorF::Red);
		}
		else
		{
			DrawMenuRectangle(SexyButton.Width, SexyButton.Height, SexyButton.X, SexyButton.Y, D2D1::ColorF::Yellow);
		}

		RenderTarget->DrawText(
			sc_helloWorld,
			ARRAYSIZE(sc_helloWorld) - 1,
			TextFormat,
			D2D1::RectF(0, 0, 200, 200),
			pBlackBrush
		);

		RenderTarget->EndDraw();
	}
}


LRESULT CALLBACK WindowProcedure(HWND Window, uint32 Message, WPARAM WParam, LPARAM LParam)
{
	switch (Message)
	{
		case WM_SIZE:
		{
			if (RenderTarget)
			{
				GetClientRect(Window, &GlobalRectangle);
				RenderTarget->Resize(D2D1::SizeU(GlobalRectangle.right, GlobalRectangle.bottom));
				RenderAndUpdateClient();
			}
		} break;

		case WM_PAINT:
		{
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			EndPaint(Window, &Paint);
			
			
		}break;
	
		case WM_CLOSE:
		{
			GlobalRunning = false;
		}
		case WM_DESTROY:
		{
			RenderTarget = 0;
			pBlackBrush = 0;
			PostQuitMessage(0);
		}break;

		default:
		{
			return DefWindowProc(Window, Message, WParam, LParam);
		}break;
	}
}

int32 WINAPI WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, char *CommandLine, int32 CommandShow)
{
	int32 Error = 0;

	WNDCLASS WindowClass;

	// Direct2D Init

	WindowClass.style = 0;
	WindowClass.lpfnWndProc = WindowProcedure;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = Instance;
	WindowClass.hIcon = 0;
	WindowClass.hCursor = 0;
	WindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);;
	WindowClass.lpszMenuName = 0;
	WindowClass.lpszClassName = "GameWindow";
	
	if (!RegisterClass(&WindowClass))
	{
		return 1;
	}

	Window = CreateWindowEx(
		0,
		WindowClass.lpszClassName,
		"GameWindow",
		WS_OVERLAPPEDWINDOW|WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1000,
		1000,
		0,
		0,
		Instance,
		0);



	//HRESULT hr;

	Direct2DFactory = 0;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Direct2DFactory);

	GetClientRect(Window, &GlobalRectangle);

	Direct2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(Window, D2D1::SizeU(GlobalRectangle.right, GlobalRectangle.bottom)), &RenderTarget);

	RenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Blue),
		&pBlackBrush);
		
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(WriteFactory),
		reinterpret_cast<IUnknown **>(&WriteFactory)
	);

	WriteFactory->CreateTextFormat(
		L"Verdana",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		50,
		L"", //locale
		&TextFormat
	);

	GlobalRunning = true;
	while(GlobalRunning)
	{
		MSG Message;
		while((PeekMessage(&Message, 0, 0, 0, PM_REMOVE)))
		{
			switch (Message.message)
			{
				case WM_QUIT:
				{
					GlobalRunning = false;
				} break;

				//case WM_KEYDOWN:
				case WM_KEYUP:
				{

					uint32 VKCode = (uint32)Message.wParam;
					if (VKCode == 'W')
					{

						POINT Mouse;

						GetCursorPos(&Mouse);
						ScreenToClient(Window, &Mouse);

						char DebugMessage[256];
						sprintf_s(DebugMessage, "x:%d, y:%d\n", Mouse.x, Mouse.y);
						OutputDebugStringA(DebugMessage);
					}
				}break;

				default:
				{
					TranslateMessage(&Message);
					DispatchMessageA(&Message);
				} break;
			}
		}
		
		RenderAndUpdateClient();

		
	}

	return Error;
}

