///////////////////////////////////////////////////////////////////////////////
// Minimal code to have a window up and a simple bitmap backbuffer to draw to
//
// To Do
// - add/test the alpha channel
// - add error checking
// - add window size change... maybe... or have graphics engine render to one size, and blit to window size... decisions!!
//
// Maybe To Do
// - fix frame rate?
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <time.h>
#include "BERenderPipeline.h"

// global windows variables
HWND hwnd;
HDC hdc;

// global control variables
bool running = true;
float deltaTime = 0.0f;
bool clearBackBuffer = true;

// global back buffer variables
int width = 0;
int height = 0;
BECanvas backBuffer;
BITMAPINFO bmpInfo = { 0 };

// global engine variables
BECamera camera;
BEWorld world;

/////////////////////////////////
// back buffer functions

void CreateBackBuffer()
{
	if (width == 0 || height == 0) return;

	backBuffer.Initialise(width, height);

	bmpInfo.bmiHeader.biSize = sizeof(bmpInfo.bmiHeader);
	bmpInfo.bmiHeader.biWidth = width;
	bmpInfo.bmiHeader.biHeight = height; // use - for top-down DIB and its origin is the upper-left corner
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
}

void DrawBackBuffer()
{
	backBuffer.BufferToBMP();

	StretchDIBits(
		hdc,
		0,
		0,
		width,
		height,
		0,
		0,
		width,
		height,
		backBuffer.bmp,
		&(bmpInfo),
		DIB_RGB_COLORS,
		SRCCOPY
	);
}

/////////////////////////////////
// windows code

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	//case WM_PAINT:
	//{
	//	DrawBackBuffer();
	//	break;
	//}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			running = false;
			break;
		case VK_SPACE:
			break;
		case VK_UP:
			camera.Translate({ 0,0.1f,0 });
			break;
		case VK_DOWN:
			camera.Translate({ 0,-0.1f,0 });
			break;
		case VK_RIGHT:
			camera.Translate({ 0.1f,0,0 });
			break;
		case VK_LEFT:
			camera.Translate({ -0.1f,0,0 });
			break;
		case VK_ADD:
			camera.Translate({ 0,0,0.1f });
			break;
		case VK_SUBTRACT:
			camera.Translate({ 0,0,-0.1f });
			break;
		}
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		running = false;
		break;
	}	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	MSG msg = { 0 };
	WNDCLASS wc = { 0 };
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = L"minWinApp";

	if (!RegisterClass(&wc))
		return 1;

	hwnd = CreateWindow(wc.lpszClassName,
		L"Minimal Windows Application",
		WS_VISIBLE, // WS_OVERLAPPEDWINDOW |
		0, 0,
		800, 600, ///////////////// window size!!!!
		0, 0, hInstance, NULL);

	if (!hwnd)
		return 2;

	hdc = GetDC(hwnd);

	RECT dcRect;
	GetClientRect(hwnd, &dcRect);
	width = dcRect.right;
	height = dcRect.bottom;

	CreateBackBuffer();
	backBuffer.Clear();

	world.Create();

	BERenderPipeline pipeline(&world, &camera, &backBuffer);

	clock_t lastTime = clock();

	while (running)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		clock_t currentTime = clock();
		deltaTime = (float)(currentTime - lastTime);
		lastTime = currentTime;

		if (clearBackBuffer) backBuffer.Clear();

		//pipeline.UpdateScreenSpace();
		pipeline.Draw();

		//pipeline.DrawTriangle({ 100,100,0 }, { 100,400,0 }, { 250,400,0 }, { 1,1,1,1 });
		//pipeline.DrawTriangle({ 300,400,0 }, { 300,100,0 }, { 550,100,0 }, { 0,1,1,1 });
		//pipeline.DrawTriangle({ 600,300,0 }, { 750,350,0 }, { 650,400,0 }, { 1,0,1,1 });

		//backBuffer.DrawTestPattern();
		//backBuffer.TestStuff();

		DrawBackBuffer();
	}

	ReleaseDC(hwnd, hdc);

	return 0;
}

