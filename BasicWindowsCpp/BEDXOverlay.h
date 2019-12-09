#pragma once
#include <d2d1_3.h>
#include <wrl.h>
#include <dwrite.h>
#include "BEDirectXDevice.h"

class BEDXOverlay
{
public:

	int Initialise(BEDirectXDevice& dx);

	void Draw();

private:
	unsigned int width, height;

	Microsoft::WRL::ComPtr<ID2D1Factory3> pFactory = nullptr;
	Microsoft::WRL::ComPtr<ID2D1Device2> pDevice = nullptr;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext2> pContext = nullptr;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> pBitmap = nullptr;
	//Microsoft::WRL::ComPtr<IDWriteFactory> pWriteFactory = nullptr;

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pwBrush = nullptr;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> porBrush = nullptr;

};

