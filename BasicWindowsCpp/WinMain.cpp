///////////////////////////////////////////////////////////////////////////////
// Minimal code to have a window up and a simple bitmap backbuffer to draw to
//
// To Do
// - add/test the alpha channel
// - add error checking
// - add window size change... maybe... or have graphics engine render to one size, and blit to window size... decisions!!
// - assuming all windows are the same size so a single buffer w,h variable at the moment.
// - more threading?... each window/render should be on a separate thread. Raytracing in it's own thread
// - create an interupt for the ray tracing thread to restart rendering when something changes?
//
// Maybe To Do
// - fix frame rate?
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <time.h>
#include "BERenderPipeline.h"

// global windows variables and macros
#define BENUMBER_WINDOWS 4
#define BE_SWBUFFERSIZE 1000
HWND hwnd[BENUMBER_WINDOWS];
HDC hdc[BENUMBER_WINDOWS];
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
BECanvas backBuffer[BENUMBER_WINDOWS];
BITMAPINFO bmpInfo[BENUMBER_WINDOWS] = { 0 };

// global engine variables
BECamera camera({0,0,3}, {0,0,-1});
BEWorld world;
BERenderPipeline* pipeline[BENUMBER_WINDOWS];

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

void BEDrawBackBuffer(int bufferindx, int windowIndx)
{
	backBuffer[bufferindx].BufferToBMP();

	StretchDIBits(
		hdc[windowIndx],
		0,
		0,
		bufferWidth,
		bufferHeight,
		0,
		0,
		bufferWidth,
		bufferHeight,
		backBuffer[bufferindx].bmp,
		&(bmpInfo[bufferindx]),
		DIB_RGB_COLORS,
		SRCCOPY
	);
}

void BEDrawBackBuffer(int indx)
{
	BEDrawBackBuffer(indx, indx);
}

//HFONT hFont = (HFONT)GetStockObject(ANSI_FIXED_FONT);
void BEWriteOverlayToWindow(int indx, LPCWSTR lpwString)
{
	RECT rect;
	GetClientRect(hwnd[indx], &rect);

	//SetBkColor(hdc[indx], RGB(0, 0, 0));
	//SelectObject(hdc[indx], hFont);
	SetTextColor(hdc[indx], RGB(255, 255, 255));
	SetBkMode(hdc[indx], TRANSPARENT);
	//	DrawText(hdc[indx], lpwString, wcslen(lpwString), &rect, DT_LEFT);
	DrawText(hdc[indx], lpwString, -1, &rect, DT_LEFT);
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
			camera.Pan(0, 0, 0.1f);
			break;
		case VK_SUBTRACT:
			camera.Pan(0, 0, -0.1f);
			break;
		case 0x44: // D
			camera.RotateDirection(0.1f, 0, 0);
			break;
		case 0x41: // A
			camera.RotateDirection(-0.1f, 0, 0);
			break;
		case 0x53: // S
			camera.RotateDirection(0, -0.1f, 0);
			break;
		case 0x57: // W
			camera.RotateDirection(0, 0.1f, 0);
			break;
		case 0x52: // R
			pipeline[1]->restartLoop = true;
			break;
		case 0x54: // T
			BEDrawBackBuffer(1);
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

	RECT rect;
	GetWindowRect(GetDesktopWindow(), &rect);
	if (windowPosX + windowSizeW > rect.right)
	{
		// next window would be off screen so shuffle down.
		// To Do: tidy up this hack?
		windowPosX = 0;
		windowPosY += windowSizeH;
	}


	hdc[indx] = GetDC(hwnd[indx]);

	GetClientRect(hwnd[indx], &rect);
	bufferWidth = rect.right;
	bufferHeight = rect.bottom;

	BECreateBackBuffer(indx);
	backBuffer[indx].Clear();

	return 0;
}

void BECleanupWindow(int indx)
{
	ReleaseDC(hwnd[indx], hdc[indx]);
}

