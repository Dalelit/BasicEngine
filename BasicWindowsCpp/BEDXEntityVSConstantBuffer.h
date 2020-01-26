#pragma once
#include "BEDirectXConstantBuffer.h"

class BEDXEntityVSConstantBuffer : public BEDirectXConstantBuffer
{
public:

	struct Buffer {
		DirectX::XMMATRIX transform;
		DirectX::XMMATRIX transformRotation;
	};

	BEDXEntityVSConstantBuffer(BEDirectXDevice& device, BEEntity& entity);
	void Update(BEDirectXDevice& device);
	void Bind(BEDirectXDevice& device);

private:
	Buffer buffer = {};
	BEEntity& entity;
};

