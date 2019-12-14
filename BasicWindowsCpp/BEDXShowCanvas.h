#pragma once

#include "BEMesh.h"
#include "BEDirectX.h"

class BEDXShowCanvas
{
public:
	BEDXShowCanvas();
	void Initialise(BEDirectX& dx);
	void InitialiseTexture(BEDirectX& dx, BECanvas& canvas);
	void UpdateTexture(BEDirectX& dx, BECanvas& canvas);
	void Draw(BEDirectX& dx, BECanvas& canvas);

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D1> pTexture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerState = nullptr;
	BETexVertex mesh[6];
};

