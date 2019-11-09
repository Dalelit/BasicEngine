///////////////////////////////////////////////////////////////////////////////
// Minimal code to have a window up and a simple bitmap backbuffer to draw to
//
// To Do
// - add/test the alpha channel
// - add error checking
// - add window size change... maybe... or have graphics engine render to one size, and blit to window size... decisions!!
// - assuming all windows are the same size so a single buffer w,h variable at the moment.
//
// Maybe To Do
// - fix frame rate?
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <time.h>
#include "BERenderPipeline.h"

// global windows variables
HWND hwnd[2];
HDC hdc[2];
int windowPosX = 0;
int windowPosY = 0;
int windowSizeW = 800;
int windowSizeH = 600;


// global control variables
bool running = true;
float deltaTime = 0.0f;

// global back buffer variables
int bufferWidth = 0;
int bufferHeight = 0;
BECanvas backBuffer[2];
BITMAPINFO bmpInfo[2] = { 0 };

// global engine variables
BECamera camera;
BEWorld world;
BERenderPipeline* pipeline[2];

/////////////////////////////////
// back buffer functions

void BECreateBackBuffer(int indx)
{
	if (bufferWidth == 0 || bufferHeight == 0) return;

	backBuffer[indx].Initialise(bufferWidth, bufferHeight);

	bmpInfo[indx].bmiHeader.biSize = sizeof(bmpInfo[indx].bmiHeader);
	bmpInfo[indx].bmiHeader.biWidth = bufferWidth;
	bmpInfo[indx].bmiHeader.biHeight = bufferHeight; // use - for top-down DIB and its origin is the upper-left corner
	bmpInfo[indx].bmiHeader.biPlanes = 1;
	bmpInfo[indx].bmiHeader.biBitCount = 32;
	bmpInfo[indx].bmiHeader.biCompression = BI_RGB;
}

void BEDrawBackBuffer(int indx)
{
	backBuffer[indx].BufferToBMP();

	StretchDIBits(
		hdc[indx],
		0,
		0,
		bufferWidth,
		bufferHeight,
		0,
		0,
		bufferWidth,
		bufferHeight,
		backBuffer[indx].bmp,
		&(bmpInfo[indx]),
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
			camera.Pan(0, 0.1f, 0);
			break;
		case VK_DOWN:
			camera.Pan(0, -0.1f, 0);
			break;
		case VK_RIGHT:
			camera.Pan(0.1f, 0, 0);
			break;
		case VK_LEFT:
			camera.Pan(-0.1f, 0, 0);
			break;
		case VK_ADD:
			camera.Translate({ 0,0,0.1f });
			break;
		case VK_SUBTRACT:
			camera.Translate({ 0,0,-0.1f });
			break;
		case 0x44: // D
			camera.RotateDirection(0.1f, 0, 0);
			break;
		case 0x41: // A
			//camera.Pan(-1, 0, 0);
			camera.RotateDirection(-0.1f, 0, 0);
			break;
		case 0x57: // W
			//camera.Pan(0, 1, 0);
			camera.RotateDirection(0, 0.1f, 0);
			break;
		case 0x53: // S
			//camera.Pan(0, -1, 0);
			camera.RotateDirection(0, -0.1f, 0);
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

WNDCLASS wc = { 0 };

int BECreateWindowClass(HINSTANCE hInstance)
{
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = L"BasicEngingWindow";

	if (!RegisterClass(&wc))
		return 1;

	return 0;
}

int BECreateWindow(int indx, HINSTANCE hInstance)
{
	hwnd[indx] = CreateWindow(wc.lpszClassName,
		L"Minimal Windows Application",
		WS_VISIBLE, // WS_OVERLAPPEDWINDOW |
		windowPosX, windowPosY,
		windowSizeW, windowSizeH,
		(indx == 0 ? 0 : hwnd[0]), // parent window
		0, hInstance, NULL);

	windowPosX += windowSizeW;

	if (!hwnd[indx])
		return 2;

	hdc[indx] = GetDC(hwnd[indx]);

	RECT dcRect;
	GetClientRect(hwnd[indx], &dcRect);
	bufferWidth = dcRect.right;
	bufferHeight = dcRect.bottom;

	BECreateBackBuffer(indx);
	backBuffer[indx].Clear();

	return 0;
}

void BECleanupWindow(int indx)
{
	ReleaseDC(hwnd[indx], hdc[indx]);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg = { 0 };

	BECreateWindowClass(hInstance);
	BECreateWindow(0, hInstance);
	BECreateWindow(1, hInstance);

	world.Create();

	pipeline[0] = new BERenderPipeline(&world, &camera, &backBuffer[0]);
	pipeline[1] = new BERenderPipeline(&world, &camera, &backBuffer[1]);

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

		backBuffer[0].Clear();
		backBuffer[1].Clear();

		pipeline[0]->Draw();
		pipeline[1]->Raytrace();

		BEDrawBackBuffer(0);
		BEDrawBackBuffer(1);
	}

	BECleanupWindow(0);
	BECleanupWindow(1);

	return 0;
}

