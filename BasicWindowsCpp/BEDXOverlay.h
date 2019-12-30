#pragma once
#include <d2d1_3.h>
#include <wrl.h>
#include <dwrite.h>
#include "BEDirectXDevice.h"
#include "BECanvas.h"
#include <sstream>

class BEDXOverlay
{
public:

	int Initialise(BEDirectXDevice& dx);

	void Draw();

	void ShowBitmap(BECanvas& canvas);

	inline void ClearMessage() { message.str(std::wstring()); };
	void AppendMessage(std::wstring str) { message << str << std::endl; };

	std::wstringstream message; // the message that will get displayed

	std::wstring fontName = L"Arial";
	float fontSize = 20.0f;
	std::wstring locale = L"en-us";

	D2D1_RECT_F d2rect = {};

private:
	unsigned int width, height;

	Microsoft::WRL::ComPtr<ID2D1Factory3> pFactory = nullptr;
	Microsoft::WRL::ComPtr<ID2D1Device2> pDevice = nullptr;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext2> pContext = nullptr;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> pBitmap = nullptr;

	Microsoft::WRL::ComPtr<IDWriteFactory> pWriteFactory = nullptr;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> pWriteFormat = nullptr;

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pwBrush = nullptr;

	int InitialiseTextWrite();
};

