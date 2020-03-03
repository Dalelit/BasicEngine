#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "BECanvas.h"


class BEWindow
{
public:
	struct WindowDesc
	{
		std::wstring name;
		HINSTANCE hInstance;
		unsigned int left;
		unsigned int top;
		unsigned int width;
		unsigned int height;
	};

	static void RegsiterWindowClass(HINSTANCE hInstance, WNDPROC wndProc);

	BEWindow() = delete;
	BEWindow(BEWindow&) = delete;
	BEWindow(WindowDesc descriptor);
	~BEWindow();

	BECanvas* GetBackBuffer() { return &backBuffer; }
	HWND GetHandle() { return handle; }

	void Present();
	void Present(std::wstring message);

	void Hide();
	void Show();
	void ShowMaximised();
	void ShowFullScreen();
	void Restore();

	inline bool IsVisible() { return visible; }
	inline bool IsFullScreen() { return fullScreen; }
	inline bool IsFullScreenVisible() { return fullScreen && visible; }

	inline unsigned int GetClientWidth() { return rect.right; }
	inline unsigned int GetClientHeight() { return rect.bottom; }

	static void GetAdjustedWindowRect(RECT* rect);

protected:
	WindowDesc desc;
	HWND handle;
	HDC deviceContext;
	BITMAPINFO bmpInfo;
	BECanvas backBuffer;
	bool visible;
	RECT rect;

	WINDOWPLACEMENT winPlacement;
	bool fullScreen = false;
	void ToggleFullScreen();

	void CreateBackBuffer();
	void ResizeBackBuffer();
	void DrawBackBuffer(BECanvas* pCanvas);
	void WriteText(std::wstring message); // to do: a version that works with reference rather than copy

	void UpdateRectSize();

	static WNDCLASS windowClass;
	static DWORD windowStyle;
	static unsigned int windowClassCount;
	static void UnregsiterWindowClass();
};

