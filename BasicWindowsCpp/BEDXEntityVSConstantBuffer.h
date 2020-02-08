#pragma once
#include "BEDirectXConstantBuffer.h"

class BEDXEntityVSConstantBuffer : public BEDirectXEntityConstantBuffer
{
public:

	struct Buffer {
		DirectX::XMMATRIX transform;
		DirectX::XMMATRIX transformRotation;
	};

	BEDXEntityVSConstantBuffer(BEDirectXDevice& device);
	void Update(BEDirectXDevice& device, BEEntity& entity);
	void Bind(BEDirectXDevice& device);

private:
	Buffer buffer = {};
};

