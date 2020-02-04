#pragma once
#include "BEDirectXConstantBuffer.h"

class BEDXEntityPSConstantBuffer : public BEDirectXConstantBuffer
{
public:

	struct Buffer {
		DirectX::XMVECTOR color; // temp for now to ensure packing, rather than __declspec(align(32))
		bool textured;
	};

	BEDXEntityPSConstantBuffer(BEDirectXDevice& device, BEMesh& mesh, BEEntity& entity);
	void Update(BEDirectXDevice& device);
	void Bind(BEDirectXDevice& device);

private:
	Buffer buffer = {};
	BEEntity* pEntity;
};

