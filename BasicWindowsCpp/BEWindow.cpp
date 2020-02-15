#include "pch.h"
#include "BEWindow.h"

#define WINDOWSTYLE WS_VISIBLE | WS_CAPTION

BEWindow::BEWindow(WindowDesc desc) : desc(desc)
{
	handle = CreateWindow(windowClass.lpszClassName,
		desc.name.c_str(),
		WINDOWSTYLE,
		desc.left, desc.top,
		desc.width, desc.height,
		0, // parent window
		0, desc.hInstance, NULL);

	deviceContext = GetDC(handle);

	RECT rect;
	GetClientRect(handle, &rect);
	desc.width = rect.right;
	desc.height = rect.bottom;

	CreateBackBuffer();
	backBuffer.Clear();

	windowClassCount++;
}

BEWindow::~BEWindow()
{
	ReleaseDC(handle, deviceContext);
	windowClassCount--;

	UnregsiterWindowClass();
}

void BEWindow::WriteText(std::wstring message)
{
	RECT rect;
	GetClientRect(handle, &rect);

	SetTextColor(deviceContext, RGB(255, 255, 255));
	SetBkMode(deviceContext, TRANSPARENT);
	//SetBkColor(deviceContext, RGB(0, 0, 0));
	//HFONT hFont = (HFONT)GetStockObject(ANSI_FIXED_FONT);
	//SelectObject(deviceContext, hFont);
	
	DrawText(deviceContext, message.c_str(), -1, &rect, DT_LEFT);
}

void BEWindow::GetAdjustedWindowRect(RECT* rect)
{
	BOOL awr = AdjustWindowRectEx(rect, WINDOWSTYLE, false, 0);
	// adjust for the negative offset that results.
	rect->right -= rect->left;
	rect->left = 0;
	rect->bottom -= rect->top;
	rect->top = 0;
	
}

void BEWindow::CreateBackBuffer()
{
	assert(desc.width > 0 && desc.height > 0);

	backBuffer.Initialise(desc.width, desc.height);

	bmpInfo.bmiHeader.biSize = sizeof(bmpInfo.bmiHeader);
	bmpInfo.bmiHeader.biWidth = desc.width;
	bmpInfo.bmiHeader.biHeight = desc.height; // use - for top-down DIB and its origin is the upper-left corner
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
}

void BEWindow::DrawBackBuffer()
{
	DrawBackBuffer(&backBuffer);
}

void BEWindow::DrawBackBuffer(BECanvas* pCanvas)
{
	StretchDIBits(
		deviceContext,
		0,
		0,
		desc.width,
		desc.height,
		0,
		0,
		desc.width,
		desc.height,
		pCanvas->bmpSurface->GetData(),
		&bmpInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
}

unsigned int BEWindow::windowClassCount = 0;
WNDCLASS BEWindow::windowClass = {};

void BEWindow::RegsiterWindowClass(HINSTANCE hInstance, WNDPROC wndProc)
{
	windowClass = { 0 };
	windowClass.style = CS_OWNDC; //CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = wndProc;
	windowClass.hInstance = hInstance;
	windowClass.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	windowClass.lpszClassName = L"BasicEngingWindow";

	HRESULT hr = RegisterClass(&windowClass);
	// to do: error checking
}

void BEWindow::UnregsiterWindowClass()
{
	if (windowClassCount == 0)
	{
		UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
	}
}
