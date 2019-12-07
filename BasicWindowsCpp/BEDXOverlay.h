#pragma once
#include <d2d1_1.h>
#include <wrl.h>
#include <dwrite.h>
#include "BEDirectXDevice.h"

class BEDXOverlay
{
public:

	int Initialise(BEDirectXDevice& dx);

	void Draw();

	D2D1_COLOR_F whiteSolid = { D2D1::ColorF::White, 1.0f };
	D2D1_COLOR_F oredSolid = { D2D1::ColorF::OrangeRed, 1.0f };

private:
	unsigned int width, height;

	Microsoft::WRL::ComPtr<ID2D1DeviceContext> pContext = nullptr;
	//Microsoft::WRL::ComPtr<IDWriteFactory> pWriteFactory = nullptr;
	//Microsoft::WRL::ComPtr<ID2D1RenderTarget> pRenderTgt = nullptr;

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pwBrush = nullptr;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> porBrush = nullptr;

};

