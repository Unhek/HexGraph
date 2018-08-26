#include <Windows.h>

#include <stdio.h>

#include "Utility.h"

#include "FGraphics.h"

#include "Main.h"
#include "Button.h"

HINSTANCE__ *GlobalInstance;
static HWND WindowHandle;

FGraphics *Graphics;

bool GlobalRunning;

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

LRESULT CALLBACK WindowProcedure(HWND Window, uint32 Message, WPARAM WParam, LPARAM LParam)
{
	switch (Message)
	{
		case WM_SIZE:
		{
			/*if (WindowGraphics.RenderTarget)
			{
				GetClientRect(Window, &WindowGraphics.GlobalRectangle);
				WindowGraphics.RenderTarget->Resize(D2D1::SizeU(WindowGraphics.GlobalRectangle.right, WindowGraphics.GlobalRectangle.bottom));
				WindowGraphics.RenderAndUpdateClient();
			}*/
		} break;

		case WM_PAINT:
		{
			Graphics->BeginDraw();
			
			Graphics->ClearScreen(0.f, 0.f, 0.5f);
			Graphics->DrawCircle(400, 100, 50, 1.f, 0.f, 0.f, 1.f);
			Graphics->DrawRectangle(20, 220, 20, 220);
			Graphics->EndDraw();
			/*PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			EndPaint(Window, &Paint);*/
		}break;
	
		case WM_CLOSE:
		{
			GlobalRunning = false;
		}
		case WM_DESTROY:
		{
			/*WindowGraphics.RenderTarget = 0;*/
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

	WindowHandle = CreateWindowEx(
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

	Graphics = new FGraphics();
	if (!Graphics->Init(WindowHandle))
	{
		delete Graphics;
		return -1;
	}
	/*GetClientRect(Window, &WindowGraphics.GlobalRectangle);*/
	

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
						ScreenToClient(WindowHandle, &Mouse);

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
		
		//WindowGraphics.RenderAndUpdateClient();
	}

	delete Graphics;

	return Error;
}

