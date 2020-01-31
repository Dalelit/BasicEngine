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
#include "BERenderRaytrace.h"
#include "BERenderProgrammablePipeline.h"
#include "BEDirectX.h"
#include "BETimer.h"
#include "BEDXShowCanvas.h"
#include "BESceneTests.h"
#include "BEInput.h"

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
BYTE rawBuffer[1024]; // to do: stop with the hacking and do a raw buffer properly. Using for the mouse input.

// global control variables
bool running = true;
bool doUpdate = true;
bool raytraceIsRunning = false;
bool raytraceKeepRunning = true;

// global back buffer variables
int bufferWidth = 0;
int bufferHeight = 0;
BECanvas backBuffer[BENUMBER_WINDOWS];
BITMAPINFO bmpInfo[BENUMBER_WINDOWS] = { 0 };

// global engine variables
BECamera camera;
BEScene scene;
BEInput input;
BETimer loopTimer;
BETimer timers[BENUMBER_WINDOWS];
BERenderRaytrace* pRayTracer = nullptr;

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

long drawBackBufferTime = 0;
long drawBackBufferCount = 0;
void BEDrawBackBuffer(int bufferindx, int windowIndx)
{
	drawBackBufferCount++;
	clock_t startTime = clock();

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
		backBuffer[bufferindx].bmpSurface->GetData(),
		&(bmpInfo[bufferindx]),
		DIB_RGB_COLORS,
		SRCCOPY
	);

	drawBackBufferTime += clock() - startTime;
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
	/////// Keyboard
	case WM_KEYUP:
		input.KeyUpInput((char)wParam);
		break;
	case WM_KEYDOWN:
	{
		input.KeyDownInput((char)wParam);

		// keeping these for basic controls for the moment
		switch (wParam)
		{
		case VK_ESCAPE:
			running = false;
			break;
		case 'R':
			pRayTracer->restartLoop = true;
			break;
		case 'P':
			raytraceKeepRunning = !raytraceKeepRunning;
			break;
		}
		break;
	}
	/////// Raw mouse
	case WM_INPUT:
	{
		UINT size;
		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1) break;

		if (size == 0) break;

		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, rawBuffer, &size, sizeof(RAWINPUTHEADER)) != size) throw "Raw buffer read did not match"; // to do: error hacking

		RAWINPUT* raw = (RAWINPUT*)rawBuffer;

		if (raw->header.dwType == RIM_TYPEMOUSE) input.RawMouseInput(raw->data.mouse.lLastX, raw->data.mouse.lLastY);

		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		running = false;
		break;
	}
	default:
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

