#pragma once
#include "BEDirectXConstantBuffer.h"

class BEDXEntityPSConstantBuffer : public BEDirectXEntityConstantBuffer
{
public:

	struct Buffer {
		DirectX::XMVECTOR ambientColor = { 1,1,1,1 };
		DirectX::XMVECTOR diffuseColor = { 1,1,1,1 };
		DirectX::XMVECTOR specularColor = { 1,1,1,1 };
		//DirectX::XMVECTOR emmissionColor // to do: not used yet
		float specularExponent = 40.0f;
	};

	BEDXEntityPSConstantBuffer(BEDirectXDevice& device);
	void Update(BEDirectXDevice& device, BEEntity& entity);
	void Bind(BEDirectXDevice& device);


private:
	Buffer buffer = {};
	BEEntity* pEnt;
};

