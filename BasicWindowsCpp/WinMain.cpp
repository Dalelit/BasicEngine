///////////////////////////////////////////////////////////////////////////////
// Minimal code to have a window up and a simple bitmap backbuffer to draw to
//
// To Do
// - add/test the alpha channel
// - add error checking
// - add window size change... maybe... or have graphics engine render to one size, and blit to window size... decisions!!
// - assuming all windows are the same size so a single buffer w,h variable at the moment.
// - more threading?... each window/render should be on a separate thread. Raytracing in it's own thread
//
// Maybe To Do
// - fix frame rate?
//


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <algorithm>
#include <random>
#include "BERenderPipeline.h"
#include "BEDirectX.h"
#include "BETimer.h"
#include "BEDXShowCanvas.h"

// global windows variables and macros
#define BENUMBER_WINDOWS 6
#define BE_SWBUFFERSIZE 1000
#define BE_WINDOWSTYLE WS_VISIBLE | WS_CAPTION
HWND hwnd[BENUMBER_WINDOWS];
HDC hdc[BENUMBER_WINDOWS];
int windowPosX = 0;
int windowPosY = 0;
int displaySizeW = 800;
int displaySizeH = 600;
RECT windowRect = {0,0, displaySizeW, displaySizeH }; // AdjustWindowRect is called in main to set this

// global control variables
bool running = true;

// global back buffer variables
int bufferWidth = 0;
int bufferHeight = 0;
BECanvas backBuffer[BENUMBER_WINDOWS];
BITMAPINFO bmpInfo[BENUMBER_WINDOWS] = { 0 };

// global engine variables
BECamera camera({0,3,4}, {0,-2,-1});
BEScene scene;
BERenderPipeline* pipeline[BENUMBER_WINDOWS];
BETimer timers[BENUMBER_WINDOWS];
BETimer loopTimer;

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
			//camera.Pan(0, 0.1f, 0);
			camera.RotatePosition(0, 0.1f);
			break;
		case VK_DOWN:
			//camera.Pan(0, -0.1f, 0);
			camera.RotatePosition(0, -0.1f);
			break;
		case VK_RIGHT:
			//camera.Pan(0.1f, 0, 0);
			camera.RotatePosition(0.1f, 0);
			break;
		case VK_LEFT:
			//camera.Pan(-0.1f, 0, 0);
			camera.RotatePosition(-0.1f, 0);
			break;
		case VK_ADD:
			camera.Pan(0, 0, 0.1f);
			break;
		case VK_SUBTRACT:
			camera.Pan(0, 0, -0.1f);
			break;
		case 'D': //0x44: // D
			camera.RotateDirection(0.1f, 0, 0);
			break;
		case 'A': //0x41: // A
			camera.RotateDirection(-0.1f, 0, 0);
			break;
		case 'S': //0x53: // S
			camera.RotateDirection(0, -0.1f, 0);
			break;
		case 'W': //0x57: // W
			camera.RotateDirection(0, 0.1f, 0);
			break;
		case 'R': //0x52: // R
			pipeline[1]->restartLoop = true;
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

int BERegisterWindowClass(HINSTANCE hInstance)
{
	wc.style = CS_OWNDC; //CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = L"BasicEngingWindow";

	if (!RegisterClass(&wc))
		return 1;

	return 0;
}

bool BEUnregisterWindowClass()
{
	return UnregisterClass(wc.lpszClassName, wc.hInstance);
}

