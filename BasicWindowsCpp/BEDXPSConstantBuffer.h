#pragma once

#include "BEDirectXConstantBuffer.h"
#include "BECamera.h"
#include "BEScene.h"
#include <DirectXMath.h>

class BEDXPSConstantBuffer : public BEDirectXConstantBuffer
{
public:
	struct Buffer {
		DirectX::XMVECTOR lightDirection;
		DirectX::XMVECTOR lightColor;
	};

	BEDXPSConstantBuffer(BEDirectXDevice& device, BEScene* pScene, BECamera* pCamera);
	void Update(BEDirectXDevice& device);
	void Bind(BEDirectXDevice& device);

private:
	Buffer buffer = {};
	BEScene* pScene;
	BECamera* pCamera;
};