DWORD WINAPI BEThreadFunctionRayTrace(LPVOID lpParam)
{
	WCHAR swbuffer[BE_SWBUFFERSIZE];

	int indx = *((int*)lpParam);
	int resultIndx = 3; // to do: hard coded the window indx

	BEWriteOverlayToWindow(resultIndx, L"Starting...");
	BEWriteOverlayToWindow(resultIndx, L""); // To Do: something strange when this is in the thread... need a second 1 for it to show?!

	clock_t lastTime = clock();

	while (running)
	{
		pipeline[indx]->restartLoop = false;
		backBuffer[indx].Clear();
		pipeline[indx]->Draw();
		
		clock_t currentTime = clock();
		float deltaTime = (float)(currentTime - lastTime) / CLOCKS_PER_SEC; // seconds
		lastTime = currentTime;

		// add an extra window, and show the completed image
		BEDrawBackBuffer(indx, resultIndx);

		swprintf(swbuffer, BE_SWBUFFERSIZE, L"Render time: %7.2fs", deltaTime);
		BEWriteOverlayToWindow(resultIndx, swbuffer);
		BEWriteOverlayToWindow(resultIndx, L""); // To Do: something strange when this is in the thread... need a second 1 for it to show?!
	}

	return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg = { 0 };
	WCHAR swbuffer[BE_SWBUFFERSIZE];

	BECreateWindowClass(hInstance);
	BECreateWindow(0, hInstance);
	BECreateWindow(1, hInstance);
	BECreateWindow(2, hInstance);
	BECreateWindow(3, hInstance);

	world.Create();

	// for scanline rendering
	pipeline[0] = new BERenderPipelineScanline(&world, &camera, &backBuffer[0]);
	backBuffer[0].AddDepthBuffer(); // to do: automatically add a depth buffer?

	// for raytrace rendering
	pipeline[1] = new BERenderPipelineRaytrace(&world, &camera, &backBuffer[1]);
	BERenderPipelineRaytrace* pRT = (BERenderPipelineRaytrace*)pipeline[1];

	// for wireframe rendering
	pipeline[2] = new BERenderPipelineWireframe(&world, &camera, &backBuffer[2]);


	// ready to go...

	// put ray tracer in a separate thread
	DWORD threadId;
	int arg = 1;
	HANDLE thread = CreateThread(NULL, 0, BEThreadFunctionRayTrace, &arg, 0, &threadId);
	if (thread == NULL) return -1;

	clock_t lastTime = clock();
	clock_t startRend, endRend;

	while (running)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}


		//
		// scanline..........
		//

		backBuffer[0].Clear();
		startRend = clock();
		pipeline[0]->Draw();
		endRend = clock();
		BEDrawBackBuffer(0);

		swprintf(swbuffer, BE_SWBUFFERSIZE, L"Rendering time: %ims", (endRend - startRend));
		BEWriteOverlayToWindow(0, swbuffer);

		//
		// raytrace.......... running in a thread so showing only
		//

		//if (pipeline[1]->showBuffer)
		{
			BEDrawBackBuffer(1);
			pipeline[1]->showBuffer = false;

			int todo = pRT->raysToProcess;
			int done = pRT->raysProcessed;
			float pcnt = (float)done / (float)todo * 100.0f;
			swprintf(swbuffer, BE_SWBUFFERSIZE, L"Rays to process %i\nRays processed %i(%3.2f%%)", todo, done, pcnt);
			BEWriteOverlayToWindow(1, swbuffer);
		}

		//
		// wireframe.............
		//

		backBuffer[2].Clear();
		startRend = clock();
		pipeline[2]->Draw();
		endRend = clock();
		BEDrawBackBuffer(2);

		// to use for the overall loop...
		// to do: limit framerate? Worry about that when it actually works quick
		clock_t currentTime = clock();
		deltaTime = (float)(currentTime - lastTime) / (float)CLOCKS_PER_SEC; // time in seconds
		lastTime = currentTime;

		swprintf(swbuffer, BE_SWBUFFERSIZE, L"Rendering time: %ims\nLoop time: %3.3fs\nfps: %f", (endRend - startRend), deltaTime, 1.0f / deltaTime);
		BEWriteOverlayToWindow(2, swbuffer);

	}

	pipeline[1]->exitLoop = true; // tell it to stop!
	WaitForSingleObject(thread, 3000);
	CloseHandle(thread);

	BECleanupWindow(0);
	BECleanupWindow(1);

	return 0;
}

