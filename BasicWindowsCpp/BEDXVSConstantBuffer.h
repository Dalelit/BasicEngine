#pragma once

#include "pch.h"
#include "BEDirectXConstantBuffer.h"
#include "BECamera.h"
#include "BEScene.h"

class BEDXVSConstantBuffer : public BEDirectXConstantBuffer
{
public:
	struct Buffer {
		DirectX::XMMATRIX transformation;
	};

	BEDXVSConstantBuffer(BEDirectXDevice& device, BEScene* pScene, BECamera* pCamera);
	void Update(BEDirectXDevice& device);
	void Bind(BEDirectXDevice& device);

private:
	Buffer buffer = {};
	BEScene* pScene;
	BECamera* pCamera;
};

