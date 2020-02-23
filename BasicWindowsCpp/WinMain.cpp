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


#include "pch.h"

#include "BEWindow.h"
#include "BELogger.h"
#include "BERenderRaytrace.h"
#include "BERenderRaytraceThread.h"
#include "BERenderProgrammablePipeline.h"
#include "BEDirectX.h"
#include "BETimer.h"
#include "BESceneTests.h"
#include "BEInput.h"

// global control variables
bool running = true;
bool raytraceKeepRunning = true;
bool raytraceDrawResult = false;
std::wstring raytraceDrawMessage;

void RaytraceShowResult(std::wstring message)
{
	raytraceDrawResult = true;
	raytraceDrawMessage = message;
}

// global engine variables
BECamera camera;
BEScene scene;
BEInput input;
BETimer loopTimer;

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
		input.KeyDownInput((char)wParam, (lParam & 0xffff));

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
		BYTE rawBuffer[1024]; // to do: hacking away... best way to set this up?
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
	BELoggerConsole::Init();

	BELOG_DEBUG("Starting...");

	char buffer[256];
	GetCurrentDirectoryA(256, buffer);
	BELOG_DEBUG("Directory " + std::string(buffer));

	MSG msg = { 0 };

	RECT windowRect = { 0,0, 800, 600 }; // AdjustWindowRect is called in main to set this

	BEWindow::GetAdjustedWindowRect(&windowRect); // get the window size to ensure the client rect is the size we want.

	BEWindow::RegsiterWindowClass(hInstance, WndProc);
	
	BEWindow::WindowDesc wndDesc;
	wndDesc.name = L"Points";
	wndDesc.hInstance = hInstance;
	wndDesc.left = 0;
	wndDesc.top = 0;
	wndDesc.width = windowRect.right;
	wndDesc.height = windowRect.bottom;

	BEWindow wndPoints(wndDesc);

	wndDesc.name = L"Wireframe";
	wndDesc.left += wndDesc.width;

	BEWindow wndWireframe(wndDesc);

	wndDesc.name = L"Full render";
	wndDesc.left += wndDesc.width;

	BEWindow wndFullrender(wndDesc);

	wndDesc.name = L"Raytrace working";
	wndDesc.left = 0;
	wndDesc.top += wndDesc.height;

	BEWindow wndRaytraceWorking(wndDesc);

	wndDesc.name = L"Raytrace final";
	wndDesc.left += wndDesc.width;

	BEWindow wndRaytraceFinal(wndDesc);

	wndDesc.name = L"DirectX";
	wndDesc.left += wndDesc.width;

	BEWindow wndDirectX(wndDesc);

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
	BERenderProgrammablePipeline pointsPL(&scene, &camera, wndPoints.GetBackBuffer());
	pointsPL.pRasterizerFunc = &BERenderProgrammablePipeline::RasterizerWireFrameAndNormals;
	pointsPL.pPixelShaderFunc = &BERenderProgrammablePipeline::PixelShaderColor;
	pointsPL.backFaceCull = false;

	// wireframe
	BERenderProgrammablePipeline wireframePL(&scene, &camera, wndWireframe.GetBackBuffer());
	wireframePL.pRasterizerFunc = &BERenderProgrammablePipeline::RasterizerWireframe;
	wireframePL.pPixelShaderFunc = &BERenderProgrammablePipeline::PixelShaderColor;
	wireframePL.backFaceCull = false;

	// Full output
	BERenderProgrammablePipeline mainPL(&scene, &camera, wndFullrender.GetBackBuffer());
	//mainPL.pRasterizerFunc = &BERenderProgrammablePipeline::RasterizerTriangle;
	//mainPL.pPixelShaderFunc = &BERenderProgrammablePipeline::PixelShaderPointOnly;

	// Raytrace rendering
	BERenderRaytrace raytracing(&scene, &camera, wndRaytraceWorking.GetBackBuffer());
	BERenderRaytraceThread raytracingThread(raytracing);
	raytracingThread.callback = RaytraceShowResult;
	raytracingThread.pResultCanvas = wndRaytraceFinal.GetBackBuffer();

	// for DirectX rendering
	BEDirectX dx;
	dx.Initialise(wndDirectX.GetHandle(), wndDirectX.GetBackBuffer()->Width(), wndDirectX.GetBackBuffer()->Height());
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
		wndPoints.Present(pointsPL.GetStats());

		//
		// wireframe..........
		//
		wireframePL.Draw();
		wndWireframe.Present(wireframePL.GetStats());

		//
		// main pipeline..........
		//
		mainPL.Draw();
		wndFullrender.Present(mainPL.GetStats());

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

		wndRaytraceWorking.Present(raytracing.GetWorkingStats());

		if (raytraceDrawResult)
		{
			wndRaytraceFinal.Present(raytraceDrawMessage);
			raytraceDrawResult = false;
		}

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

	BELOG_DEBUG("Done.");

	return 0;
}