int BECreateWindow(int indx, HINSTANCE hInstance, LPCWSTR name)
{
	hwnd[indx] = CreateWindow(wc.lpszClassName,
		name,
		BE_WINDOWSTYLE,
		windowPosX, windowPosY,
		windowRect.right, windowRect.bottom,
		(indx == 0 ? 0 : hwnd[0]), // parent window
		0, hInstance, NULL);

	windowPosX += windowRect.right;

	if (!hwnd[indx])
		return 2;

	RECT rect;
	GetWindowRect(GetDesktopWindow(), &rect);
	if (windowPosX + windowRect.right > rect.right)
	{
		// next window would be off screen so shuffle down.
		// To Do: tidy up this hack?
		windowPosX = 0;
		windowPosY += windowRect.bottom;
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

/////////////////////////////////
// Ray tracing threading functions

struct BERayTraceSubsection
{
	int indx;
	unsigned int xStart;
	unsigned int width;
	unsigned int yStart;
	unsigned int height;
	bool available;
};

HANDLE rayTraceMutex;
std::vector<BERayTraceSubsection> rayTraceArgs;

int BEThreadFunctionGetNextRayTraceSubsection()
{
	DWORD result = WaitForSingleObject(rayTraceMutex, INFINITE);
	// to do: result error checking

	for (int i = 0; i < rayTraceArgs.size(); i++)
	{
		if (rayTraceArgs[i].available)
		{
			rayTraceArgs[i].available = false;
			ReleaseMutex(rayTraceMutex);
			return i;
		}
	}

	ReleaseMutex(rayTraceMutex);
	return -1;
}

DWORD WINAPI BEThreadFunctionRayTraceSubsection(LPVOID lpParam)
{
	int argIndx = BEThreadFunctionGetNextRayTraceSubsection();

	while (argIndx >= 0)
	{
		BERayTraceSubsection* args = &rayTraceArgs[argIndx];
		pipeline[args->indx]->Draw(args->xStart, args->width, args->yStart, args->height);
		argIndx = BEThreadFunctionGetNextRayTraceSubsection();
	}
	return 0;
}

// note: for now, ensure these are nice division of the buffer size
#define BE_RAYTRACE_SUBSCETIONS_WIDE 8
#define BE_RAYTRACE_SUBSCETIONS_HIGH 6
#define BE_RAYTRACE_THREADS 4

DWORD WINAPI BEThreadFunctionRayTrace(LPVOID lpParam)
{
	DWORD threadIds[BE_RAYTRACE_THREADS] = {};
	HANDLE threadHandles[BE_RAYTRACE_THREADS] = {};

	WCHAR swbuffer[BE_SWBUFFERSIZE];

	int indx = *((int*)lpParam);
	int resultIndx = 3; // to do: hard coded the window indx

	BEWriteOverlayToWindow(resultIndx, L"Starting...");
	BEWriteOverlayToWindow(resultIndx, L""); // To Do: something strange when this is in the thread... need a second 1 for it to show?!

	// create the subsections
	unsigned int sectionWidth = bufferWidth / BE_RAYTRACE_SUBSCETIONS_WIDE;
	unsigned int sectionHeight = bufferHeight / BE_RAYTRACE_SUBSCETIONS_HIGH;

	BERayTraceSubsection arg;
	arg.indx = indx;
	arg.available = true;
	arg.height = sectionHeight;
	arg.width = sectionWidth;

	for (unsigned int y = 0; y < BE_RAYTRACE_SUBSCETIONS_HIGH; y++)
	{
		arg.yStart = y * sectionHeight;
		for (unsigned int x = 0; x < BE_RAYTRACE_SUBSCETIONS_WIDE; x++)
		{
			arg.xStart = x * sectionWidth;
			rayTraceArgs.emplace_back(arg);
		}
	}
	std::random_device rd;
	std::shuffle(rayTraceArgs.begin(), rayTraceArgs.end(), rd);
	// To do: check the end cases for the args width and height

	clock_t lastTime = clock();

	rayTraceMutex = CreateMutex(NULL, FALSE, NULL);
	if (rayTraceMutex == NULL) throw "Error creating raytrace mutex"; // to do: better error checking

	while (running)
	{
		pipeline[indx]->restartLoop = false;
		pipeline[indx]->ResetStats();
		backBuffer[indx].Clear();
		for (int i = 0; i < rayTraceArgs.size(); i++) rayTraceArgs[i].available = true;

		timers[indx].Start();
		
		//pipeline[indx]->Draw(); // single thread version

		// start the threads
		// to do: thread pooling?
		for (int i = 0; i < BE_RAYTRACE_THREADS; i++)
		{
			threadHandles[i] = CreateThread(NULL, 0, BEThreadFunctionRayTraceSubsection, NULL, 0, &(threadIds[i]));
			if (threadHandles[i] == NULL) return -1; /// to do: proper error handling
		}

		WaitForMultipleObjects(BE_RAYTRACE_THREADS, threadHandles, TRUE, INFINITE);

		for (int i = 0; i < BE_RAYTRACE_THREADS; i++)
		{
			CloseHandle(threadHandles[i]);
		}

		timers[indx].Tick();

		// add an extra window, and show the completed image
		BEDrawBackBuffer(indx, resultIndx);

		swprintf(swbuffer, BE_SWBUFFERSIZE, L"Render time: %.2fs", timers[indx].ElapsedSec());
		BEWriteOverlayToWindow(resultIndx, swbuffer);
		BEWriteOverlayToWindow(resultIndx, L""); // To Do: something strange when this is in the thread... need a second 1 for it to show?!
	}

	CloseHandle(rayTraceMutex);

	return 0;
}

/////////////////////////////////
// Main funciton

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	//char buffer[256];
	//GetCurrentDirectoryA(256, buffer);

	MSG msg = { 0 };
	WCHAR swbuffer[BE_SWBUFFERSIZE];

	camera.LookAt(DirectX::g_XMZero);

	BERegisterWindowClass(hInstance);

	// get the window size to ensure the client rect is the size we want.
	BOOL awr = AdjustWindowRectEx(&windowRect, BE_WINDOWSTYLE, false, 0);
	// adjust for the negative offset that results.
	windowRect.right -= windowRect.left;
	windowRect.left = 0;
	windowRect.bottom -= windowRect.top;
	windowRect.top = 0;

	BECreateWindow(0, hInstance, L"Scanline");
	BECreateWindow(1, hInstance, L"Ray tracing");
	BECreateWindow(2, hInstance, L"Wireframe");
	BECreateWindow(3, hInstance, L"Raytrace final");
	BECreateWindow(4, hInstance, L"Direct3D");
	BECreateWindow(5, hInstance, L"Scanline DirectX window");

	scene.CreateSceneTestGround();

	// for scanline rendering
	pipeline[0] = new BERenderPipelineScanline(&scene, &camera, &backBuffer[0]);
	backBuffer[0].AddDepthBuffer(); // to do: automatically add a depth buffer?

	// for raytrace rendering
	pipeline[1] = new BERenderPipelineRaytrace(&scene, &camera, &backBuffer[1]);
	BERenderPipelineRaytrace* pRT = (BERenderPipelineRaytrace*)pipeline[1];

	// for wireframe rendering
	pipeline[2] = new BERenderPipelineWireframe(&scene, &camera, &backBuffer[2]);
	BEDirectX dx2;
	dx2.InitialiseBase(hwnd[2], bufferWidth, bufferHeight);
	BEDXShowCanvas dx2sc(dx2.device, backBuffer[2], true);

	// for DirectX rendering
	BEDirectX dx4;
	dx4.InitialiseBase(hwnd[4], bufferWidth, bufferHeight);
	dx4.Initialise3D(&scene, &camera);
	dx4.LoadScene(&scene);

	// for using DirectX to show the scanline output rather than our BECanvas display
	BEDirectX dx5;
	dx5.InitialiseBase(hwnd[5], bufferWidth, bufferHeight);
	BEDXShowCanvas dx5sc(dx5.device, backBuffer[0], true);

	// ready to go...

	// put ray tracer in a separate thread
	DWORD threadId;
	int arg = 1;
	HANDLE thread = CreateThread(NULL, 0, BEThreadFunctionRayTrace, &arg, 0, &threadId);
	if (thread == NULL) return -1;

	BETimer deltaTime;

	clock_t frametime = 1000 / 60; // 60 frames a second
	clock_t sleeptime = 0;

	clock_t loopstarttime = clock();
	while (running)
	{

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// update loop
		float dt = deltaTime.DeltaTime();
		scene.Update(dt);

		//
		// scanline..........
		//
		backBuffer[0].Clear();
		timers[0].Start();
		pipeline[0]->Draw();
		int t1 = timers[0].Tick().ElapsedMilSec();
		BEDrawBackBuffer(0);
		int t2 = timers[0].Tick().ElapsedMilSec();

		swprintf(swbuffer, BE_SWBUFFERSIZE, L"Rendering time: %ims\nTime inc buffer draw: %ims\nBuffer time: %ims", t1, t2, t2-t1);
		BEWriteOverlayToWindow(0, swbuffer);

		//
		// raytrace.......... running in a thread so showing only
		//
		BEDrawBackBuffer(1);
		pipeline[1]->showBuffer = false;

		int todo = pRT->raysToProcess;
		int done = pRT->raysProcessed;
		float pcnt = (float)done / (float)todo * 100.0f;
		swprintf(swbuffer, BE_SWBUFFERSIZE, L"Rays to process %i\nRays processed %i(%3.2f%%)", todo, done, pcnt);
		BEWriteOverlayToWindow(1, swbuffer);

		//
		// directx.............
		//
		timers[4].Start();
		dx4.DoFrame();
		timers[4].Tick();

		//
		// wireframe.............
		//
		backBuffer[2].Clear();
		timers[2].Start();
		pipeline[2]->Draw();
		t1 = timers[2].Tick().ElapsedMilSec();
		dx2.overlay.message << "Wrieframe draw time: " << t1 << std::endl;
		dx2.overlay.message << "Delta time: " << dt << std::endl;
		dx2.overlay.message << "Sleep time: " << sleeptime << std::endl;
		dx2.DoFrameWithExtra(dx2sc);
		//BEDrawBackBuffer(2); // slow method to show the buffer

		loopTimer.Start(); // here only because of where it gets displayed

		//
		// use dx to show the scanline output
		//
		//dxwindow.ShowBitmap(backBuffer[0]); // uses the generated bmp. To Do: upsidedown
		//dx5.DoFrameWithExtra(dx5sc);

		// to use for the overall loop...
		// to do: limit framerate? Worry about that when it actually works quick

		// lock the framerate
		clock_t currenttime = clock();
		sleeptime = frametime - (currenttime - loopstarttime);
		if (sleeptime > 0) Sleep(sleeptime);
		loopstarttime = currenttime;
	}

	pipeline[1]->exitLoop = true; // tell it to stop!
	WaitForSingleObject(thread, 3000);
	CloseHandle(thread);

	BECleanupWindow(0);
	BECleanupWindow(1);
	BECleanupWindow(2);
	BECleanupWindow(3);
	BECleanupWindow(4);
	BEUnregisterWindowClass();

	return 0;
}

