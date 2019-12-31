#pragma once
#include "BEDirectXConstantBuffer.h"

class BEDXEntityVSConstantBuffer : public BEDirectXConstantBuffer
{
public:

	struct Buffer {
		DirectX::XMVECTOR position;
	};

	BEDXEntityVSConstantBuffer(BEDirectXDevice& device, BEEntity* pEntity);
	void Update(BEDirectXDevice& device);
	void Bind(BEDirectXDevice& device);

private:
	Buffer buffer = {};
	BEEntity* pEntity;
};

