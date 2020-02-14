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
#include "BERenderRaytraceThread.h"
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

	camera.viewDistance = 100.0f;
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
	BERenderRaytraceThread raytracingThread(raytracing);
	raytracingThread.callback = [](std::wstring msg) {
		constexpr int resultIndx = 4;
		BEDrawBackBuffer(3, resultIndx);
		BEWriteOverlayToWindow(resultIndx, msg.c_str());
		BEWriteOverlayToWindow(resultIndx, L""); // To Do: something strange when this is in the thread... need a second 1 for it to show?!
	};

	// for DirectX rendering
	BEDirectX dx;
	dx.Initialise(hwnd[5], bufferWidth, bufferHeight);
	dx.LoadScene(&scene, &camera);

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

		if (!input.keyEvents.empty())
		{
			raytracingThread.StopAndWait();

			auto e = input.keyEvents.front();
			input.keyEvents.pop();

			scene.Clear();
			switch (e.key - '0')
			{
			case 0:
				BESceneTests::CreateSceneTest0(scene);
				break;
			case 1:
				BESceneTests::CreateSceneTest1(scene);
				break;
			case 2:
				BESceneTests::CreateSceneTest2(scene);
				break;
			case 3:
				BESceneTests::CreateSceneTest3(scene);
				break;
			case 4:
			default:
				BESceneTests::CreateBoxWorld(scene, camera);
				break;
			}
			scene.Update(0);
			dx.LoadScene(&scene, &camera);
		}

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
		if (raytraceKeepRunning && !raytracingThread.IsRunning())
		{
			raytracingThread.SetContinousLoop();
			raytracingThread.Start();
		}
		else if (!raytraceKeepRunning && raytracingThread.IsRunning())
		{
			raytracingThread.StopContinousLoop();
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

	raytracingThread.StopAndWait();

	BECleanupWindow(0);
	BECleanupWindow(1);
	BECleanupWindow(2);
	BECleanupWindow(3);
	BECleanupWindow(4);
	BEUnregisterWindowClass();

	return 0;
}

