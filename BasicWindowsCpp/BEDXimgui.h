#pragma once

#include "BEDirectXDevice.h"

class BEDXimgui
{
public:
	static LRESULT WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	~BEDXimgui();

	void Initialise(HWND hwnd, BEDirectXDevice& device);

	void NewFrame();
	void Draw();

	void Enable() { enabled = true; }
	void Disable() { enabled = false; }
	bool IsEnabled() { return enabled; }

	bool WantCaptureKeyboard();

private:
	bool enabled = true;
};