void BESetupRawMouseIntput()
{
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01; // mouse page
	rid.usUsage = 0x02; // mouse usage
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
	{
		throw "Error with RegisterRawInputDevices"; // to do: really need some error handling... more hacking
	}
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
		pRayTracer->Draw(args->xStart, args->width, args->yStart, args->height);
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
	raytraceIsRunning = true;

	DWORD threadIds[BE_RAYTRACE_THREADS] = {};
	HANDLE threadHandles[BE_RAYTRACE_THREADS] = {};

	WCHAR swbuffer[BE_SWBUFFERSIZE];

	//int indx = *((int*)lpParam);
	int indx = 3; // to do: hard coded the working window indx
	int resultIndx = 4; // to do: hard coded the result window indx

	BEWriteOverlayToWindow(resultIndx, L"Starting...");
	BEWriteOverlayToWindow(resultIndx, L""); // To Do: something strange when this is in the thread... need a second 1 for it to show?!

	// create the subsections if not already created
	if (rayTraceArgs.size() == 0)
	{
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
	}

	clock_t lastTime = clock();

	rayTraceMutex = CreateMutex(NULL, FALSE, NULL);
	if (rayTraceMutex == NULL) throw "Error creating raytrace mutex"; // to do: better error checking

	while (running && raytraceKeepRunning)
	{
		pRayTracer->restartLoop = false;
		pRayTracer->ResetStats();
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

	raytraceIsRunning = false;

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

	BERegisterWindowClass(hInstance);

	// get the window size to ensure the client rect is the size we want.
	BOOL awr = AdjustWindowRectEx(&windowRect, BE_WINDOWSTYLE, false, 0);
	// adjust for the negative offset that results.
	windowRect.right -= windowRect.left;
	windowRect.left = 0;
	windowRect.bottom -= windowRect.top;
	windowRect.top = 0;

	BECreateWindow(0, hInstance, L"Points");
	BECreateWindow(1, hInstance, L"Wireframe");
	BECreateWindow(2, hInstance, L"Full");
	BECreateWindow(3, hInstance, L"Raytrace working");
	BECreateWindow(4, hInstance, L"Raytrace final");
	BECreateWindow(5, hInstance, L"Direct3D");

	BESetupRawMouseIntput();

	camera.viewDistance = 10.0f;
	camera.SetPosition(0, 1, 4);
	camera.LookAt(0, 0, 0);

	BESceneTests::CreateSceneTest0(scene);
	//BESceneTests::CreateSceneTest1(scene);
	//BESceneTests::CreateSceneTest2(scene);
	//BESceneTests::CreateSceneTest3(scene);
	//BESceneTests::CreateBoxWorld(scene, camera);

	scene.Update(0);

	// points
	BERenderProgrammablePipeline pointsPL(&scene, &camera, &backBuffer[0]);
	pointsPL.pRasterizerFunc = &BERenderProgrammablePipeline::RasterizerWireFrameAndNormals;
	pointsPL.pPixelShaderFunc = &BERenderProgrammablePipeline::PixelShaderColor;
	pointsPL.backFaceCull = false;

	// wireframe
	BERenderProgrammablePipeline wireframePL(&scene, &camera, &backBuffer[1]);
	wireframePL.pRasterizerFunc = &BERenderProgrammablePipeline::RasterizerWireframe;
	wireframePL.pPixelShaderFunc = &BERenderProgrammablePipeline::PixelShaderColor;
	wireframePL.backFaceCull = false;

	// Full output
	BERenderProgrammablePipeline mainPL(&scene, &camera, &backBuffer[2]);
	//mainPL.pRasterizerFunc = &BERenderProgrammablePipeline::RasterizerTriangle;
	//mainPL.pPixelShaderFunc = &BERenderProgrammablePipeline::PixelShaderPointOnly;

	// Raytrace rendering
	BERenderRaytrace raytracing(&scene, &camera, &backBuffer[3]);
	pRayTracer = &raytracing;
	DWORD threadId;
	HANDLE thread = nullptr;

	// for DirectX rendering
	BEDirectX dx;
	dx.InitialiseBase(hwnd[5], bufferWidth, bufferHeight);
	dx.Initialise3D(&scene, &camera);
	dx.LoadScene(&scene);

	// ready to go...

	BETimer deltaTime;
	clock_t frametime = 1000 / 60; // 60 frames a second
	clock_t sleeptime = 0;

	long frameCount = 0;
	long totalUpdateTime = 0;

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
		clock_t updateStartTime = clock();

		input.Update(dt, camera);
		if (!input.paused) scene.Update(dt);

		totalUpdateTime += updateStartTime - clock();
		frameCount++;

		//
		// points..........
		//
		pointsPL.Draw();
		BEDrawBackBuffer(0);
		std::wstring msg = pointsPL.GetStats();
		msg += L"Avg update time: " + std::to_wstring( (float)totalUpdateTime / (float)frameCount ) + L"ms\n";
		msg += L"Draw back buffer avg time: " + std::to_wstring((float)drawBackBufferTime / (float)drawBackBufferCount) + L"ms\n";
		BEWriteOverlayToWindow(0, msg.c_str());

		//
		// wireframe..........
		//
		wireframePL.Draw();
		BEDrawBackBuffer(1);
		BEWriteOverlayToWindow(1, wireframePL.GetStats().c_str());

		//
		// main pipeline..........
		//
		mainPL.Draw();
		BEDrawBackBuffer(2);
		BEWriteOverlayToWindow(2, mainPL.GetStats().c_str());

		//
		// raytrace..........
		//
		if (raytraceKeepRunning && !raytraceIsRunning)
		{
			if (thread != NULL) CloseHandle(thread);
			
			// put ray tracer in a separate thread
			int arg = 0;
			thread = CreateThread(NULL, 0, BEThreadFunctionRayTrace, &arg, 0, &threadId);
		}

		BEDrawBackBuffer(3);
		raytracing.showBuffer = false;

		int todo = raytracing.raysToProcess;
		int done = raytracing.raysProcessed;
		float pcnt = (float)done / (float)todo * 100.0f;
		swprintf(swbuffer, BE_SWBUFFERSIZE, L"Rays to process %i\nRays processed %i(%3.2f%%)", todo, done, pcnt);
		BEWriteOverlayToWindow(3, swbuffer);

		//
		// directx.............
		//
		dx.DoFrame();


		// lock the framerate
		clock_t currenttime = clock();
		sleeptime = frametime - (currenttime - loopstarttime);
		if (sleeptime > 0) Sleep(sleeptime);
		loopstarttime = currenttime;
	}

	raytracing.exitLoop = true; // tell it to stop!
	if (thread != NULL)
	{
		WaitForSingleObject(thread, 3000);
		CloseHandle(thread);
	}

	BECleanupWindow(0);
	BECleanupWindow(1);
	BECleanupWindow(2);
	BECleanupWindow(3);
	BECleanupWindow(4);
	BEUnregisterWindowClass();

	return 0;
}

