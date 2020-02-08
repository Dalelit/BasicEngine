#pragma once
#include "BEDirectXConstantBuffer.h"

class BEDXEntityPSConstantBuffer : public BEDirectXEntityConstantBuffer
{
public:

	struct Buffer {
		DirectX::XMVECTOR color;
	};

	BEDXEntityPSConstantBuffer(BEDirectXDevice& device);
	void Update(BEDirectXDevice& device, BEEntity& entity);
	void Bind(BEDirectXDevice& device);

private:
	Buffer buffer = {};
	BEEntity* pEnt;
};

